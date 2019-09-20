#ifndef IE_TOOLITEMDELEGATE_H
#define IE_TOOLITEMDELEGATE_H

#include <QStyledItemDelegate>

class QModelIndex;
class QPainter;
class QStyleOptionViewItem;

class IE_ToolItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit IE_ToolItemDelegate(QObject *parent=nullptr)
                : QStyledItemDelegate(parent) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
};

#endif // IE_TOOLITEMDELEGATE_H
