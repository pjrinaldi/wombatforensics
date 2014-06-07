/*

Copyright (C) 2011 by Mike McQuaid

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "tabletotreeproxymodel.h"

#include <QItemSelection>
#include <QSqlQueryModel>
#include <QDebug>
#include <QStringList>

class TreeNode {
public:
    TreeNode(int row=-1, int column=-1, TreeNode *parent=0)
        : row(row), column(column), parent(parent)
    {
        if (parent)
            parent->children.append(this);
    };
    ~TreeNode()
    {
        qDeleteAll(children);
    }
    int row;
    int column;
    TreeNode *parent;
    QList<TreeNode*> children;
    int objid;
    int addid;
    int parid;
    int typid;
    //QString text;
};

TableToTreeProxyModel::TableToTreeProxyModel(int rootaddress, QObject *parent)
    : QAbstractProxyModel(parent), rootaddress(rootaddress), rootNode(0)
{
}

TableToTreeProxyModel::~TableToTreeProxyModel()
{
    foreach(QList<TreeNode*> rowNodes, tableNodes)
        rowNodes.clear();
    tableNodes.clear();
    delete rootNode;
}

QModelIndex TableToTreeProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    //TreeNode *node = tableNodes.at(sourceIndex.column()).at(sourceIndex.row());
    TreeNode* node = tableNodes.at(0).at(sourceIndex.row());
    qDebug() << "tablenodes-id: " << node->objid;
    Q_ASSERT(node);
    if (!node)
        return QModelIndex();

    //Q_ASSERT(false);
    return index(sourceIndex.row(), 0);
    //return index(sourceIndex.row(), sourceIndex.column());
}

QModelIndex TableToTreeProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QModelIndex();

    const TreeNode *node = static_cast<TreeNode*>(proxyIndex.internalPointer());
    Q_ASSERT(node);
    if (!node)
        return QModelIndex();

    //return sourceModel()->index(node->row, node->column);
    return QModelIndex();
}

QVariant TableToTreeProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (!proxyIndex.isValid())
        return QVariant();

    const TreeNode *node = static_cast<TreeNode*>(proxyIndex.internalPointer());
    Q_ASSERT(node);
    if (!node)
        return QVariant();

    const QModelIndex index = sourceModel()->index(node->row, node->column);
    return sourceModel()->data(index, role);
}

QModelIndex TableToTreeProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    const TreeNode *parentNode = static_cast<TreeNode*>(parent.internalPointer());
    if (!parentNode)
        parentNode = rootNode;

    TreeNode *node = parentNode->children.at(row);
    Q_ASSERT(node);
    if (!node)
        return QModelIndex();

    return createIndex(row, column, node);
}

QModelIndex TableToTreeProxyModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    const TreeNode *childNode = static_cast<TreeNode*>(child.internalPointer());
    Q_ASSERT(childNode);
    if (!childNode)
        return QModelIndex();

    TreeNode *parentNode = childNode->parent;
    Q_ASSERT(parentNode);
    if (!parentNode)
        return QModelIndex();
    if (parentNode == rootNode)
        return QModelIndex();

    return createIndex(parentNode->row, parentNode->column, parentNode);
}

int TableToTreeProxyModel::rowCount(const QModelIndex &parent) const
{
    const TreeNode *node = static_cast<TreeNode*>(parent.internalPointer());
    if (!node)
        node = rootNode;

    return node->children.size();
}

int TableToTreeProxyModel::columnCount(const QModelIndex &) const
{
    //return sourceModel()->columnCount();
    return 1;
}

bool TableToTreeProxyModel::hasChildren(const QModelIndex &parent) const
{
    const TreeNode *node = static_cast<TreeNode*>(parent.internalPointer());
    if (!node)
        node = rootNode;

    return node->children.size() > 0;
}

void TableToTreeProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    if (this->sourceModel()) {
        disconnect(this->sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                   this, SLOT(reset()));

        disconnect(this->sourceModel(), SIGNAL(headerDataChanged(Qt::Orientation,int,int)),
                   this, SLOT(reset()));

        //    disconnect(this->sourceModel(), SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),
        //               this, SLOT(reset()));

        disconnect(this->sourceModel(), SIGNAL(rowsInserted(QModelIndex,int,int)),
                   this, SLOT(reset()));

        //    disconnect(this->sourceModel(), SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)),
        //               this, SLOT(reset()));

        disconnect(this->sourceModel(), SIGNAL(columnsInserted(QModelIndex,int,int)),
                   this, SLOT(reset()));

        //    disconnect(this->sourceModel(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
        //               this, SLOT(reset()));

        disconnect(this->sourceModel(), SIGNAL(rowsRemoved(QModelIndex,int,int)),
                   this, SLOT(reset()));

        //    disconnect(this->sourceModel(), SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)),
        //               this, SLOT(reset()));

        disconnect(this->sourceModel(), SIGNAL(columnsRemoved(QModelIndex,int,int)),
                   this, SLOT(reset()));

        //    disconnect(this->sourceModel(), SIGNAL(layoutAboutToBeChanged()),
        //               this, SLOT(reset()));

        disconnect(this->sourceModel(), SIGNAL(layoutChanged()),
                   this, SLOT(reset()));

        //    disconnect(this->sourceModel(), SIGNAL(modelAboutToBeReset()), this, SLOT(_q_sourceAboutToBeReset()));
        //    disconnect(this->sourceModel(), SIGNAL(modelReset()), this, SLOT(_q_sourceReset()));
    }

    QAbstractProxyModel::setSourceModel(sourceModel);
    reset();

    connect(this->sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(reset()));

    connect(this->sourceModel(), SIGNAL(headerDataChanged(Qt::Orientation,int,int)),
            this, SLOT(reset()));

//    connect(this->sourceModel(), SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),
//            this, SLOT(reset()));

    connect(this->sourceModel(), SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(reset()));

//    connect(this->sourceModel(), SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)),
//            this, SLOT(reset()));

    connect(this->sourceModel(), SIGNAL(columnsInserted(QModelIndex,int,int)),
            this, SLOT(reset()));

//    connect(this->sourceModel(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
//            this, SLOT(reset()));

    connect(this->sourceModel(), SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(reset()));

//    connect(this->sourceModel(), SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)),
//            this, SLOT(reset()));

    connect(this->sourceModel(), SIGNAL(columnsRemoved(QModelIndex,int,int)),
            this, SLOT(reset()));

//    connect(this->sourceModel(), SIGNAL(layoutAboutToBeChanged()),
//            this, SLOT(reset()));

    connect(this->sourceModel(), SIGNAL(layoutChanged()),
            this, SLOT(reset()));

//    connect(this->sourceModel(), SIGNAL(modelAboutToBeReset()), this, SLOT(_q_sourceAboutToBeReset()));
//    connect(this->sourceModel(), SIGNAL(modelReset()), this, SLOT(_q_sourceReset()));
}

void TableToTreeProxyModel::reset()
{
    beginResetModel();

    foreach(QList<TreeNode*> rowNodes, tableNodes)
        rowNodes.clear();
    tableNodes.clear();
    delete rootNode;

    //QAbstractProxyModel::reset();

    rootNode = new TreeNode;
    QList<TreeNode*> rowNodes;
    TreeNode *previousNode = rootNode;

    for (int row=0; row < sourceModel()->rowCount(); ++row) { // for each row...
        //qDebug() << sourceModel()->rowCount();
        TreeNode* imagenode;
        int col = 0;
        //for(int col = 0; col < sourceModel()->columnCount(); ++col)
        //{
            //qDebug() << "id|address|parent|type" << sourceModel()->data(sourceModel()->index(row, 0)).toString() << sourceModel()->data(sourceModel()->index(row, 5)).toString() << sourceModel()->data(sourceModel()->index(row, 11)).toString() << sourceModel()->data(sourceModel()->index(row, 4)).toString();
            int objid = sourceModel()->data(sourceModel()->index(row, 0)).toInt(); // objectid
            int addid = sourceModel()->data(sourceModel()->index(row, 5)).toInt(); // address
            int parid = sourceModel()->data(sourceModel()->index(row, 11)).toInt(); // parentid
            int typid = sourceModel()->data(sourceModel()->index(row, 4)).toInt(); // objecttype
            if(typid < 5)
            {
                switch(typid)
                {
                    qDebug() << "typid: " << typid;
                    // image node
                    case 1:
                        imagenode = new TreeNode(row, col, previousNode);
                        qDebug() << "root address: " << rootaddress;
                        imagenode->addid = rootaddress;
                        rowNodes.insert(0, imagenode);
                        previousNode = imagenode;
                        break;
                    // volume node
                    case 2:
                        //node = new TreeNode(row, column, previousNode);
                        break;
                    // partition node
                    case 3:
    
                        break;
                    // filesystem node
                    case 4:
                        break;
                }
            }
            else
            {
                TreeNode* tmpnode = 0;
                foreach(TreeNode* siblingnode, previousNode->children)
                {
                    if(!siblingnode)
                        continue;
                    if(siblingnode->addid == parid)
                    {
                        //qDebug() << "siblingnode->addid" << siblingnode->addid << " == parid " << parid;
                        tmpnode = siblingnode;
                        break;
                    }
                }
                if(!tmpnode)
                {
                    tmpnode = new TreeNode(row, col, previousNode);
                    tmpnode->objid = objid;
                    tmpnode->addid = addid;
                    tmpnode->parid = parid;
                    tmpnode->typid = typid;
                }
                rowNodes.insert(col, tmpnode);
                previousNode = tmpnode;
            }
        //}
        /*
        for (int column=0; column < sourceModel()->columnCount(); ++column) { // for each column...
            const QString &nodeText = sourceModel()->data(sourceModel()->index(row, column)).toString();
            TreeNode *node = 0;
            foreach (TreeNode *siblingNode, previousNode->children) {
                Q_ASSERT(siblingNode);
                if (!siblingNode)
                    continue;
                if (siblingNode->text == nodeText) {
                    node = siblingNode;
                    break;
                }
            }

            if (!node) {
                node = new TreeNode(row, column, previousNode);
                node->text = nodeText;
            }
            rowNodes.insert(column, node);
            previousNode = node;
        }
        */
        tableNodes.insert(row, rowNodes);
    }

    endResetModel();
}
