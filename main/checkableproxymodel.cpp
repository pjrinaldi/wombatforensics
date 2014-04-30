/*
    CheckableProxyModel: a Qt proxy model to add a checkbox to column 0
        of each row of any QAbstractItemModel

    Copyright (C) 2011  Andre Somers <andre@familiesomers.nl>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "checkableproxymodel.h"
#include <QtDebug>
#include <QTimer>
#include <QQueue>
#include "delayedexecutiontimer.h"


#ifdef CHECKABLEPROXYMODEL_DEBUG
class StackPopper {
public:
    StackPopper(QStack<QModelIndex>* stack):
        m_stack(stack)
    {}

    ~StackPopper() {
        m_stack->pop();
    }

private:
    QStack<QModelIndex>* m_stack;
};
#endif
/**
  Class to add check boxes to any item model.

  This class can be used to add a check box before every item in column 0 of any QAbstractItemModel.
  It will work with list-type models, but a lot of the complexity comes from being able to deal
  with tree-type models.

  For tree type models, branch items in the tree are checkable. Checking a branch item will check
  all decendent items, and unchecking a branch item will uncheck all decendent items as well. If
  a branch item has both checked and unchecked decending items, the branch item will be displayed
  in a partially checked state.

  CheckableProxyModel achieves internally uses an efficient way to keep track of the check state of
  each item in the model. This allows it to deal efficiently with big models, and even with models
  that are populated lazily like QFileSystemModel.

  To retreive lists of (un-) checked items in the model, use the checkedState() method.
  */

CheckableProxyModel::CheckableProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_cleanupTimer(new DelayedExecutionTimer(30000, 1000, this)),
    m_periodicalCleanupTimer(new QTimer(this))
{
    m_baseState.defaultChildState = CheckableProxyModel::Unchecked;
    m_baseState.nodeState = CheckableProxyModel::Unchecked;

    setDynamicSortFilter(true);
    connect (m_cleanupTimer, SIGNAL(triggered()), SLOT(cleanupStorage()));

    connect (m_periodicalCleanupTimer, SIGNAL(timeout()), m_cleanupTimer, SLOT(trigger()));
    m_periodicalCleanupTimer->setInterval(5 * 60 * 1000); // 5 minutes
    m_periodicalCleanupTimer->start();
}


Qt::ItemFlags CheckableProxyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    QModelIndex sourceIndex = mapToSource(index);
    Qt::ItemFlags flags = sourceIndex.flags();
    if (index.column() == 0) {
        flags |= Qt::ItemIsUserCheckable;
        if (sourceIndex.model()->hasChildren(sourceIndex)) {
            flags |= Qt::ItemIsTristate;
        }
    }

    return flags;
}


void CheckableProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    if (sourceModel == QSortFilterProxyModel::sourceModel())
        return;

    QSortFilterProxyModel::setSourceModel(sourceModel);
    m_checkStates.clear();
}


QVariant CheckableProxyModel::data(const QModelIndex &index, int role) const
{
    QModelIndex sourceIndex = mapToSource(index);

    if (index.column() == 0 && role == Qt::CheckStateRole) {
        return QVariant(resolveCheckStateRole(sourceIndex));
    }

    return sourceIndex.data(role);
}


bool CheckableProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex sourceIndex = mapToSource(index);

    if (index.column() == 0 && role == Qt::CheckStateRole) {
        Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
        return setCheckState(sourceIndex, state);
    }

    return QSortFilterProxyModel::sourceModel()->setData(sourceIndex, value, role);
}


Qt::CheckState CheckableProxyModel::resolveCheckStateRole(QModelIndex sourceIndex) const
{
#ifdef CHECKABLEPROXYMODEL_DEBUG
    if (m_indexStack.contains(sourceIndex)) {
        qWarning() << "Loop in resolving checkStateRole! Stack contents";
        foreach(QModelIndex index, m_indexStack) {
            qDebug() << "  " << index << index.data(Qt::UserRole + 1).toString();
        }
        qWarning() << "while trying to resolve" << sourceIndex << sourceIndex.data(Qt::UserRole + 1).toString();
        return Qt::PartiallyChecked;
    }
    m_indexStack.push(sourceIndex);
    StackPopper popper(&m_indexStack);
#endif

    NodeState nodeState = getTreeNodeState(sourceIndex);
    switch (nodeState.nodeState) {
    case CheckableProxyModel::DeterminedByParent:
    {
        NodeState parentState = getTreeNodeState(sourceIndex.parent());
        if (parentState.nodeState == CheckableProxyModel::DeterminedByChildren) {
            return (parentState.defaultChildState == CheckableProxyModel::Checked ?
                        Qt::Checked :
                        Qt::Unchecked );
        }
        Qt::CheckState state = resolveCheckStateRole(sourceIndex.parent());
        return state;
        break;
    }
    case CheckableProxyModel::DeterminedByChildren:
        return getCombinedChildrenCheckState(sourceIndex);
        break;
    case CheckableProxyModel::Checked:
        return Qt::Checked;
    case CheckableProxyModel::Unchecked:
        return Qt::Unchecked;
    }

    //shut up compiler about control reaching end of non-void function. The switch above covers all cases
    return Qt::PartiallyChecked;
}

CheckableProxyModel::NodeState CheckableProxyModel::getTreeNodeState(QModelIndex sourceIndex) const
{
    if (!sourceIndex.isValid())
        return m_baseState;

    QPersistentModelIndex pIndex(sourceIndex);
    return m_checkStates.value(pIndex, NodeState(CheckableProxyModel::DeterminedByParent, m_baseState.defaultChildState));
}


Qt::CheckState CheckableProxyModel::getCombinedChildrenCheckState(QModelIndex sourceIndex) const
{
    if (!sourceIndex.isValid()) {
        qWarning() << "Inconsistency in checkstates!" << __LINE__;
        return Qt::PartiallyChecked;
    }

    int rows = sourceModel()->rowCount(sourceIndex);
    if (rows == 0) {
        qWarning() << "Inconsistency in checkstates!" << __LINE__;
        return Qt::PartiallyChecked;
    }

    Qt::CheckState state;

    TreeCheckState defaultChildState = getTreeNodeState(sourceIndex).defaultChildState;
    //get state of first child item
    NodeState nodeState = getTreeNodeState(sourceModel()->index(0, 0, sourceIndex));
    switch (nodeState.nodeState) {
    case CheckableProxyModel::Checked:
        state = Qt::Checked;
        break;
    case CheckableProxyModel::Unchecked:
        state = Qt::Unchecked;
        break;
    case CheckableProxyModel::DeterminedByParent:
    {
        switch (defaultChildState) {
        case CheckableProxyModel::Checked:
            state = Qt::Checked;
            break;
        case CheckableProxyModel::Unchecked:
            state = Qt::Unchecked;
            break;
        default:
            qWarning() << "Inconsistency in checkstates!" << __LINE__ << sourceIndex;
        }
        break;
    }
    case CheckableProxyModel::DeterminedByChildren:
        state = getCombinedChildrenCheckState(sourceModel()->index(0, 0, sourceIndex));
        if (state == Qt::PartiallyChecked)
            return Qt::PartiallyChecked;

        break;
    }

    //match with state of next child items
    if (rows > 1) {
        for (int i = 1; i < rows; ++i) {
            NodeState nodeState = getTreeNodeState(sourceModel()->index(i, 0, sourceIndex));
            switch (nodeState.nodeState) {
            case CheckableProxyModel::Checked:
                if (state == Qt::Unchecked)
                    return Qt::PartiallyChecked;
                break;
            case CheckableProxyModel::Unchecked:
                if (state == Qt::Checked)
                    return Qt::PartiallyChecked;
                break;
            case CheckableProxyModel::DeterminedByParent:
            {
                switch (defaultChildState) {
                case CheckableProxyModel::Checked:
                    if (state == Qt::Unchecked)
                        return Qt::PartiallyChecked;
                    break;
                case CheckableProxyModel::Unchecked:
                    if (state == Qt::Checked)
                        return Qt::PartiallyChecked;
                    break;
                default:
                    qWarning() << "Inconsistency in checkstates!" << __LINE__ << sourceModel()->index(i,0,sourceIndex) << sourceModel()->index(i,0,sourceIndex).data(Qt::DisplayRole).toString();
                    return Qt::PartiallyChecked;
                }
                break;
            }
            case CheckableProxyModel::DeterminedByChildren:
                Qt::CheckState childState = getCombinedChildrenCheckState(sourceModel()->index(i, 0, sourceIndex));
                if (childState == Qt::PartiallyChecked || childState != state)
                    return Qt::PartiallyChecked;

                break;
            }
        }
    }

    if (state == Qt::Checked || state == Qt::Unchecked) {
#ifdef CHECKABLEPROXYMODEL_DEBUG
        qDebug() << "schedule hash cleanup";
#endif
        m_cleanupTimer->trigger();
    }
    return state;
}


bool CheckableProxyModel::setCheckState(QModelIndex sourceIndex, Qt::CheckState state)
{
#ifdef CHECKABLEPROXYMODEL_DEBUG
    qDebug() << "setCheckState" << state;
#endif
    QPersistentModelIndex pIndex(sourceIndex);
    CheckableProxyModel::TreeCheckState treeState(CheckableProxyModel::Checked);
    if (state == Qt::Unchecked) {
        treeState = CheckableProxyModel::Unchecked;
    } else if (state == Qt::PartiallyChecked) {
        qWarning() << "Unexpected new tree state.";
        return false;
    }

    CheckableProxyModel::TreeCheckState oldTreeState(CheckableProxyModel::DeterminedByParent);

    if (m_checkStates.contains(pIndex)) {
        oldTreeState = m_checkStates.value(pIndex).nodeState;
        if (oldTreeState == treeState)
            return true;
    }

    QModelIndexList changedIndices;
    m_checkStates.insert(pIndex, NodeState(treeState));
#ifdef CHECKABLEPROXYMODEL_DEBUG
    qDebug() << "updated node" << sourceIndex << sourceIndex.data(Qt::DisplayRole) << treeState;
#endif
    changedIndices << sourceIndex;

    //update parent status
    QModelIndex currentParent = sourceIndex.parent();
    forever {
        if (!currentParent.isValid())
            break;

        CheckableProxyModel::NodeState parentNodeState = getTreeNodeState(currentParent);
        QPersistentModelIndex parentPIndex(currentParent);

        switch (parentNodeState.nodeState) {
        case CheckableProxyModel::DeterminedByChildren:
            break;
        case CheckableProxyModel::Checked:
        case CheckableProxyModel::Unchecked:
            parentNodeState.defaultChildState = parentNodeState.nodeState;
            parentNodeState.nodeState = CheckableProxyModel::DeterminedByChildren;
            m_checkStates.insert(parentPIndex, parentNodeState);
            break;
        case CheckableProxyModel::DeterminedByParent:
            Qt::CheckState currentCheckState = resolveCheckStateRole(currentParent);
            if (currentCheckState == Qt::Checked) {
                parentNodeState.defaultChildState = CheckableProxyModel::Checked;
            } else {
                parentNodeState.defaultChildState = CheckableProxyModel::Unchecked;
            }
            parentNodeState.nodeState = CheckableProxyModel::DeterminedByChildren;
            m_checkStates.insert(parentPIndex, parentNodeState);
        }
        changedIndices << currentParent;

#ifdef CHECKABLEPROXYMODEL_DEBUG
        qDebug() << "updated node" << currentParent << currentParent.data(Qt::DisplayRole) << parentNodeState.nodeState << parentNodeState.defaultChildState;
#endif

        currentParent = currentParent.parent();
    }

    //update child status
    removeSubtree(sourceIndex);

    foreach (const QModelIndex changedSourceIndex, changedIndices) {
        QModelIndex proxyIndex = mapFromSource(changedSourceIndex);
        emit dataChanged(proxyIndex, proxyIndex);
    }

#ifdef CHECKABLEPROXYMODEL_DEBUG
    qDebug() << "number of items in hash:" << m_checkStates.count();
#endif

    emit checkedNodesChanged();

    return true;
}

void CheckableProxyModel::removeSubtree(QModelIndex sourceIndex)
{
    //removes items from the data hash that are decendant of sourceIndex, but not sourceIndex itself
    int rowCount = sourceModel()->rowCount(sourceIndex);
    for (int i(0); i<rowCount; ++i) {
        QPersistentModelIndex pIndex(sourceIndex.child(i, 0));
        if (m_checkStates.contains(pIndex))
            removeSubtree(sourceIndex.child(i, 0));
        m_checkStates.remove(pIndex);
    }

    if (rowCount > 0)
        emit dataChanged(sourceIndex.child(0,0), sourceIndex.child(rowCount-1,0));
}

void CheckableProxyModel::cleanupStorage()
{
#ifdef CHECKABLEPROXYMODEL_DEBUG
    qDebug() << "Hash size before cleanup:" << m_checkStates.count();
#endif

    m_periodicalCleanupTimer->stop();
    QList<indexStatePair> itemsToReSet;

    QMutableHashIterator<QPersistentModelIndex, NodeState> it(m_checkStates);
    while (it.hasNext()) {
        it.next();
        if (!(it.key().isValid())) {
            it.remove();
            continue;
        }

        if (it.value().nodeState == CheckableProxyModel::DeterminedByChildren) {
            Qt::CheckState state = getCombinedChildrenCheckState(it.key());
            if (state != Qt::PartiallyChecked) {
                indexStatePair resetItem;
                resetItem.first = it.key();
                resetItem.second = state;
                itemsToReSet << resetItem;
            }
        }
    }

    //get rid of subitems by setting the state directly
    foreach(indexStatePair resetItem, itemsToReSet) {
        setCheckState(resetItem.first, resetItem.second);
    }

#ifdef CHECKABLEPROXYMODEL_DEBUG
    qDebug() << "Hash size after cleanup:" << m_checkStates.count();
#endif
    m_periodicalCleanupTimer->start();
}

/**
  Sets the default state for the checkboxes in the view. Use true to check
  the checkboxes by default, and false to uncheck them.

  Calling this method results in a reset of the model and the connected views.

  The default value is false.
*/
void CheckableProxyModel::setDefaultCheckState(bool checked)
{
    if (checked) {
        if (m_baseState.nodeState == CheckableProxyModel::Checked)
            return;
        beginResetModel();
        m_baseState.nodeState = CheckableProxyModel::Checked;
        m_baseState.defaultChildState = CheckableProxyModel::Checked;
    } else {
        if (m_baseState.nodeState == CheckableProxyModel::Unchecked)
            return;

        beginResetModel();
        m_baseState.nodeState = CheckableProxyModel::Unchecked;
        m_baseState.defaultChildState = CheckableProxyModel::Unchecked;

    }
    m_checkStates.clear();
    emit checkedNodesChanged();
    endResetModel();
}

/**
  Returns the default state for the checkboxes in the view.
  */
bool CheckableProxyModel::defaultCheckStateIsChecked() const
{
    return m_baseState.nodeState == CheckableProxyModel::Checked;
}

/**
  Resets the model and returns to the default state for every checkbox.

  Note that calling this method results in a reset of the model and the connected views.
  */
void CheckableProxyModel::resetToDefault()
{
    beginResetModel();
    m_checkStates.clear();
    emit checkedNodesChanged();
    endResetModel();
}


/**
  Sets the check state of the indicated item in the model.

  Note that this function takes a model index from the source model, not from the proxy
  model. This is intended as a convenience function, to make it easy to, for instance,
  set the check state of certain paths that are represented by a QFileSystemModel.

  @see setCheckedState
  */
bool CheckableProxyModel::setSourceIndexCheckedState(QModelIndex sourceIndex, bool checked)
{
    if (sourceIndex.model() != sourceModel())
        return false;

    return setCheckState(sourceIndex, checked ? Qt::Checked : Qt::Unchecked);
}

/**
  Sets the check state of the indicated item in the model.

  @see setSourceIndexCheckedState
  */
bool CheckableProxyModel::setCheckedState(QModelIndex proxyIndex, bool checked)
{
    if (proxyIndex.model() != this)
        return false;

    return setCheckState(mapToSource(proxyIndex), checked ? Qt::Checked : Qt::Unchecked);
}

/**
  \brief Method to access the check state of items in the model

  This method can be used to retreive the check state of items in the model. The following sample
  shows how this works:

  \code
    QModelIndexList selectedFiles;
    QModelIndexList selectedDirectories;
    QModelIndexList unselectedFiles;
    QModelIndexList unselectedDirectories;

    m_checkProxy->checkedState()
            .checkedLeafSourceModelIndexes(selectedFiles)
            .checkedBranchSourceModelIndexes(selectedDirectories)
            .uncheckedLeafSourceModelIndexes(unselectedFiles)
            .uncheckedBranchSourceModelIndexes(unselectedDirectories);
  \endcode

  Retreiving the source model indexes is more efficient than retreiving the proxy model indexes.

  Technically, checkedState() returns an CheckableProxyModelState object. This object in turn has
  methods that take a reference to a QModelIndexList in which the results are copied, and return a
  reference to the class again. This way, you can chain calls to the result object. Retreiving all
  the required lists in one go is much more efficient than making separate calls like this:

  \code
    QModelIndexList selectedFiles;
    QModelIndexList selectedDirectories;
    QModelIndexList unselectedFiles;
    QModelIndexList unselectedDirectories;

    //WRONG! This is very inefficient!
    m_checkProxy->checkedState()
            .checkedLeafIndexes(selectedFiles);
    m_checkProxy->checkedState()
            .checkedBranchIndexes(selectedDirectories);
    m_checkProxy->checkedState()
            .uncheckedLeafIndexes(unselectedFiles);
    m_checkProxy->checkedState()
            .uncheckedBranchIndexes(unselectedDirectories);
  \endcode

  */
CheckableProxyModelState CheckableProxyModel::checkedState()
{
    cleanupStorage();

    QSet<QModelIndex> checkedBNodes;
    QSet<QModelIndex> checkedLNodes;
    QSet<QModelIndex> uncheckedBNodes;
    QSet<QModelIndex> uncheckedLNodes;

    QQueue<QModelIndex> scanQueue;

    //first step: add all root nodes to the scan queue:
    int rootNodeCount = sourceModel()->rowCount(QModelIndex());
    for (int i(0); i < rootNodeCount; ++i) {
        QModelIndex node = sourceModel()->index(i, 0, QModelIndex());
        scanQueue.enqueue(node);
    }

    foreach(const QPersistentModelIndex pmIndex, m_checkStates.keys()) {
        scanQueue << pmIndex;
    }

    //now scan the enqueued items:
    while (!scanQueue.isEmpty()) {
        QModelIndex node = scanQueue.dequeue();
        Qt::CheckState state = resolveCheckStateRole(node);

        if (state == Qt::Checked) {
            if (sourceModel()->hasChildren(node)) {
                checkedBNodes << node;
            } else {
                checkedLNodes << node;
            }
        } else if (state == Qt::Unchecked) {
            if (sourceModel()->hasChildren(node)) {
                uncheckedBNodes << node;
            } else {
                uncheckedLNodes << node;
            }
        } else if (state == Qt::PartiallyChecked) {
            int childCount = sourceModel()->rowCount(node);
            for (int i(0); i < childCount; ++i) {
                QModelIndex childNode = sourceModel()->index(i, 0, node);
                scanQueue.enqueue(childNode);
            }
        }
    }

    CheckableProxyModelState result(this);

    result.m_checkedBranchNodes = checkedBNodes.toList();
    result.m_checkedLeafNodes = checkedLNodes.toList();
    result.m_uncheckedBranchNodes = uncheckedBNodes.toList();
    result.m_uncheckedLeafNodes = uncheckedLNodes.toList();

    return result;
}

/****************************************************************************************/

/**
  This function can be used to retreive a QModelIndexList filled with indexes of the items
  that correspond to the checked branch items.

  @see checkedBranchIndexes
  */
CheckableProxyModelState& CheckableProxyModelState::checkedBranchIndexes(QModelIndexList& output)
{
    output.clear();
    foreach(const QModelIndex index, m_checkedBranchNodes) {
        output << m_proxy->mapFromSource(index);
    }
    return *this;
}


/**
  This function can be used to retreive a QModelIndexList filled with indexes of the items
  that correspond to the checked leaf items.

  @see checkedLeafIndexes
  */
CheckableProxyModelState& CheckableProxyModelState::checkedLeafIndexes(QModelIndexList& output)
{
    output.clear();
    foreach(const QModelIndex index, m_checkedLeafNodes) {
        output << m_proxy->mapFromSource(index);
    }
    return *this;
}


/**
  This function can be used to retreive a QModelIndexList filled with indexes of the items
  that correspond to the unchecked branch items.

  @see uncheckedBranchIndexes
  */
CheckableProxyModelState& CheckableProxyModelState::uncheckedBranchIndexes(QModelIndexList& output)
{
    output.clear();
    foreach(const QModelIndex index, m_uncheckedBranchNodes) {
        output << m_proxy->mapFromSource(index);
    }
    return *this;
}

/**
  This function can be used to retreive a QModelIndexList filled with indexes of the items
  in the source model that correspond to the unchecked leaf items.

  @see uncheckedLeafIndexes
  */
CheckableProxyModelState& CheckableProxyModelState::uncheckedLeafIndexes(QModelIndexList& output)
{
    output.clear();
    foreach(const QModelIndex index, m_uncheckedLeafNodes) {
        output << m_proxy->mapFromSource(index);
    }
    return *this;
}


/**
  \fn CheckableProxyModelState& CheckableProxyModelState::checkedBranchSourceModelIndexes(QModelIndexList& output)
  Returns a QModelIndexList containing the indexes of the branch items in the source model
  that have been checked. Branch items are items that have children.

  Only the top item of a checked subtree will be returned. That is: items returned by this
  function also have all of their desending items, both leaf and other branch items, checked.

  @see checkedLeafSourceModelIndexes checkedBranchIndexes
  */

/**
  \fn CheckableProxyModelState& CheckableProxyModelState::checkedLeafSourceModelIndexes(QModelIndexList& output)
  Returns a QModelIndexList containing the indexes of the leaf items in the source model
  that have been checked. Leaf items are items that do not have children.

  Only leaf items that are children of a branch item that is partially checked will be returned.
  Otherwise, the checked status of the corresponding leaf item is already implicit in the
  checked status of the branch item.

  @see checkedBranchSourceModelIndexes checkedLeafIndexes
  */

/**
  \fn CheckableProxyModelState& CheckableProxyModelState::uncheckedBranchSourceModelIndexes(QModelIndexList& output)
  Returns a QModelIndexList containing the indexes of the branch items in the source model
  that have been unchecked. Branch items are items that have children.

  Only the top item of an unchecked subtree will be returned. That is: items returned by this
  function also have all of their desending items, both leaf and other branch items, unchecked.

  @see checkedLeafSourceModelIndexes checkedBranchIndexes
  */

/**
  \fn CheckableProxyModelState& CheckableProxyModelState::uncheckedLeafSourceModelIndexes(QModelIndexList& output)
  Returns a QModelIndexList containing the indexes of the leaf items in the source model
  that have been unchecked. Leaf items are items that do not have children.

  Only leaf items that are children of a branch item that is partially checked will be returned.
  Otherwise, the unchecked status of the corresponding leaf item is already implicit in the
  unchecked status of the branch item.

  @see checkedBranchSourceModelIndexes checkedLeafIndexes
  */
