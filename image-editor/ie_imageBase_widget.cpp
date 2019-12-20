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
#include <QLabel>

// ------------- IE_IB_widget
        IE_IB_widget::IE_IB_widget(QWidget *parent) : QWidget(parent)
{
m_pListView = nullptr;
}

int     IE_IB_widget::init()
{
    m_pListView = new IE_IB_listView();
    QVBoxLayout * pvbl = new QVBoxLayout(this);

    pvbl->addWidget(m_pListView);

    QDialogButtonBox * pButtonBox = new QDialogButtonBox();
    pButtonBox->setParent(this);

    pButtonBox->addButton(m_pListView->m_pushButton_goBack,
                          QDialogButtonBox::ButtonRole::NoRole);
    pButtonBox->addButton(m_pListView->m_pushButton_showSelected,
                          QDialogButtonBox::ButtonRole::NoRole);
    pButtonBox->addButton(m_pListView->m_pushButton_makeCorrelation,
                          QDialogButtonBox::ButtonRole::NoRole);
    connect(m_pListView->m_pushButton_makeCorrelation, &QPushButton::clicked,
            this, &IE_IB_widget::pushButtonClicked_corellation);
    pvbl->addWidget(pButtonBox);

    return 0;
}

void    IE_IB_widget::setDataModel(QAbstractItemModel *model)
{
    m_model = model;
    m_pListView->setDataModel(m_model);
}


// ------------- IE_IB_listView

IE_IB_listView::IE_IB_listView(): QListView()
{
    m_isSelectedItemsViewActive = false;
    m_pProxyModel = nullptr;

    QAbstractItemDelegate * delegate = new Delegate();
    connect(this, &IE_IB_listView::doubleClicked,this, &IE_IB_listView::changeRootIndex);
    setItemDelegate(delegate);

    m_pushButton_makeCorrelation = new QPushButton("Соспоставить", this);
    m_pushButton_makeCorrelation->setToolTip("Соспоставление выбранных изображений из базы и интсрументов-маркеров на активном поле зрения. \nПеред выгрузкой отчета cопоставление автоматически не выполняется.");

    m_pushButton_showSelected = new QPushButton("Выбранные", this);
    m_pushButton_showSelected->setToolTip("Отображается список выбранных изображений.");
    QObject::connect(m_pushButton_showSelected, &QPushButton::clicked,
                     this, &IE_IB_listView::showSelectedItems);

    m_pushButton_goBack = new QPushButton("Назад", this);
    connect(m_pushButton_goBack, &QPushButton::clicked, [this]()
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
//        m_pushButton_goBack->setEnabled(true);
//    else
//        m_pushButton_goBack->setEnabled(false);

}

QList<QAction *> IE_IB_listView::getActionList() const
{
    return m_actionList;
}

void IE_IB_listView::resizeEvent(QResizeEvent *e)
{
    if(model() != Q_NULLPTR){
        model()->layoutChanged();
    }
    QListView::resizeEvent(e);
}

void IE_IB_listView::showSelectedItems()
{
    m_isSelectedItemsViewActive = !m_isSelectedItemsViewActive;
    if(m_isSelectedItemsViewActive)
    {
        if(!m_pProxyModel)
            m_pProxyModel = new SortProxy(this);
        m_pProxyModel->setSourceModel(m_Model);
        setModel(m_pProxyModel);
        emit m_pProxyModel->hideEverythingButA1AndChildren();
        m_pushButton_showSelected->setText("Все изображения");
        m_pushButton_goBack->setDisabled(true);
    }
    else
    {
        setModel(m_Model);
        m_pushButton_showSelected->setText("Выбранные");
//        changeRootIndex( m_Model->index(0,0) );
    }
}

void IE_IB_listView::changeRootIndex(const QModelIndex &index)
{
//    if(!index.isValid())
//        return;
//    if(index.model()->hasChildren(index))
        setRootIndex(index);
//    if(rootIndex().parent().isValid())
//        m_pushButton_goBack->setEnabled(false);
//    else
//        m_pushButton_goBack->setEnabled(true);
}


// ------------- Delegate

QWidget*Delegate::createEditor(QWidget *parent,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index
                               ) const
{
    (void)parent;
    (void)option;
    (void)index;
    return nullptr;
}

QRect   Delegate::GetCheckboxRect(const QStyleOptionViewItem &option) const
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

QRect   Delegate::getImageSize(const QStyleOptionViewItem &option) const
{

}

bool    Delegate::editorEvent(QEvent *event,
                              QAbstractItemModel *model,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index
                              )
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

void    Delegate::paint(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index
                        ) const
{
    painter->save();


    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    QStyleOptionButton checkBoxOpt;
    checkBoxOpt.rect = GetCheckboxRect(opt);

    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);

    painter->fillRect(rect, opt.state & QStyle::State_Selected ?
                              palette.highlight().color() :
                              palette.light().color());

    QVariant variantResult = index.data( CHECK_ROLE );


    if( !variantResult.isNull() )
    {
        bool isChecked = index.data( CHECK_ROLE ).toBool();
        if (isChecked)
            checkBoxOpt.state |= QStyle::State_On;
        else
            checkBoxOpt.state |= QStyle::State_Off;
        checkBoxOpt.state |= QStyle::State_Enabled;
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxOpt, painter);

    }


    QRect rec(option.rect);
    rec.setX(checkBoxOpt.rect.right());





    QModelIndex ind = index;

    QString imagePath = index.data(100).toString();
    if(!imagePath.isEmpty())
    {
        QImage img(index.data( 100 ).toString());
        QImage img2( img.scaledToWidth(rec.width()) );
        rec.setHeight( img2.height() );
        painter->drawImage( rec, img2 );
    }
    painter->drawText( rec, Qt::TextWordWrap, ind.data().toString() );
    painter->restore();
}

QSize   Delegate::sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index
                           ) const
{
    QStyleOptionViewItem options = option;
        initStyleOption(&options, index);
    int scrollBarrWidth = options.widget->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    QRect rowRect(0,0,(options.widget->rect().right()-scrollBarrWidth),options.widget->rect().bottom());


    QString imagePath = index.data(100).toString();
    if(!imagePath.isEmpty())
    {
        QImage img(index.data( 100 ).toString());
        QImage img2( img.scaledToWidth(  rowRect.width() - 27 ) );
        return img2.size();
    }
    else
    {
        rowRect.setHeight(QStyledItemDelegate::sizeHint(option, index).height());
        return rowRect.size();
    }
//        return QStyledItemDelegate::sizeHint(option, index);

}


// ------------- SortProxy

            SortProxy::SortProxy(QObject *parent) : QAbstractProxyModel(parent),
                                                    hideThem(false)
{
    fixModel();
}

int         SortProxy::rowCount(const QModelIndex &parent) const
{
    return mapping.count();
//    QModelIndex sourceParent;
//    if (parent.isValid())
//        sourceParent = mapToSource(parent);
//    int count = 0;
//    QMapIterator<QPersistentModelIndex, QPersistentModelIndex> it(proxySourceParent);
//    while (it.hasNext()) {
//        it.next();
//        if (it.value() == sourceParent)
//            count++;
//    }
//    return count;
}

int         SortProxy::columnCount(const QModelIndex &) const
{
    return 1;
}

QModelIndex SortProxy::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex SortProxy::parent(const QModelIndex &child) const
{
    QModelIndex mi = proxySourceParent.value(child);
    if (mi.isValid())
        return mapFromSource(mi);
    return QModelIndex();
}

QModelIndex SortProxy::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QModelIndex();
    return mapping.key(proxyIndex);
}

QModelIndex SortProxy::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid())
        return QModelIndex();
    return mapping.value(sourceIndex);
}

void        SortProxy::hideEverythingButA1AndChildren()
{
    hideThem = !hideThem;
    // Now we set up the proxy <-> source mappings
    emit layoutAboutToBeChanged();
    fixModel();
    emit layoutChanged();
}

void        SortProxy::fixModel()
{
    if(!sourceModel())
        return;
    m_list.clear();
    //! поиск всех элементов исходной модели данных
    fillListFromTreeModel(sourceModel()->index(0,0).parent());
    mapping.clear();
    proxySourceParent.clear();

    for (int i=0;i<m_list.size();i++)
    {
        QPersistentModelIndex pmi = m_list.at(i);
        if (!pmi.data(Qt::CheckStateRole).toBool())
            continue;

        QModelIndex proxy = createIndex(mapping.count(), 0, pmi.internalPointer());

        mapping.insert(pmi, proxy);
        QModelIndex sourceParent;

        if (pmi.parent().parent().isValid())
            sourceParent = pmi.parent();
        proxySourceParent.insert(proxy, sourceParent);



    }
}

void        SortProxy::fillListFromTreeModel(QModelIndex ind)
{
    int numOfRows = sourceModel()->rowCount(ind);
    if(!numOfRows)
        return;
    for(int i=0; i<numOfRows; i++)
    {
        if( sourceModel()->rowCount( sourceModel()->index(i,0, ind) ) )
            fillListFromTreeModel( sourceModel()->index(i,0, ind) );
        else
            m_list << QPersistentModelIndex(sourceModel()->index(i,0, ind));

    }

}





