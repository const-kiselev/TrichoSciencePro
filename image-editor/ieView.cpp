#include "ieView.h"


// Задачи:
// - реализовать блокировку редактирования, пока не создана сцена-модель

TSPImageEditorView::TSPImageEditorView()
{
    pModel = nullptr;
    init();
}

TSPImageEditorView::TSPImageEditorView(TSPImageEditorModel *pInputModel):pModel(pInputModel)
{
    init();
    sceneChanged();

    //resize(pModel->sceneRect().width(), pModel->sceneRect().height());
    //setFixedSize(pModel->sceneRect().width(), pModel->sceneRect().height());
}

void TSPImageEditorView::init()
{
    pDockInfo = nullptr;
    currentScale = 1.0;
    currentViewedModelLeftTopPoint = QPoint(0,0);

    setRenderHint (QPainter::Antialiasing, true ) ;
    centerOn(0,0);
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pToolsController = new ToolsController();
    // Связывание м/у сигнала "начало использование нового инструмента" и слотом "добавление нового элемента в модель ч/з контроллер инструментов"
    connect(pToolsController, &ToolsController::zoomSignal,
            this, &TSPImageEditorView::zoomSlot);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connect(pToolsController, &ToolsController::needMeasureIndex, [this](){
        this->pToolsController->setMeasureIndexInActiveTool(this->pModel->getMeasureIndex());
    });
}

// ------- EVENTS

void TSPImageEditorView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    emit(changedSizeOfView(size().width(), size().height()));
}
void TSPImageEditorView::mousePressEvent(QMouseEvent *pe)
{
    // Проверка на инструмент, у которого необходимо отслеживать выход за границу изображения
   if(checkTheMousePosViaImageBorder(pe))
       return;
   QApplication::sendEvent(pToolsController, pe);

}
void TSPImageEditorView::mouseReleaseEvent(QMouseEvent *pe)
{
    // Проверка на инструмент, у которого необходимо отслеживать выход за границу изображения
    if(checkTheMousePosViaImageBorder(pe))
        return;
    QApplication::sendEvent(pToolsController, pe);
}
void TSPImageEditorView::mouseMoveEvent(QMouseEvent *pe)
{
    // Проверка на инструмент, у которого необходимо отслеживать выход за границу изображения
    if(checkTheMousePosViaImageBorder(pe))
        return;
    QApplication::sendEvent(pToolsController, pe);
    emit(mousePos(pe->pos()));
}
void TSPImageEditorView::wheelEvent(QWheelEvent *pe)
{
    QApplication::sendEvent(pToolsController, pe);
}

QDockWidget *TSPImageEditorView::getPDockDebugInfo() const
{
    return pDockDebugInfo;
}
QDockWidget *TSPImageEditorView::getPDockInfo() const
{
    return pDockInfo;
}

// ------- END EVENTS -------
// -------
// -------
// ------- DockWidgets

QDockWidget * TSPImageEditorView::initDockDebugWidget()
{


    static QLabel  *pImageOfModelSize, *pZoomScale, *pMeasureIndex,
            *pCurrentSizeOfViewWidget, *pLabelMousePos, *pLabelMousePosMapToScene;

    pDockDebugInfo =  new QDockWidget("Debug Info");
    pDockDebugInfo->setAllowedAreas(Qt::RightDockWidgetArea);
    pDockDebugInfo->setFeatures(QDockWidget::DockWidgetClosable);

    pdockTableWidget = new QTableWidget();

    pImageOfModelSize = new QLabel(pdockTableWidget);

    pImageOfModelSize->setText(QString().number(pModel->width())+"x"+QString().number(pModel->height()));
    connect(pModel, &TSPImageEditorModel::changedModelSize, [=](qreal fx, qreal fy){
        pImageOfModelSize->setText(QString::number(fx)+" x "+QString::number(fy));
    });

    pMeasureIndex = new QLabel(pdockTableWidget);
    pMeasureIndex->setText(QString().number(pModel->getMeasureIndex()));
    connect(pModel,&TSPImageEditorModel::measureIndexChanged, [=](qreal measureIndex){
        pMeasureIndex->setText(QString::number(measureIndex));
    });

    pZoomScale = new QLabel(pdockTableWidget);
    pZoomScale->setText(QString().number(currentScale));
    connect(this, &TSPImageEditorView::changedScale, [=](qreal currentScale){
        pZoomScale->setText(QString::number(currentScale));
    });

    pCurrentSizeOfViewWidget = new QLabel(pdockTableWidget);
    connect(this, &TSPImageEditorView::changedSizeOfView, [=](int ix, int iy){
        pCurrentSizeOfViewWidget->setText(QString::number(ix)+" x "+QString::number(iy));
    });

    pLabelMousePos = new QLabel(pdockTableWidget);
    pLabelMousePos->setText("0 x 0");
    connect(this, &TSPImageEditorView::mousePos, [=](QPointF point){
        pLabelMousePos->setText(QString().number(point.x())+" x "+QString().number(point.y()));
    });

    pLabelMousePosMapToScene = new QLabel(pdockTableWidget);
    pLabelMousePosMapToScene->setText("0 x 0");
    connect(this, &TSPImageEditorView::mousePos, [=](QPointF point){
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

void TSPImageEditorView::initInfoDock()
{

}

// ------- END DockWidgets -------
// -------
// -------
// ------- Tool's methods

void TSPImageEditorView::zoomSlot(float delta, QPointF point)
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


void TSPImageEditorView::sceneChanged()
{
    setScene(pModel);
    setSceneRect(0, 0, pModel->sceneRect().width(), pModel->sceneRect().height());
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void TSPImageEditorView::resize(int w, int h)
{
    QGraphicsView::resize(w,h);
    //fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}


    // Проверка на инструмент, у которого необходимо отслеживать выход за границу изображения
int TSPImageEditorView::checkTheMousePosViaImageBorder(QMouseEvent *pe)
{ 
    pe->setLocalPos(computeSceneRelativelyPosition(pe->pos()));
    if(pToolsController->getActiveToolType() >= ToolType::checkableTool)
    {
        QPointF checkMousePressPos = static_cast<QMouseEvent*>(pe)->pos();
        if(checkMousePressPos.x() < 0)
            return 1;
        if(checkMousePressPos.y() < 0)
            return 1;
        if(checkMousePressPos.x() >= pModel->getImageRect().width())
            return 1;
        if(checkMousePressPos.y() >= pModel->getImageRect().height())
            return 1;
    }
    return 0;
}
    // уточнение координат относительно координаты ieView и ieModel
QPointF TSPImageEditorView::computeSceneRelativelyPosition(const QPointF point)
{
    return mapToScene(point.x(), point.y());
}


// ------- GETTERS and SETTERS

ToolsController *TSPImageEditorView::getPToolsController() const
{
    return pToolsController;
}
TSPImageEditorModel *TSPImageEditorView::getPImgModel() const
{
    return pModel;
}
void TSPImageEditorView::setPImgModel(TSPImageEditorModel *value)
{
    pModel = value;
}
QString TSPImageEditorView::getStatusBarInfoDataForUser() const
{

}
TSPImageEditorModel *TSPImageEditorView::getPModel() const
{
    return pModel;
}
void TSPImageEditorView::setPModel(TSPImageEditorModel *value)
{
    pModel = value;
}


// !!!!!! ------- TODO:
