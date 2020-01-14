#include "ie_imageBase_treeItem.h"

IE_IB_treeItem::IE_IB_treeItem(const QVector<QVariant> &data, IE_IB_treeItem *parent)
    : m_itemData(data), m_parentItem(parent)
{}

IE_IB_treeItem::~IE_IB_treeItem()
{
    qDeleteAll(m_childItems);
}

void IE_IB_treeItem::appendChild(IE_IB_treeItem *item)
{
    m_childItems.append(item);
}


IE_IB_treeItem *IE_IB_treeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}


int IE_IB_treeItem::childCount() const
{
    return m_childItems.count();
}


int IE_IB_treeItem::columnCount() const
{
//    return 1;
    return m_itemData.count();
}


QVariant IE_IB_treeItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}


IE_IB_treeItem *IE_IB_treeItem::parentItem()
{
    return m_parentItem;
}

bool IE_IB_treeItem::setData(int column, const QVariant &data)
{
    if (column < 0 || column >= m_itemData.size())
        return false;
    m_itemData.replace(column, data);
    return true;
}


int IE_IB_treeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<IE_IB_treeItem*>(this));

    return 0;
}
