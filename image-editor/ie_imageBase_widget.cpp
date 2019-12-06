#include "ie_imageBase_widget.h"

#include <QCheckBox>
#include <QPainter>
#include <QVBoxLayout>
#include <QDebug>
#include <QSignalMapper>
#include <QDialogButtonBox>
#include <QDialog>
#include <QDialog>
#include <QPushButton>
#include <QAction>

IE_IB_widget::IE_IB_widget(QWidget *parent) : QWidget(parent)
{
m_pListView = nullptr;
}

int IE_IB_widget::init()
{
    m_pListView = new IE_IB_listView();


    QVBoxLayout * pvbLayout = new QVBoxLayout(this);
    m_pListView->setParent(this);
    pvbLayout->addWidget(m_pListView);

    QDialogButtonBox * pButtonBox = new QDialogButtonBox();
    pButtonBox->addButton(m_pListView->getPushButtonGoBack(), QDialogButtonBox::ButtonRole::NoRole);
    pButtonBox->setParent(m_pListView);
    pvbLayout->addWidget(pButtonBox);

    setLayout(pvbLayout);
    return 0;
}

void IE_IB_widget::setDataModel(QAbstractItemModel *model)
{
    m_model = model;
    m_pListView->setDataModel(m_model);
}

IE_IB_listView::IE_IB_listView(): QListView()
{
    QAbstractItemDelegate * delegate = new Delegate();
    connect(this, &IE_IB_listView::doubleClicked,this, &IE_IB_listView::changeRootIndex);
    setItemDelegate(delegate);

    m_pushButtonGoBack = new QPushButton("Назад", this);
    connect(m_pushButtonGoBack, &QPushButton::clicked, [this]()
    {
        QModelIndex currentRoot = rootIndex();
//        if(currentRoot.parent().isValid())
            changeRootIndex( currentRoot.parent() );
    });


}

void IE_IB_listView::setDataModel(QAbstractItemModel *model)
{
    m_Model = model;
    setModel(model);
//    if(rootIndex().parent().isValid())
//        m_pushButtonGoBack->setEnabled(false);
//    else
//        m_pushButtonGoBack->setEnabled(true);

}

QList<QAction *> IE_IB_listView::getActionList() const
{
    return m_actionList;
}

QPushButton *IE_IB_listView::getPushButtonGoBack() const
{
    return m_pushButtonGoBack;
}

void IE_IB_listView::changeRootIndex(const QModelIndex &index)
{
//    if(!index.isValid())
//        return;
//    if(index.model()->hasChildren(index))
        setRootIndex(index);
//    if(rootIndex().parent().isValid())
//        m_pushButtonGoBack->setEnabled(false);
//    else
//        m_pushButtonGoBack->setEnabled(true);
}

QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)parent;
    (void)option;
    (void)index;
    return nullptr;
}

QRect Delegate::GetCheckboxRect(const QStyleOptionViewItem &option) const
{
    QStyleOptionButton opt_button;
    opt_button.QStyleOption::operator=(option);
    QRect sz = QApplication::style()->subElementRect(QStyle::SE_ViewItemCheckIndicator, &opt_button);
    QRect r = option.rect;
    // center 'sz' within 'r'
    int dx = 10;
    int dy = (r.height()- sz.height())/2;
    r.setTopLeft(r.topLeft() + QPoint(dx,dy));
    r.setWidth(sz.width());
    r.setHeight(sz.height());

    return r;
}

bool Delegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{

    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* pME = static_cast<QMouseEvent*>(event);
        if(pME->button() == Qt::LeftButton)
        {
            QRect ro = GetCheckboxRect(option);
            QPoint pte = pME->pos();
            if(ro.contains(pte) )
            {
                bool value = index.data( CHECK_ROLE).toBool();
                return model->setData(index, !value, CHECK_ROLE);
            }
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();


    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    QStyleOptionButton cbOpt;
    cbOpt.rect = GetCheckboxRect(opt);

    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);

    painter->fillRect(rect, opt.state & QStyle::State_Selected ?
                              palette.highlight().color() :
                              palette.light().color());


    bool isChecked = index.data( CHECK_ROLE ).toBool();
    if (isChecked)
    {
        cbOpt.state |= QStyle::State_On;
    }
    else
    {
        cbOpt.state |= QStyle::State_Off;
    }

    QVariant enabled = index.data(Qt::ItemIsEnabled);
    if(enabled.isNull() || enabled.toBool() )
    {
        cbOpt.state |= QStyle::State_Enabled;
    }





    QRect rec(option.rect);
    rec.setX(cbOpt.rect.right());
    painter->setFont(opt.font);
    painter->setPen( Qt::black );



    QApplication::style()->drawControl(QStyle::CE_CheckBox, &cbOpt, painter);

    QModelIndex ind = index;

    painter->drawText(rec, Qt::TextWordWrap, ind.data().toString());
    QString imagePath = index.data(100).toString();
    if(!imagePath.isEmpty())
    {
        painter->drawImage(rec, QImage( index.data( 100 ).toString()) );
    }
    painter->restore();

}
