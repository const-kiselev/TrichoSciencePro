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
#include <QLabel>


#include <QStyledItemDelegate>

class IE_IB_listView;

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

        friend class IE_IB_listView;
private:
        QMap<QString, QPixmap*> *m_pSmallImages;
};



//! \brief класс прокси-фильтра для отображения выбранных изображений
class SortProxy : public QAbstractProxyModel
{
    Q_OBJECT

public:
                SortProxy(QObject *parent = 0);
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

public slots:
        void hideEverythingButA1AndChildren();

private:
    void fixModel();
    void fillListFromTreeModel(QModelIndex ind);
    QList<QPersistentModelIndex> m_list;

    QMap<QPersistentModelIndex, QPersistentModelIndex> mapping;
    QMap<QPersistentModelIndex, QPersistentModelIndex> proxySourceParent;
    bool hideThem;
};

class IE_IB_listView: public QListView
{
    Q_OBJECT
public:
    explicit        IE_IB_listView();
    void            setDataModel(QAbstractItemModel * model);
    QList<QAction*> getActionList() const;

    void            resizeEvent(QResizeEvent *e);

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
    QList<QModelIndex> indexList;
    SortProxy * m_pProxyModel;

    QMap<QString, QPixmap*> m_smallImages;

    void changeCurrentParent(const QModelIndex &index);
    void loadImages(const QModelIndex &index);

private slots:
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
    void pushButtonClicked_corellation();

public slots:
private:
    QAbstractItemModel * m_model;
    IE_IB_listView * m_pListView;
};

#endif // IE_IB_WIDGET_H
