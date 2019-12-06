#ifndef IE_IB_TREEITEM_H
#define IE_IB_TREEITEM_H

#include <QVariant>
#include <QVector>


class IE_IB_treeItem
{
public:
    explicit IE_IB_treeItem(const QVector<QVariant> &data, IE_IB_treeItem *parentItem = nullptr);
    ~IE_IB_treeItem();

        void appendChild(IE_IB_treeItem *child);

        IE_IB_treeItem *child(int row);
        int childCount() const;
        int columnCount() const;
        QVariant data(int column) const;
        int row() const;
        IE_IB_treeItem *parentItem();

        bool setData(int column, const QVariant &data);

    private:
        QVector<IE_IB_treeItem*> m_childItems;
        QVector<QVariant> m_itemData;
        IE_IB_treeItem *m_parentItem;
};

#endif // IE_IB_TREEITEM_H
