#include "ie_view.h"

IE_View::IE_View(IE_Model *pInputModel):pModel(pInputModel)
{
    init();
    sceneChanged();
    setBackgroundBrush(Qt::NoBrush);
//    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);




    //resize(pModel->sceneRect().width(), pModel->sceneRect().height());
    //setFixedSize(pModel->sceneRect().width(), pModel->sceneRect().height());
}

IE_View::~IE_View()
{
    if(pModel)
        delete pModel;
    pModel = nullptr;
//    delete pToolsController;
    pToolsController = nullptr;
}

void IE_View::init()
{
    if(!pModel)
    {
        qWarning() << "pModel is nullptr.";
        return;
    }
    _p_ie_global_data = pModel->getPGlobal_data();

    pDockInfo = nullptr;
    currentScale = 1.0;
    currentViewedModelLeftTopPoint = QPoint(0,0);

    setRenderHint (QPainter::Antialiasing, true ) ;
    centerOn(0,0);
//    setAlignment(Qt::AlignTop|Qt::AlignLeft);
    setAlignment(Qt::AlignCenter);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    pToolsController = new ToolsController(p_ie_global_data());
    //! Связывание м/у сигнала "начало использование нового инструмента" и слотом "добавление нового элемента в модель ч/з контроллер инструментов"
    connect(pToolsController, &ToolsController::zoomSignal,
            this, &IE_View::zoomSlot);


    connect(pModel, &IE_Model::boundingRectWasChanged, this, &IE_View::changeCurrentViewRect);


    pModel->setPToolCnt(pToolsController);

    connect(pToolsController, &ToolsController::startUsingNewTool,
            pModel, &IE_Model::addLayerViaToolCnt);


}

// ------- EVENTS

void    IE_View::resizeEvent           (QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    emit(changedSizeOfView(size().width(), size().height()));

}
void    IE_View::mousePressEvent       (QMouseEvent *pe)
{
    // Проверка на инструмент, у которого необходимо отслеживать выход за границу изображения
   if(checkTheMousePosViaImageBorder(pe))
       return;
   QApplication::sendEvent(pToolsController, pe);

}
void    IE_View::mouseReleaseEvent     (QMouseEvent *pe)
{
    // Проверка на инструмент, у которого необходимо отслеживать выход за границу изображения
    if(checkTheMousePosViaImageBorder(pe))
        return;
    QApplication::sendEvent(pToolsController, pe);
}
void    IE_View::mouseMoveEvent        (QMouseEvent *pe)
{
    // Проверка на инструмент, у которого необходимо отслеживать выход за границу изображения
    if(checkTheMousePosViaImageBorder(pe))
        return;
    QApplication::sendEvent(pToolsController, pe);
    emit(mousePos(pe->pos()));
}
void    IE_View::wheelEvent            (QWheelEvent *pe)
{
    QApplication::sendEvent(pToolsController, pe);
}

void    IE_View::drawBackground(QPainter *painter, const QRectF &rect)
{

}

void    IE_View::drawForeground(QPainter *painter, const QRectF &rect)
{
    //! \todo в данном методе необходимо реализовать скртие других ПЗ. Проблема возникает с синхронизацией масштабов. Необходимо при разработке сделать тестовый виджет с данными для отладки.
}


_global_ie *
        IE_View::p_ie_global_data() const
{
    return _p_ie_global_data;
}


QDockWidget *
        IE_View::getPDockDebugInfo() const
{
    return pDockDebugInfo;
}
QDockWidget *
        IE_View::getPDockInfo() const
{
    return pDockInfo;
}

// ------- END EVENTS -------
// -------
// -------
// ------- DockWidgets

QDockWidget *
        IE_View::initDockDebugWidget()
{


    QLabel  *pImageOfModelSize, *pZoomScale, *pMeasureIndex,
            *pCurrentSizeOfViewWidget, *pLabelMousePos, *pLabelMousePosMapToScene;

    pDockDebugInfo =  new QDockWidget("Debug Info");
    pDockDebugInfo->setAllowedAreas(Qt::RightDockWidgetArea);
    pDockDebugInfo->setFeatures(QDockWidget::DockWidgetClosable);

    pdockTableWidget = new QTableWidget();

    pImageOfModelSize = new QLabel(pdockTableWidget);

    pImageOfModelSize->setText(QString().number(pModel->width())+"x"+QString().number(pModel->height()));
    connect(pModel, &IE_Model::changedModelSize, [=](qreal fx, qreal fy){
        pImageOfModelSize->setText(QString::number(fx)+" x "+QString::number(fy));
    });

    pMeasureIndex = new QLabel(pdockTableWidget);
    pMeasureIndex->setText(QString().number(pModel->getMeasureIndex()));
    connect(pModel,&IE_Model::measureIndexChanged, [=](qreal measureIndex){
        pMeasureIndex->setText(QString::number(measureIndex));
    });

    pZoomScale = new QLabel(pdockTableWidget);
    pZoomScale->setText(QString().number(currentScale));
    connect(this, &IE_View::changedScale, [=](qreal currentScale){
        pZoomScale->setText(QString::number(currentScale));
    });

    pCurrentSizeOfViewWidget = new QLabel(pdockTableWidget);
    connect(this, &IE_View::changedSizeOfView, [=](int ix, int iy){
        pCurrentSizeOfViewWidget->setText(QString::number(ix)+" x "+QString::number(iy));
    });

    pLabelMousePos = new QLabel(pdockTableWidget);
    pLabelMousePos->setText("0 x 0");
    connect(this, &IE_View::mousePos, [=](QPointF point){
        pLabelMousePos->setText(QString().number(point.x())+" x "+QString().number(point.y()));
    });

    pLabelMousePosMapToScene = new QLabel(pdockTableWidget);
    pLabelMousePosMapToScene->setText("0 x 0");
    connect(this, &IE_View::mousePos, [=](QPointF point){
        point = computeSceneRelativelyPosition(point);
        pLabelMousePosMapToScene->setText(QString().number(point.x())+" x "+QString().number(point.y()));
    });

    pDockDebugInfo->setWidget(pdockTableWidget);

    pdockTableWidget->setRowCount(6);
    pdockTableWidget->setColumnCount(2);

    QStringList       lst;
    lst << "Name" << "Data";
    pdockTableWidget->setHorizontalHeaderLabels(lst);

    pdockTableWidget->setItem(0,0,new QTableWidgetItem(QString("pImageOfModelSize")));
    pdockTableWidget->setCellWidget(0,1, pImageOfModelSize);
    pdockTableWidget->setItem(1,0,new QTableWidgetItem("pZoomScale"));
    pdockTableWidget->setCellWidget(1,1, pZoomScale);
    pdockTableWidget->setItem(2,0,new QTableWidgetItem("pCurrentSizeOfViewWidget"));
    pdockTableWidget->setCellWidget(2,1, pCurrentSizeOfViewWidget);
    pdockTableWidget->setItem(3,0,new QTableWidgetItem("pLabelMousePos"));
    pdockTableWidget->setCellWidget(3,1, pLabelMousePos);
    pdockTableWidget->setItem(4,0,new QTableWidgetItem("pLabelMousePosMapToScene"));
    pdockTableWidget->setCellWidget(4,1, pLabelMousePosMapToScene);
    pdockTableWidget->setItem(5,0,new QTableWidgetItem("pMeasureIndex"));
    pdockTableWidget->setCellWidget(5,1, pMeasureIndex);
    return pDockDebugInfo;
}

void    IE_View::setToolCntType(ToolSet ts)
{
    pToolsController->setToolSetType(ts);
}

void    IE_View::initInfoDock()
{

}

// ------- END DockWidgets -------
// -------
// -------
// ------- Tool's methods

void    IE_View::zoomSlot(float delta, QPointF point)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    static const double scaleFactor = 1.15;
    static const double scaleMin = .1; // defines the min scale limit.


    if(delta > 0) {
            scale(scaleFactor, scaleFactor);
            currentScale *= scaleFactor;
        } else if (currentScale > scaleMin) {
            scale(1 / scaleFactor, 1 / scaleFactor);
            currentScale /= scaleFactor;
        }


    emit(changedScale(currentScale));
}


// ------- END Tool's methods -------


void    IE_View::sceneChanged()
{
    setScene(pModel);
    setSceneRect(pModel->getModelRect());
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void    IE_View::changeCurrentViewRect(QRectF rectOfView)
{
    setSceneRect(rectOfView);
//    setFixedSize(rectOfView.width(), rectOfView.height());
}

void    IE_View::resize(int w, int h)
{
    QGraphicsView::resize(w,h);
    //fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}


    // Проверка на инструмент, у которого необходимо отслеживать выход за границу изображения
int     IE_View::checkTheMousePosViaImageBorder(QMouseEvent *pe)
{ 
    pe->setLocalPos(computeSceneRelativelyPosition(pe->pos()));
    if(pToolsController->getActiveToolType() >= ToolType::checkableTool)
    {
//        QPointF checkMousePressPos = ;
        QRectF checkMousePressRect;
        checkMousePressRect.setTopLeft(static_cast<QMouseEvent*>(pe)->pos());
        checkMousePressRect.setWidth(0.1);
        checkMousePressRect.setHeight(0.1);
        if(!pModel->getModelRect().intersects(checkMousePressRect))
            return 1;
//        if(checkMousePressPos.x() < 0)
//            return 1;
//        if(checkMousePressPos.y() < 0)
//            return 1;
//        if(checkMousePressPos.x() >= pModel->getModelRect().width())
//            return 1;
//        if(checkMousePressPos.y() >= pModel->getModelRect().height())
//            return 1;
    }
    return 0;
}
    // уточнение координат относительно координаты ieView и ieModel
QPointF IE_View::computeSceneRelativelyPosition(const QPointF point)
{
    return mapToScene(point.x(), point.y());
}


// ------- GETTERS and SETTERS

ToolsController *IE_View::getPToolsController() const
{
    return pToolsController;
}


QString IE_View::getStatusBarInfoDataForUser() const
{

}
IE_Model *IE_View::getPModel() const
{
    return pModel;
}




