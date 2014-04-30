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
#ifndef CHECKABLEPROXYMODEL_H
#define CHECKABLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QHash>
#include <QStack>

//disable to get rid of the debug output and checks
//#define CHECKABLEPROXYMODEL_DEBUG
//note that the default debug output uses Qt::UserRole + 1. When using a QFileSystemModel, this is the full path


class DelayedExecutionTimer;
class QTimer;
class CheckableProxyModelState;

class CheckableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CheckableProxyModel(QObject *parent = 0);

    virtual void setSourceModel(QAbstractItemModel *sourceModel);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    bool defaultCheckStateIsChecked() const;

    CheckableProxyModelState checkedState();

    bool setSourceIndexCheckedState(QModelIndex sourceIndex, bool checked);
    bool setCheckedState(QModelIndex proxyIndex, bool checked);

signals:
    void checkedNodesChanged();

public slots:
    void setDefaultCheckState(bool checked);
    void resetToDefault();

private:
    enum TreeCheckState {
        DeterminedByParent, DeterminedByChildren, Checked, Unchecked
    };
    struct NodeState
    {
        TreeCheckState nodeState;
        TreeCheckState defaultChildState;

        NodeState():
            nodeState(DeterminedByParent),
            defaultChildState(DeterminedByParent)
        {
        }
        NodeState(TreeCheckState stateForNode):
            nodeState(stateForNode),
            defaultChildState(DeterminedByParent)
        {
        }
        NodeState(TreeCheckState stateForNode, TreeCheckState stateForChildren):
            nodeState(stateForNode),
            defaultChildState(stateForChildren)
        {
        }
    };

    typedef QPair<QPersistentModelIndex, Qt::CheckState> indexStatePair;

private slots:
    void cleanupStorage();

private:
    Qt::CheckState resolveCheckStateRole(QModelIndex index) const;
    Qt::CheckState getCombinedChildrenCheckState(QModelIndex index) const;
    NodeState getTreeNodeState(QModelIndex index) const;
    bool setCheckState(QModelIndex sourceIndex, Qt::CheckState state);
    void removeSubtree(QModelIndex sourceIndex);
    void rebuildCheckedNodes() ;

private:
    QHash<QPersistentModelIndex, NodeState> m_checkStates;
    DelayedExecutionTimer* m_cleanupTimer;
    QTimer* m_periodicalCleanupTimer;
    NodeState m_baseState;

#ifdef CHECKABLEPROXYMODEL_DEBUG
    mutable QStack<QModelIndex> m_indexStack;
#endif
};


class CheckableProxyModelState
{
public:
    CheckableProxyModelState& checkedBranchIndexes(QModelIndexList& list);
    CheckableProxyModelState& checkedLeafIndexes(QModelIndexList& list);
    CheckableProxyModelState& checkedBranchSourceModelIndexes(QModelIndexList& list);
    CheckableProxyModelState& checkedLeafSourceModelIndexes(QModelIndexList& list);
    CheckableProxyModelState& uncheckedBranchIndexes(QModelIndexList& list);
    CheckableProxyModelState& uncheckedLeafIndexes(QModelIndexList& list);
    CheckableProxyModelState& uncheckedBranchSourceModelIndexes(QModelIndexList& list);
    CheckableProxyModelState& uncheckedLeafSourceModelIndexes(QModelIndexList& list);

private:
    //private constructors: this class is not to be copied around or anything like that.
    CheckableProxyModelState(CheckableProxyModel* proxy): m_proxy(proxy) {}
    //declaration only, we don't need implementation
    CheckableProxyModelState(const CheckableProxyModelState& other);
    CheckableProxyModelState& operator=(CheckableProxyModelState const& other);
    bool operator==(CheckableProxyModelState const& other);

    QModelIndexList m_checkedBranchNodes;
    QModelIndexList m_checkedLeafNodes;
    QModelIndexList m_uncheckedBranchNodes;
    QModelIndexList m_uncheckedLeafNodes;

    CheckableProxyModel* m_proxy;

    friend class CheckableProxyModel;
};

//inline implemenation of the basic accessor methods
inline CheckableProxyModelState& CheckableProxyModelState::checkedBranchSourceModelIndexes(QModelIndexList &list)
{
    list = m_checkedBranchNodes;
    return *this;
}

inline CheckableProxyModelState& CheckableProxyModelState::checkedLeafSourceModelIndexes(QModelIndexList &list)
{
    list = m_checkedLeafNodes;
    return *this;
}

inline CheckableProxyModelState& CheckableProxyModelState::uncheckedBranchSourceModelIndexes(QModelIndexList &list)
{
    list = m_uncheckedBranchNodes;
    return *this;
}

inline CheckableProxyModelState& CheckableProxyModelState::uncheckedLeafSourceModelIndexes(QModelIndexList &list)
{
    list = m_uncheckedLeafNodes;
    return *this;
}

#endif // CHECKABLEPROXYMODEL_H
