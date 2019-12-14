#ifndef IE_IB_WIDGET_H
#define IE_IB_WIDGET_H

#include <QListView>
#include <QWidget>
#include <QAbstractItemModel>
#include <QCheckBox>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPushButton>
#include <QDockWidget>
#include <QComboBox>
#include <QStandardItem>
#include <QAbstractProxyModel>


#include <QStyledItemDelegate>

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    const int CHECK_ROLE = Qt::CheckStateRole;

        QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;


        QRect GetCheckboxRect(const QStyleOptionViewItem &option)const;
        QRect getImageSize(const QStyleOptionViewItem &option)const;


        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override
        {

             editor->setGeometry(option.widget->rect());
        }

        // click event
        bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
};




class SortProxy : public QAbstractProxyModel
{
    Q_OBJECT

public:
    SortProxy(QObject *parent = 0) : QAbstractProxyModel(parent), hideThem(false)
    {
        fixModel();
    }

    int rowCount(const QModelIndex &parent) const
    {
        QModelIndex sourceParent;
        if (parent.isValid())
            sourceParent = mapToSource(parent);
        int count = 0;
        QMapIterator<QPersistentModelIndex, QPersistentModelIndex> it(proxySourceParent);
        while (it.hasNext()) {
            it.next();
            if (it.value() == sourceParent)
                count++;
        }
        return count;
    }

    int columnCount(const QModelIndex &) const
    {
        return 1;
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const
    {
        QModelIndex sourceParent;
        if (parent.isValid())
            sourceParent = mapToSource(parent);
        QMapIterator<QPersistentModelIndex, QPersistentModelIndex> it(proxySourceParent);
        while (it.hasNext()) {
            it.next();
            if (it.value() == sourceParent && it.key().row() == row &&
                it.key().column() == column)
                return it.key();
        }
        return QModelIndex();
    }

    QModelIndex parent(const QModelIndex &child) const
    {
        QModelIndex mi = proxySourceParent.value(child);
        if (mi.isValid())
            return mapFromSource(mi);
        return QModelIndex();
    }

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const
    {
        if (!proxyIndex.isValid())
            return QModelIndex();
        return mapping.key(proxyIndex);
    }

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const
    {
        if (!sourceIndex.isValid())
            return QModelIndex();
        return mapping.value(sourceIndex);
    }

public slots:
        void hideEverythingButA1AndChildren()
        {
            hideThem = !hideThem;
            // Now we set up the proxy <-> source mappings
            emit layoutAboutToBeChanged();
            fixModel();
            emit layoutChanged();
        }

private:
    void fixModel();
    void fillListFromTreeModel(QModelIndex ind);
    QList<QStandardItem*> m_list;

    QMap<QPersistentModelIndex, QPersistentModelIndex> mapping;
    QMap<QPersistentModelIndex, QPersistentModelIndex> proxySourceParent;
    bool hideThem;
};







class IE_IB_listView: public QListView
{
    Q_OBJECT
public:
    explicit IE_IB_listView();
    void setDataModel(QAbstractItemModel * model);
    QList<QAction*> getActionList() const;

    void resizeEvent(QResizeEvent *e)
    {
        if(model() != Q_NULLPTR){
            model()->layoutChanged();
        }
        QListView::resizeEvent(e);
    }

    friend class IE_IB_widget;

signals:

public slots:
    void showSelectedItems();
private:
    QAbstractItemModel * m_Model;
    QModelIndex m_currentIndex;
    QList<QAction*> m_actionList;
    QPushButton * m_pushButton_goBack,
                * m_pushButton_showSelected,
                * m_pushButton_makeCorrelation;
    bool m_isSelectedItemsViewActive;
    SortProxy * m_pProxyModel;
    void changeCurrentParent(const QModelIndex &index);

private slots:
    QList<QStandardItem *> list;
    void changeRootIndex(const QModelIndex &index);
};



class IE_IB_widget : public QWidget
{
    Q_OBJECT
public:
    explicit IE_IB_widget(QWidget *parent = nullptr);
    int init();
    void setDataModel(QAbstractItemModel * model);


signals:

public slots:
private:
    QAbstractItemModel * m_model;
    IE_IB_listView * m_pListView;
};

#endif // IE_IB_WIDGET_H
