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


#include <QStyledItemDelegate>



class Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    const int CHECK_ROLE = Qt::CheckStateRole;

        // dont't let the default QStyledItemDelegate create the true/false combobox
        QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;


        QRect GetCheckboxRect(const QStyleOptionViewItem &option)const;


        // click event
        bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
};


class IE_IB_listView: public QListView
{
    Q_OBJECT
public:
    explicit IE_IB_listView();
    void setDataModel(QAbstractItemModel * model);
    QList<QAction*> getActionList() const;
    QPushButton *getPushButtonGoBack() const;

signals:

public slots:
private:
    QAbstractItemModel * m_Model;
    QModelIndex m_currentIndex;
    QList<QAction*> m_actionList;
    QPushButton * m_pushButtonGoBack;
    void changeCurrentParent(const QModelIndex &index);



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

public slots:
private:
    QAbstractItemModel * m_model;
    IE_IB_listView * m_pListView;
};

#endif // IE_IB_WIDGET_H
