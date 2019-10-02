#include "toolController.h"


// TODO: сделать логику обнудения pActiveTool = nullptr; !!!!


ToolsController::ToolsController()
{
    setIconSize(QSize(20,20));
    setMovable(false);

    toolSetType = ToolSet::AllTools;
    //addAction(QPixmap(":/icon/imageEditor/colored/magic-wand"), "1tool");
    //addAction(QPixmap(":/icon/imageEditor/colored/scale"), "scale");
    //addAction(QPixmap(":/icon/imageEditor/colored/hand"), "Load img");
    //addAction(QPixmap(":/icon/imageEditor/colored/color"), "marker");
    pDock = nullptr;
    pActiveTool = nullptr;
    activeToolType = ToolType::NoneTool;
    doNextMousePressEvent = true;
    editModeActive = false;

    connect(this, &ToolsController::startUsingNewTool, &ToolsController::updateToolInfoDockWidget);
}


// ------- EVENT FILTER

// Логика: если активирован модуль редактирования, то проверяется совпадения
// активного интсрумента и редактируемого слоя, если по типам инструментов совпадают,
//    то редактирование возможно
void ToolsController::toolEventFilter(QInputEvent *pe)
{
    if(editModeActive)
    {
        if(!pActiveTool)
        {
            resetEditingMode();
            return;
        }
        if(activeToolType != pActiveTool->getToolType())
        {
            emit(stopUsingTool());
            pActiveTool->getToolPtr()->deactivateEditMode();
            doNextMousePressEvent = true;
            pActiveTool = nullptr;
            editModeActive = false;
        }
        else if(activeToolType == pActiveTool->getToolType())
        {
            pActiveTool->getToolPtr()->activateEditMode();
            updateToolInfoDockWidget();
        }
    }


    switch (activeToolType)
    {
    case ToolType::DensityAndDiameter:
    case ToolType::Ruler:
    case ToolType::SimpleLine:
    {
            switch (pe->type())
            {
            case QEvent::MouseButtonPress:
            {
                if(!editModeActive)
                {
                    switch (activeToolType) {
                    case ToolType::Ruler:
                        pActiveTool = new IE_ModelLayer(ToolType::Ruler, new IERuler());
                        break;
                        case ToolType::SimpleLine:
                        pActiveTool = new IE_ModelLayer(ToolType::SimpleLine, new IELine());
                        break;
                        case ToolType::DensityAndDiameter:
                        pActiveTool = new IE_ModelLayer(ToolType::DensityAndDiameter,
                                                        new IE_Line_DD());
                        break;

                    }

                    emit(startUsingNewTool());
//                    pDock->setWidget();
                }
                else {
                    pActiveTool->getToolPtr()->activateEditMode();
//                    dynamic_cast<IELine*>( pActiveTool->parentItem())->activateEditMode();
                }
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(pe);
                QPointF mousePos = mouseEvent->pos();
                if(mouseEvent->localPos()!=mouseEvent->pos())
                    mousePos = mouseEvent->localPos();

                // создаем сигнал о том, что началось использование нового инстурмента

                doNextMousePressEvent = false;
                pActiveTool->getToolPtr()->mouseFirstPress(mousePos);
//                dynamic_cast<IELine*>(pActiveTool->parentItem())->mouseFirstPress(mousePos);


            }
                break;
            case QEvent::MouseMove:
            {
                if(!pActiveTool)
                    return;

                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(pe);
                QPointF mousePos = mouseEvent->pos();
                if(mouseEvent->localPos()!=mouseEvent->pos())
                    mousePos = mouseEvent->localPos();
                if(pActiveTool->getToolPtr()->mouseMove(mousePos) ) //dynamic_cast<IELine*>(pActiveTool->parentItem())->mouseMove(mousePos)
                    resetEditingMode();
            }
                break;
            case QEvent::MouseButtonRelease:
            {
                doNextMousePressEvent = true;
                emit(stopUsingTool());
                if(!pActiveTool)
                    return;
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(pe);
                QPointF mousePos = mouseEvent->pos();
                if(mouseEvent->localPos()!=mouseEvent->pos())
                    mousePos = mouseEvent->localPos();
//                dynamic_cast<IELine*>(pActiveTool->parentItem())->release(mousePos);
                pActiveTool->getToolPtr()->release(mousePos);

            }
                break;
            case QEvent::Wheel:
            {
                if(pActiveTool)
                    pActiveTool->getToolPtr()->wheelMode(static_cast<QWheelEvent*>(pe));
//                    dynamic_cast<IELine*>(pActiveTool->parentItem())->wheelMode(static_cast<QWheelEvent*>(pe));
            }
                break;
            } // ----- END switch (pe->type())
            break;
    // ----- END case SimpleLine
    }
    case ToolType::Zoom:
    {
        switch (pe->type()) {
        case QEvent::Wheel:
            emit(zoomSignal(static_cast<QWheelEvent*>(pe)->delta(), static_cast<QMouseEvent*>(pe)->pos()));
            break;
        }
        break;
    }
    case ToolType::Marker_FollicularUnit:
    {
        switch (pe->type())
        {
            case QEvent::MouseButtonPress:
            {

                pActiveTool = new IE_ModelLayer(ToolType::Marker_FollicularUnit, new IE_Tool_FollicularUnit());
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(pe);
                QPointF mousePos = mouseEvent->pos();
                if(mouseEvent->localPos()!=mouseEvent->pos())
                    mousePos = mouseEvent->localPos();

                // создаем сигнал о том, что началось использование нового инстурмента
                emit(startUsingNewTool());
                doNextMousePressEvent = true;
                //static_cast<IE_ToolMarker*>(pActiveTool->parentItem())->mouseFirstPress(mousePos);
                pActiveTool->getToolPtr()->mouseFirstPress(mousePos);
            }
                break;
            case QEvent::Wheel:
            {
            if(!pActiveTool)
                return;

//                static_cast<IE_ToolMarker*>(pActiveTool->parentItem())->wheelMode(static_cast<QWheelEvent*>(pe));
                pActiveTool->getToolPtr()->wheelMode(static_cast<QWheelEvent*>(pe));
            }
                break;
        }
        break;
    }

    case ToolType::PeripilarSign_SpikyHair:
    case ToolType::PeripilarSign_ExclamationHair:
    case ToolType::PeripilarSign_BrokenHair:
    case ToolType::PeripilarSign_CadaverizedHair:
    case ToolType::PeripilarSign_YellowDot:
    case ToolType::PeripilarSign_RedDot:
    case ToolType::PeripilarSign_WhiteDot:
    case ToolType::PeripilarSign_Hyperpigmentation:
    case ToolType::RootType_Anagen:
    case ToolType::RootType_DysplasticAnagen:
    case ToolType::RootType_BrokenAnagen:
    case ToolType::RootType_AnagenWithPapilla:
    case ToolType::RootType_Telogen:
    case ToolType::RootType_Catagen:
    case ToolType::RootType_Dystrophic:
    {
        switch (pe->type())
        {
            case QEvent::MouseButtonPress:
            {

                pActiveTool = new IE_ModelLayer(activeToolType, new IE_Tool_Marker(nullptr, activeToolType));
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(pe);
                QPointF mousePos = mouseEvent->pos();
                if(mouseEvent->localPos()!=mouseEvent->pos())
                    mousePos = mouseEvent->localPos();

                // создаем сигнал о том, что началось использование нового инстурмента
                emit(startUsingNewTool());
                doNextMousePressEvent = true;
                //static_cast<IE_ToolMarker*>(pActiveTool->parentItem())->mouseFirstPress(mousePos);
                pActiveTool->getToolPtr()->mouseFirstPress(mousePos);
            }
                break;
        }
        break;
    }

        /*    case ToolType::Ruler:
    {
        switch (pe->type())
        {
        case QEvent::MouseButtonPress:
        {
            pActiveTool = new IE_ModelLayer(ToolType::Ruler, new IERuler());
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(pe);
            QPointF mousePos = mouseEvent->pos();
            if(mouseEvent->localPos()!=mouseEvent->pos()) // так как у контроллера нет доступа к View,
                            // то сопаставленные координаты напрямую получить нельзя,
                            // поэтому, перед тем как отправляется событие в контроллер,
                            // объекту собтия в локальную позицию записываются координаты относительно модели!!!!
                mousePos = mouseEvent->localPos();

            // создаем сигнал о том, что началось использование нового инстурмента
            emit(startUsingNewTool());// Это необходимо, чтобы в модель добавился соответсвующий объект-фигура
            doNextMousePressEvent = false;
//            static_cast<IERuler*>((QGraphicsItem*)pActiveTool)->mouseFirstPress(mousePos);
            pActiveTool->getToolPtr()->mouseFirstPress(mousePos);
        }
            break;
        case QEvent::MouseMove:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(pe);
            QPointF mousePos = mouseEvent->pos();
            if(mouseEvent->localPos()!=mouseEvent->pos())
                mousePos = mouseEvent->localPos();
            //static_cast<IERuler*>((QGraphicsItem*)pActiveTool)->mouseMove(mousePos);
            pActiveTool->getToolPtr()->mouseMove(mousePos);
        }
            break;
        case QEvent::MouseButtonRelease:
        {
            doNextMousePressEvent = true;
            emit(stopUsingTool());
        }
            break;
        } // ----- END switch (pe->type())
        break;// ----- END case SimpleLine
    }*/
    // ----- END switch (activeToolType)
    }

}

void ToolsController::updateToolInfoDockWidget()
{
    if(!pDock)
        return;

    if(!pActiveTool)
    {
        pDock->setWidget(pEmptyDockInfoWidget);
        return;
    }
    if(!(pActiveTool->getToolPtr()))
    {
        pDock->setWidget(pEmptyDockInfoWidget);
        return;
    }
    QWidget *pWidg = pActiveTool->getToolPtr()->getWidgetPtr();
    if(!pWidg)
    {
        pDock->setWidget(pEmptyDockInfoWidget);
        return;
    }


    pDock->setWidget(pWidg);
}

QDockWidget*  ToolsController::initInfoDock()
{
    pDock = new QDockWidget("Параметры инструмента");
    pEmptyDockInfoWidget = new QWidget();
    QVBoxLayout* pvbl = new QVBoxLayout(pEmptyDockInfoWidget);
    pvbl->setAlignment(Qt::AlignCenter);
    pvbl->addWidget(new QLabel("..."));
    pEmptyDockInfoWidget->setLayout(pvbl);
    pDock->setWidget(pEmptyDockInfoWidget);
    return pDock;
}


// ------- EVENTS

void ToolsController::mousePressEvent(QMouseEvent *pe)
{
    if(doNextMousePressEvent)
        toolEventFilter(pe);
}
void ToolsController::mouseReleaseEvent(QMouseEvent *pe)
{
    toolEventFilter(pe);
}
void ToolsController::mouseMoveEvent(QMouseEvent *pe)
{
    toolEventFilter(pe);
}
void ToolsController::wheelEvent(QWheelEvent *pe)
{
//    static QTime time = QTime::currentTime();
//    if(QTime::currentTime().msec() - time.msec()  > 10)
        toolEventFilter(pe);
//    time = QTime::currentTime();
}
void ToolsController::keyPressEvent(QKeyEvent *pe)
{
    toolEventFilter(pe);
}

void ToolsController::initToolActions()
{
    QActionGroup *pActionGroup = new QActionGroup(this);
    QList<ToolType> toolsList;
    switch (toolSetType){
        case ToolSet::AllTools:
            toolsList   << ToolType::NoneTool
                        << ToolType::SimpleLine
                        << ToolType::DensityAndDiameter
                        << ToolType::Zoom
                        << ToolType::Marker_FollicularUnit
                        << ToolType::Ruler

                        << ToolType::PeripilarSign_SpikyHair
                        << ToolType::PeripilarSign_ExclamationHair
                        << ToolType::PeripilarSign_BrokenHair
                        << ToolType::PeripilarSign_CadaverizedHair
                        << ToolType::PeripilarSign_YellowDot
                        << ToolType::PeripilarSign_RedDot
                        << ToolType::PeripilarSign_WhiteDot
                        << ToolType::PeripilarSign_Hyperpigmentation

                        << ToolType::RootType_Anagen
                        << ToolType::RootType_DysplasticAnagen
                        << ToolType::RootType_BrokenAnagen
                        << ToolType::RootType_AnagenWithPapilla
                        << ToolType::RootType_Telogen
                        << ToolType::RootType_Catagen
                        << ToolType::RootType_Dystrophic;

            break;
        case ToolSet::CallibrationToolSet:
            toolsList << ToolType::Ruler
                      << ToolType::Zoom
                      << ToolType::NoneTool;
            break;
    }

    QAction *pToolAction;


    for(QListIterator<ToolType> iterToolsList(toolsList); iterToolsList.hasNext();)
    {
        ToolType actionType = iterToolsList.next();
        switch (actionType) {
        case ToolType::NoneTool:
        {
            pToolAction = new QAction(QPixmap(":/icon/imageEditor/colored/arrow-1"),
                                      "Курсор");
            pToolAction->setCheckable(true);
            pToolAction->setChecked(true);
            break;
        }
        case ToolType::SimpleLine:
        {
            pToolAction = new QAction(QPixmap(":/icon/imageEditor/colored/resources/img/colored/brush 2.png"), "Линия");
            break;
        }
        case ToolType::DensityAndDiameter:
        {
            pToolAction = new QAction(QPixmap("://DensityAndDiameter"),"Плотность, диаметр и длина");
            break;
        }
        case ToolType::Zoom:
        {
            pToolAction = new QAction(QPixmap(":/icon/imageEditor/colored/zoom"), "Лупа");
            break;
        }
        case ToolType::Marker_FollicularUnit:
        {
            pToolAction = new QAction(QPixmap("://Marker_FollicularUnit"), "Маркер фолликулярных юнитов");
            break;
        }
        case ToolType::Ruler:
        {
            pToolAction = new QAction(QPixmap(":/icon/imageEditor/black/resources/img/black/ruler.png"), "Линейка");
            break;
        }

            case ToolType::PeripilarSign_SpikyHair:
        {
            pToolAction = new QAction(QPixmap("://PeripilarSign_SpikyHair"),"Остроконечный волос");
            break;
        }
            case ToolType::PeripilarSign_ExclamationHair:
        {
            pToolAction = new QAction(QPixmap("://PeripilarSign_ExclamationHair"),"Волос в виде восклицательного знака");
            break;
        }
            case ToolType::PeripilarSign_BrokenHair:
        {
            pToolAction = new QAction(QPixmap("://PeripilarSign_BrokenHair"),"Обломанный волос");
            break;
        }
            case ToolType::PeripilarSign_CadaverizedHair:
        {
            pToolAction = new QAction(QPixmap("://PeripilarSign_CadaverizedHair"),"Кадаверизированный волос");
            break;
        }
            case ToolType::PeripilarSign_YellowDot:
        {
            pToolAction = new QAction(QPixmap("://PeripilarSign_YellowDot"),"Желтая точка");
            break;
        }
            case ToolType::PeripilarSign_RedDot:
        {
            pToolAction = new QAction(QPixmap("://PeripilarSign_RedDot"),"Красная точка");
            break;
        }
            case ToolType::PeripilarSign_WhiteDot:
        {
            pToolAction = new QAction(QPixmap("://PeripilarSign_WhiteDot"),"Белая точка");
            break;
        }
        case ToolType::PeripilarSign_Hyperpigmentation:
        {
            pToolAction = new QAction(QPixmap("://PeripilarSign_Hyperpigmentation"),"Гиперпигментация");
            break;
        }

            case ToolType::RootType_Anagen:
        {
            pToolAction = new QAction(QPixmap("://RootType_Anagen"), "Анагеновый");
            break;
        }
            case ToolType::RootType_DysplasticAnagen:
        {
            pToolAction = new QAction(QPixmap("://RootType_DysplasticAnagen"), "Диспластичный анагеновый");
            break;
        }
            case ToolType::RootType_BrokenAnagen:
        {
            pToolAction = new QAction(QPixmap("://RootType_BrokenAnagen"), "Обломанный анагенновый");
            break;
        }
            case ToolType::RootType_AnagenWithPapilla:
        {
            pToolAction = new QAction(QPixmap("://RootType_AnagenWithPapilla"), "Анагеновый с папиллой");
            break;
        }
            case ToolType::RootType_Telogen:
        {
            pToolAction = new QAction(QPixmap("://RootType_Telogen"), "Телогеновый");
            break;
        }
            case ToolType::RootType_Catagen:
        {
            pToolAction = new QAction(QPixmap("://RootType_Catagen"), "Катагеновый");
            break;
        }
            case ToolType::RootType_Dystrophic:
        {
            pToolAction = new QAction(QPixmap("://RootType_Dystrophic"), "Дистрофичный");
            break;
        }



        }
        connect(pToolAction, &QAction::triggered, [=]() {
            this->changeTool(actionType, pToolAction);
        });
        pToolAction->setCheckable(true);
        pActionGroup->addAction(pToolAction);
    }
    addActions(pActionGroup->actions());

}

void ToolsController::setMeasureIndexInActiveTool(qreal input)
{

}

void ToolsController::resetEditingMode()
{
    doNextMousePressEvent = true;
    if(pActiveTool)
        pActiveTool->getToolPtr()->deactivateEditMode();
    pActiveTool = nullptr;
    editModeActive = false;
    updateToolInfoDockWidget();
}

void ToolsController::initToolBarView()
{

}

// ------- END EVENTS -------

ToolType ToolsController::getActiveToolType() const
{
    return activeToolType;
}


void ToolsController::changeTool(ToolType tt, QAction* pa)
{

    if(activeToolType != tt)
    {
        activeToolType = tt;
//        if(!((int)tt&(int)ToolType::checkableTool))
//            resetEditingMode();

        if(editModeActive)
        {
            if(!pActiveTool)
            {
                resetEditingMode();
                return;
            }
            if(activeToolType != pActiveTool->getToolType())
            {
                emit(stopUsingTool());
                pActiveTool->getToolPtr()->deactivateEditMode();
                doNextMousePressEvent = true;
            }
            else if(activeToolType == pActiveTool->getToolType())
            {
                pActiveTool->getToolPtr()->activateEditMode();
                updateToolInfoDockWidget();
            }
        }

    }
    else {
        activeToolType = ToolType::NoneTool;
    }
    updateToolInfoDockWidget();
}


IE_ModelLayer *ToolsController::getPActiveTool() const
{
    return pActiveTool;
}

void ToolsController::setPActiveToolForEditing(IE_ModelLayer *item)
{
    emit(stopUsingTool());
    resetEditingMode();
    doNextMousePressEvent = true;
    pActiveTool = item;
    editModeActive = true;
    if(activeToolType == pActiveTool->getToolType())
    {
        pActiveTool->getToolPtr()->activateEditMode();
        updateToolInfoDockWidget();
    }
}
ToolSet ToolsController::getToolSetType() const
{
    return toolSetType;
}
void ToolsController::setToolSetType(const ToolSet &value)
{
    toolSetType = value;
    initToolActions();
}
