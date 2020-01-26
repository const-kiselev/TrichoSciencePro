#include "ie_toolController.h"


/// \todo  сделать логику обнудения pActiveTool = nullptr; !!!!


ToolsController::ToolsController(_global_ie *gi):_p_ie_global_data(gi)
{
    setIconSize(QSize(20,20));
    setMovable(false);

    toolSetType = ToolSet::AllTools;
    pDock = nullptr;
    pActiveTool = nullptr;
    activeToolType = ToolType::NoneTool;
    doNextMousePressEvent = true;
    editModeActive = false;

    connect(this, &ToolsController::startUsingNewTool, &ToolsController::updateToolInfoDockWidget);
    initInfoDock();
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
    case ToolType::HairStandard:
    {
            switch (pe->type())
            {
            case QEvent::MouseButtonPress:
            {
                if(!editModeActive)
                {
                    switch (activeToolType) {
                    case ToolType::HairStandard:
                    pActiveTool = new IE_ModelLayer(ToolType::HairStandard,
                                                    new IE_Tool_HairStandard(p_ie_global_data()));
                    break;

                    }

                    emit(startUsingNewTool());
//                    pDock->setWidget();
                }
                else {
                    pActiveTool->getToolPtr()->activateEditMode();
//                    dynamic_cast<IE_Tool_Line*>( pActiveTool->parentItem())->activateEditMode();
                }
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(pe);
                QPointF mousePos = mouseEvent->pos();
                if(mouseEvent->localPos()!=mouseEvent->pos())
                    mousePos = mouseEvent->localPos();

                // создаем сигнал о том, что началось использование нового инстурмента

                doNextMousePressEvent = false;
                pActiveTool->getToolPtr()->mouseFirstPress(mousePos);
//                dynamic_cast<IE_Tool_Line*>(pActiveTool->parentItem())->mouseFirstPress(mousePos);


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
                if(pActiveTool->getToolPtr()->mouseMove(mousePos) ) //dynamic_cast<IE_Tool_Line*>(pActiveTool->parentItem())->mouseMove(mousePos)
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
//                dynamic_cast<IE_Tool_Line*>(pActiveTool->parentItem())->release(mousePos);
                pActiveTool->getToolPtr()->release(mousePos);

            }
                break;
            case QEvent::Wheel:
            {
                if(pActiveTool)
                    pActiveTool->getToolPtr()->wheelMode(static_cast<QWheelEvent*>(pe));
//                    dynamic_cast<IE_Tool_Line*>(pActiveTool->parentItem())->wheelMode(static_cast<QWheelEvent*>(pe));
            }
                break;
            } // ----- END switch (pe->type())
            break;
    // ----- END case SimpleLine
    }
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
                        pActiveTool = new IE_ModelLayer(ToolType::Ruler, new IERuler(p_ie_global_data()));
                        break;
                        case ToolType::SimpleLine:
                        pActiveTool = new IE_ModelLayer(ToolType::SimpleLine, new IE_Tool_Line(p_ie_global_data()));
                        break;
                        case ToolType::DensityAndDiameter:
                        pActiveTool = new IE_ModelLayer(ToolType::DensityAndDiameter,
                                                        new IE_Line_DD(p_ie_global_data()));
                        break;

                    }

                    emit(startUsingNewTool());
//                    pDock->setWidget();
                }
                else {
                    pActiveTool->getToolPtr()->activateEditMode();
//                    dynamic_cast<IE_Tool_Line*>( pActiveTool->parentItem())->activateEditMode();
                }
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(pe);
                QPointF mousePos = mouseEvent->pos();
                if(mouseEvent->localPos()!=mouseEvent->pos())
                    mousePos = mouseEvent->localPos();

                // создаем сигнал о том, что началось использование нового инстурмента

                doNextMousePressEvent = false;
                pActiveTool->getToolPtr()->mouseFirstPress(mousePos);
//                dynamic_cast<IE_Tool_Line*>(pActiveTool->parentItem())->mouseFirstPress(mousePos);


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
                if(pActiveTool->getToolPtr()->mouseMove(mousePos) ) //dynamic_cast<IE_Tool_Line*>(pActiveTool->parentItem())->mouseMove(mousePos)
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
//                dynamic_cast<IE_Tool_Line*>(pActiveTool->parentItem())->release(mousePos);
                pActiveTool->getToolPtr()->release(mousePos);

            }
                break;
            case QEvent::Wheel:
            {
                if(pActiveTool)
                    pActiveTool->getToolPtr()->wheelMode(static_cast<QWheelEvent*>(pe));
//                    dynamic_cast<IE_Tool_Line*>(pActiveTool->parentItem())->wheelMode(static_cast<QWheelEvent*>(pe));
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

    case ToolType::InterfollicularRegion:
    case ToolType::InterfollicularRegion_FineScaling:
    case ToolType::InterfollicularRegion_YellowishScaling:
    case ToolType::InterfollicularRegion_WhiteScaling:
    case ToolType::InterfollicularRegion_LamellarScaling:
    case ToolType::InterfollicularRegion_PerifollicularScaling:
    case ToolType::InterfollicularRegion_WhiteArea:
    case ToolType::InterfollicularRegion_PinkArea:
    case ToolType::InterfollicularRegion_RedArea:
    case ToolType::InterfollicularRegion_MilkyRedArea:
    case ToolType::InterfollicularRegion_Pustule:
    case ToolType::InterfollicularRegion_Exudate:


    case ToolType::HairFollicleOpening:
    case ToolType::HairFollicleOpening_PeripilarSigns:
    case ToolType::HairFollicleOpening_YellowDot:
    case ToolType::HairFollicleOpening_BlackDot:
    case ToolType::HairFollicleOpening_WhiteDot:
    case ToolType::HairFollicleOpening_RedDot:
    case ToolType::HairFollicleOpening_PeripilarCast:

    case ToolType::BloodVessel:
    case ToolType::BloodVessel_ThickArborizingVessel:
    case ToolType::BloodVessel_ThinArborizingVessel:
    case ToolType::BloodVessel_GlomerularAndTwisted:
    case ToolType::BloodVessel_CommaHair:


    case ToolType::HairShaft:
    case ToolType::HairShaft_Healthy:
    case ToolType::HairShaft_Intermediate:
    case ToolType::HairShaft_Vellus:
    case ToolType::HairShaft_UprightRegrowingHair:
    case ToolType::HairShaft_ExclamationMark:
    case ToolType::HairShaft_Cone:
    case ToolType::HairShaft_Trichoptilosis:
    case ToolType::HairShaft_Trichoclasia:
    case ToolType::HairShaft_Trichoshisis:
    case ToolType::HairShaft_Trichorexis:
    case ToolType::HairShaft_BrokenHairsAtDifferentLength:
    case ToolType::HairShaft_TuftedHairs:
    case ToolType::HairShaft_FlameAndBroom:
    case ToolType::HairShaft_ZigzagAndIntermittent:
    case ToolType::HairShaft_Twisted:
    case ToolType::HairShaft_PiliAnnulati:
    case ToolType::HairShaft_Moniletrix:
    case ToolType::HairShaft_Trichonodosis:
    case ToolType::HairShaft_BambooHair:


    case ToolType::HairRoot:
    case ToolType::HairRoot_Anagen:
    case ToolType::HairRoot_Catagen:
    case ToolType::HairRoot_Telogen:
    case ToolType::HairRoot_Dystrophic:
    case ToolType::HairRoot_Broken:
    case ToolType::HairRoot_Seborrhea:
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

void ToolsController::resetPActiveTool()
{
    pActiveTool = nullptr;
    resetEditingMode();
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

void ToolsController::groupItem(ToolType tt, QAction *pa)
{

}

_global_ie *ToolsController::p_ie_global_data() const
{
    return _p_ie_global_data;
}

QDockWidget *ToolsController::getPDock() const
{
    return pDock;
}

void ToolsController::setP_ie_global_data(_global_ie *p_ie_global_data)
{
    _p_ie_global_data = p_ie_global_data;
}

void ToolsController::initToolActions()
{

    QList<ToolType> toolsList;
    switch (toolSetType){
    case ToolSet::HairDencity:
    case ToolSet::Simple:
    {
        toolsList   << ToolType::NoneTool
//                    << ToolType::SimpleLine
                    << ToolType::DensityAndDiameter
                    << ToolType::HairStandard
                    << ToolType::Zoom
                    << ToolType::Marker_FollicularUnit
                    << ToolType::Ruler;

        break;
    }
    case ToolSet::AllTools:
    {
        toolsList   << ToolType::NoneTool
                    << ToolType::SimpleLine
                    << ToolType::DensityAndDiameter
                    << ToolType::HairStandard
                    << ToolType::Zoom
                    << ToolType::Marker_FollicularUnit
                    << ToolType::Ruler

                    << ToolType::InterfollicularRegion
                    << ToolType::HairFollicleOpening
                    << ToolType::BloodVessel
                    << ToolType::HairShaft
                    << ToolType::HairRoot;

        break;
    }
    case ToolSet::TrichoscopyPatterns:
    {
        toolsList   << ToolType::NoneTool
//                    << ToolType::SimpleLine
                    << ToolType::HairStandard
                    << ToolType::DensityAndDiameter
                    << ToolType::Zoom
                    << ToolType::Marker_FollicularUnit
                    << ToolType::Ruler

                    << ToolType::InterfollicularRegion
                    << ToolType::HairFollicleOpening
                    << ToolType::BloodVessel
                    << ToolType::HairShaft
                    << ToolType::HairRoot;

        break;
    }
    case ToolSet::MeasureIndex:
        toolsList << ToolType::NoneTool
                  << ToolType::Ruler
                  << ToolType::Zoom
                  ;
        break;

    }
    // группа нужна для того, чтобы можно было выбрать только один инструмент
    QActionGroup *pActionGroup = new QActionGroup(this);
    QAction *pToolAction;
    QToolButton * pToolButton;
    pToolButton=nullptr;
    pToolAction=nullptr;
    clear();

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
            pToolAction = new QAction(QPixmap("://toolIcons/DensityAndDiameter"),"Плотность, диаметр и длина");
            break;
        }
        case ToolType::HairStandard:
        {
            pToolAction = new QAction(QPixmap("://toolIcons/DensityAndDiameter"),"Эталон");
            break;
        }
        case ToolType::Zoom:
        {
            pToolAction = new QAction(QPixmap(":/icon/imageEditor/colored/zoom"), "Лупа");
            break;
        }
        case ToolType::Marker_FollicularUnit:
        {
            pToolAction = new QAction(QPixmap("://toolIcons/Marker_FollicularUnit"), "Маркер фолликулярных юнитов");
            break;
        }
        case ToolType::Ruler:
        {
            pToolAction = new QAction(QPixmap(":/icon/imageEditor/black/resources/img/black/ruler.png"), "Линейка");
            break;
        }

            case ToolType::InterfollicularRegion:
            case ToolType::HairFollicleOpening:
            case ToolType::BloodVessel:
            case ToolType::HairShaft:
            case ToolType::HairRoot:
        {
            pToolButton = new QToolButton(this);
            pToolButton->setIcon(QPixmap("://PeripilarSign_SpikyHair"));
            pToolButton->setText(getToolTitle(actionType));
            pToolButton->setToolTip(getToolTitle(actionType));
            QMenu * pMenu = new QMenu(pToolButton);
            pToolButton->setCheckable(true);


            QList<QAction*> list = IE_Tool_Marker::getListOfAction(actionType);
            // соединяем все действия с тригерром выбора для активации нового инстурмента
            foreach(QAction * pAct, list)
            {
                pAct->setCheckable(true);
                connect(pAct, &QAction::triggered, [=]() {
                    this->changeTool(convertToolTitleToToolType(pAct->data().toString()), pAct);
                    pAct->setChecked(true);
                });
                pMenu->addAction(pAct);
                pActionGroup->addAction(pAct);
            }

            pToolButton->setMenu(pMenu);

            pToolButton->setPopupMode(QToolButton::InstantPopup);
//
            break;
        }


        }
        if(!pToolButton)
        {
            connect(pToolAction, &QAction::triggered, [=]() {
                this->changeTool(actionType, pToolAction);
            });
            pToolAction->setCheckable(true);
            pActionGroup->addAction(pToolAction);
            addAction(pToolAction);
//            addAction(pToolAction);
        }
        else
        {
            //  соединение для выделения группы, если ее элемент выбран
            connect(pActionGroup, &QActionGroup::triggered, [=]()
            {
                bool checked = false;
                QList<QAction*> actList = pToolButton->menu()->actions();
                foreach(QAction* pAct, actList)
                    if(pAct->isChecked())
                    {
                        checked = true;
                        break;
                    }
                pToolButton->setChecked(checked);
            });

            addWidget(pToolButton);
        }
        pToolButton=nullptr;
        pToolAction=nullptr;
    }

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
//        if( getToolTypeGroupSet().contains(tt) )
//            return;

        activeToolType = tt;

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
