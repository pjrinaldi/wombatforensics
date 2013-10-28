// Proxy model for doing groupBy
class GroupByModel : public QAbstractProxyModel
{
    Q_OBJECT

private:
    int groupBy;

    QList<QString> groups;
    QList<QModelIndex> groupIndexes;
    QMap<QString, QVector<int>*> groupToSourceRow;
    QVector<int> sourceRowToGroupRow;

public:

    GroupByModel(QObject *parent = NULL) : QAbstractProxyModel(parent), groupBy(-1) {
        setParent(parent);
    }
    ~GroupByModel() {}

    void setSourceModel(QAbstractItemModel *model) {
        QAbstractProxyModel::setSourceModel(model);
        setGroupBy(groupBy);
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const {
        if (parent.isValid()) {
            return createIndex(row,column, (void*)&groupIndexes[parent.row()]);
        } else {
            if (column == 0)
                return groupIndexes[row];
            else {
                return createIndex(row,column,NULL);
            }
        }
    }

    QModelIndex parent(const QModelIndex &index) const {
        // parent should be encoded in the index if we supplied it, if
        // we didn't then return a duffer
        if (index == QModelIndex() || index.internalPointer() == NULL) {
            return QModelIndex();
        } else if (index.column()) {
            return QModelIndex();
        }  else {
            return *static_cast<QModelIndex*>(index.internalPointer());
        }
    }

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const {

        if (proxyIndex.internalPointer() != NULL) {

            int groupNo = ((QModelIndex*)proxyIndex.internalPointer())->row();
            if (groupNo < 0 || groupNo >= groups.count() || proxyIndex.column() == 0) {
                return QModelIndex();
            }

            return sourceModel()->index(groupToSourceRow.value(groups[groupNo])->at(proxyIndex.row()),
                                        proxyIndex.column()-1, // accomodate virtual column
                                        QModelIndex());
        } 
        return QModelIndex();
    }

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const {

        // which group did we put this row into?
        QString group = whichGroup(sourceIndex.row());
        int groupNo = groups.indexOf(group);

        if (groupNo < 0) {
            return QModelIndex();
        } else {
            QModelIndex *p = new QModelIndex(createIndex(groupNo, 0, NULL));
            return createIndex(sourceRowToGroupRow[sourceIndex.row()], sourceIndex.column()+1, &p); // accomodate virtual column
        } 
    }

    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const {

        if (!proxyIndex.isValid()) return QVariant();

        QVariant returning;

        // if we are not at column 0 or we have a parent
        //if (proxyIndex.internalPointer() != NULL || proxyIndex.column() > 0) {
        if (proxyIndex.column() > 0) {

            returning = sourceModel()->data(mapToSource(proxyIndex), role);

        } else if (proxyIndex.internalPointer() == NULL) {

            // its our group by!
            if (proxyIndex.row() < groups.count()) {

                // blank values become "(blank)"
                QString group = groups[proxyIndex.row()];
                if (group == "") group = QString("(blank)");

                // format the group by with ride count etc
                if (groupBy != -1) {
                    QString returnString = QString("%1: %2 (%3 rides)")
                                           .arg(sourceModel()->headerData(groupBy, Qt::Horizontal).toString())
                                           .arg(group)
                                           .arg(groupToSourceRow.value(groups[proxyIndex.row()])->count());
                    returning = QVariant(returnString);
                } else {
                    QString returnString = QString("All %1 rides")
                                           .arg(groupToSourceRow.value(groups[proxyIndex.row()])->count());
                    returning = QVariant(returnString);
                }
            }
        }

        return returning;

    }

    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const {
        if (section)
            return sourceModel()->headerData(section-1, orientation, role);
        else
            return QVariant("*");
    }

    bool setHeaderData (int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole) {
        if (section)
            return sourceModel()->setHeaderData(section-1, orientation, value, role);
        else
            return true;
    }

    int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const {
        return sourceModel()->columnCount(QModelIndex())+1; // accomodate virtual group column
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        if (parent == QModelIndex()) {

            // top level return count of groups
            return groups.count();

        } else if (parent.column() == 0 && parent.internalPointer() == NULL) {

            // second level return count of rows for group
            return groupToSourceRow.value(groups[parent.row()])->count();

        } else {

            // no children any lower
            return 0;
        }
    }

    // does this index have children?
    bool hasChildren(const QModelIndex &index) const {

        if (index == QModelIndex()) {

            // at top
            return (groups.count() > 0);

        } else if (index.column() == 0 && index.internalPointer() == NULL) {

            // first column - the group bys
            return (groupToSourceRow.value(groups[index.row()])->count() > 0);

        } else {

            return false;

        }
    }

    //
    // GroupBy features
    //
    void setGroupBy(int column) {

        // shift down
        if (column >= 0) column -= 1;

        groupBy = column; // accomodate virtual column
        setGroups();
    }

    QString whichGroup(int row) const {

        if (groupBy == -1) return tr("All Rides");
        else return groupFromValue(headerData(groupBy+1, Qt::Horizontal).toString(),
                                    sourceModel()->data(sourceModel()->index(row,groupBy)).toString());

    }

    // implemented in RideNavigator.cpp, to avoid developers
    // from working out how this QAbstractProxy works, or
    // perhaps breaking it by accident ;-)
    QString groupFromValue(QString, QString) const;

    void clearGroups() {
        // Wipe current
        QMapIterator<QString, QVector<int>*> i(groupToSourceRow);
        while (i.hasNext()) {
            i.next();
            delete i.value();
        }
        groups.clear();
        groupIndexes.clear();
        groupToSourceRow.clear();
        sourceRowToGroupRow.clear();
    }

    int groupCount() {
        return groups.count();
    }

    void setGroups() {

        // let the views know we're doing this
        beginResetModel();

        // wipe whatever is there first
        clearGroups();

        if (groupBy >= 0) {

            // create a QMap from 'group' string to list of rows in that group
            for (int i=0; i<sourceModel()->rowCount(QModelIndex()); i++) {
                QString value = whichGroup(i);

                QVector<int> *rows;
                if ((rows=groupToSourceRow.value(value,NULL)) == NULL) {
                    // add to list of groups
                    rows = new QVector<int>;
                    groupToSourceRow.insert(value, rows);
                }

                // rowmap is an array corresponding to each row in the
                // source model, and maps to its row # within the group
                sourceRowToGroupRow.append(rows->count());

                // add to this groups rows
                rows->append(i);
            }

        } else {

            // Just one group by 'All Rides'
            QVector<int> *rows = new QVector<int>;
            for (int i=0; i<sourceModel()->rowCount(QModelIndex()); i++) {
                rows->append(i);
                sourceRowToGroupRow.append(i);
            }
            groupToSourceRow.insert("All Rides", rows);

        }

        // Update list of groups
        int group=0;
        QMapIterator<QString, QVector<int>*> j(groupToSourceRow);
        while (j.hasNext()) {
            j.next();
            groups << j.key();
            groupIndexes << createIndex(group++,0,NULL);
        }

        // all done. let the views know everything changed
        endResetModel();
    }
};
