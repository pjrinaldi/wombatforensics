class TreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    TreeProxyModel(QObject* parent = 0) : QSortFilterProxyModel(parent)
    {
    };
    // my vars/functions here
protected:
    bool filterAcceptsRow(int sourcerow, const QModelIndex& sourceparent) const Q_DECL_OVERRIDE
    {
    };
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const Q_DECL_OVERRIDE
    {
        QVariant leftdata = sourceModel()->data(left);
        QVariant rightdata = sourceModel()->data(right);
        // here is where i need to calculate my sort models if i sort...
        // SORT BY SIZE TEST
        if(leftdata
    };
private:
    // my vars/functions here.
};
