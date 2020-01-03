#include "ImageEditor.h"


ImageEditor::ImageEditor():QMainWindow(nullptr)
{
    m_ieType = IE_ProfileType::None;
    m_currentTab = 0;
    QMenuBar *pMenuBar;
    pMenuBar = new QMenuBar(this);
    setMenuBar(pMenuBar);
    setCentralWidget(&m_stackedWidget); // без этого никуда не обойтись, так как если каждый раз менять IE_View через setCentralWidget, то старая будет удаляться. СМ документацию Qt


    /// \todo после профилирования необходимо IE связать с модулем вычислений

}

ImageEditor::~ImageEditor()
{
        clearIEViewVec();
//    QMainWindow::~QMainWindow();
}

void ImageEditor::closeEvent(QCloseEvent *)
{
    qDebug() << "ImageEditor::closeEvent(QCloseEvent *)";
    //close();
    emit wasClosed();
}

int ImageEditor::init(IE_ProfileType ie_type)
{
    pCurrentFVFastManagment = nullptr;
    m_ieType = ie_type;
    if(!m_ieViewVec.count())
        return 1;

    setWindowTitle(getIE_ProfileType(m_ieType)); //! \todo зависит от типа
    setDockOptions(DockOption::AllowNestedDocks | DockOption::AllowTabbedDocks);
    m_pTopToolBar = new QToolBar(this);

    m_pTopToolBar->setIconSize(QSize(25,25));
    QAction * pAct = new QAction(QIcon(":/icon/imageEditor/colored/save"), "Сохранить");
    m_pTopToolBar->addAction(pAct);
    connect(pAct, &QAction::trigger, [this]()
    {
        IE_Model * pModel = m_ieViewVec[0]->getModel();
        if(!(pModel->saveModel()))
        {
            emit this->wasSaved(pModel->get_TSP_patientData());
        }
    });

    QWidget* spacer = new QWidget(m_pTopToolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pTopToolBar->addWidget(spacer);

    switch(m_ieType)
    {
    case IE_ProfileType::None:
    {

        break;
    }
    case IE_ProfileType::Trichoscopy:
    {
        QTabBar * locTabBar = new QTabBar(this);
        foreach(IE_View * pIEView, m_ieViewVec)
        {
            IE_Model * pIE_model = pIEView->getModel();
            ToolsController * pIE_toolCnt = pIEView->getToolsController();
            locTabBar->addTab(UI_getIEM_type_title(pIE_model->getIEM_type()));
            addToolBar(Qt::LeftToolBarArea, pIE_toolCnt);
            pIE_toolCnt->hide();
            addDockWidget(Qt::RightDockWidgetArea,pIE_toolCnt->getPDock());



            switch ( pIE_model->getIEM_type() )
            {
                case IEM_type::TrichoscopyPatterns:
            {
                    if(!pIE_model->getImageBaseDockWidget())
                        break;
                    addDockWidget(Qt::RightDockWidgetArea, pIE_model->getImageBaseDockWidget());
                    tabifyDockWidget(pIE_toolCnt->getPDock(), pIE_model->getImageBaseDockWidget());

                    pIE_model->getImageBaseDockWidget()->hide();
                    break;
            }
            }
            pIE_toolCnt->getPDock()->hide();
            addDockWidget(Qt::RightDockWidgetArea,pIE_model->getFieldOfViewControllerInfoDock());
            pIE_model->getFieldOfViewControllerInfoDock()->hide();

        }


        m_pTopToolBar->addWidget(locTabBar);
        locTabBar->setCurrentIndex(0);
        connect(locTabBar, &QTabBar::currentChanged, this, &ImageEditor::changeTab);

        break;
    }
    case IE_ProfileType::Trichogram:
    case IE_ProfileType::Phototrichogram:
    case IE_ProfileType::Simple:
    case IE_ProfileType::MeasureIndex:
    case IE_ProfileType::Full:
    case IE_ProfileType::OnlyImage:
    {
        break;
    }


    }

    foreach(IE_View * pIEView, m_ieViewVec)
    {
        m_stackedWidget.addWidget(pIEView);
    }


    connect(m_ieViewVec[0]->getModel(), &IE_Model::wasSaved, [this]()
    {
        foreach(IE_View * pIEView, m_ieViewVec)
        {
            // в цикле сохраняпм все привзяаные к первой модели
            if(pIEView == m_ieViewVec[0])
                continue;
            pIEView->getModel()->saveModel();
        }
    });


    m_pTopToolBar->setMovable(false);
    this->setUnifiedTitleAndToolBarOnMac(true);
    spacer = new QWidget(m_pTopToolBar);
        spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pTopToolBar->addWidget(spacer);



    addToolBar(Qt::TopToolBarArea, m_pTopToolBar);

    m_currentTab = 0;
    m_stackedWidget.setCurrentIndex(m_currentTab);
    changeTab(0);

    return 0;
}

int ImageEditor::initModelsAsNew(TSP_PatientData patientData, IE_ProfileType ie_type)
{
    patientData.modelDir = patientData.patientDir;
    clearIEViewVec();

    m_ieType = ie_type;
    switch(m_ieType)
    {
    case IE_ProfileType::None:
    {
        break;
    }
    case IE_ProfileType::Trichoscopy:
    {
        TSP_PatientData relatePatienData = patientData;
        relatePatienData = patientData;
        for (int i=0; i<2;i++)
        {
            IE_Model * pIE_model = new IE_Model();
            IE_View * pIEView = new IE_View(pIE_model);

            switch (i)
            {
            case 0:
            {
                pIE_model->initAsNewModel(patientData, IEM_type::HairDencity, ie_type);
                relatePatienData.modelDir = pIE_model->getPGlobal_data()->getModelResDirPath();
                break;
            }
            case 1:
            {
                pIE_model->initAsNewModel(relatePatienData, IEM_type::TrichoscopyPatterns, ie_type);
                m_ieViewVec[0]->getModel()->addRelatedModel( QString("%1/%2_IE_model.json")
                                                              .arg(pIE_model->getPath())
                                                              .arg(pIE_model->get_TSP_patientData().model_ID)
                                                            );
                break;
            }
//            case 2:
//            {
//                pIE_model->initAsNewModel(relatePatienData, IEM_type::AssessmentOfScalp, ie_type);
//                m_ieViewVec[0]->getModel()->addRelatedModel( QString("%1/%2_IE_model.json")
//                                                              .arg(pIE_model->getPath())
//                                                              .arg(pIE_model->get_TSP_patientData().model_ID)
//                                                            );
//                break;
//            }
//            case 3:
//            {
//                pIE_model->initAsNewModel(relatePatienData, IEM_type::AssessmentOfHairRoots, ie_type);
//                m_ieViewVec[0]->getModel()->addRelatedModel( QString("%1/%2_IE_model.json")
//                                                              .arg(pIE_model->getPath())
//                                                              .arg(pIE_model->get_TSP_patientData().model_ID)
//                                                            );
//                break;
//            }
//            case 4:
//            {
//                pIE_model->initAsNewModel(relatePatienData, IEM_type::AssessmentOfHairRods, ie_type);
//                m_ieViewVec[0]->getModel()->addRelatedModel( QString("%1/%2_IE_model.json")
//                                                              .arg(pIE_model->getPath())
//                                                              .arg(pIE_model->get_TSP_patientData().model_ID)
//                                                            );
//                break;
//            }
//            case 5:
//            {
//                pIE_model->initAsNewModel(relatePatienData, IEM_type::DermatoscopyOfNeoplasms, ie_type);
//                m_ieViewVec[0]->getModel()->addRelatedModel( QString("%1/%2_IE_model.json")
//                                                              .arg(pIE_model->getPath())
//                                                              .arg(pIE_model->get_TSP_patientData().model_ID)
//                                                            );
//                break;
//            }

            }
            m_ieViewVec.push_back(pIEView);

        }

        break;
    }
    case IE_ProfileType::Trichogram:
    case IE_ProfileType::Phototrichogram:
    case IE_ProfileType::Simple:
    case IE_ProfileType::MeasureIndex:
    case IE_ProfileType::Full:
    case IE_ProfileType::OnlyImage:
    {
        IE_Model * pIE_model = new IE_Model();
        IE_View * pIEView = new IE_View(pIE_model);
        pIE_model->initAsNewModel(patientData, IEM_type::HairDencity, ie_type);
        m_ieViewVec.push_front(pIEView);
        break;
    }


    }
    int answer = init(ie_type);
    if(answer)
        return answer;

    return 0;
}

int ImageEditor::initModels(TSP_PatientData patientData)
{
    clearIEViewVec();

    IE_View * pIEView = new IE_View(new IE_Model());

    if(pIEView->getModel()->initWithModel(patientData))
    {
        delete pIEView;
        return 1;
    }
    m_ieViewVec.push_front(pIEView);

    if(!m_ieViewVec[0]->getModel()->getRelatedModelList().isEmpty())
    {
        QStringList relatedModelList = m_ieViewVec[0]->getModel()->getRelatedModelList();
        foreach(QString modelFilePath, relatedModelList)
        {
            IE_Model * pIE_model = new IE_Model();
            IE_View * pIEView = new IE_View(pIE_model);

            TSP_PatientData relatedPatientData = patientData;
            patientData.modelFilePath = modelFilePath;
            if(pIE_model->initWithModel(patientData))
                delete pIEView;
            else
                m_ieViewVec.push_back(pIEView);
        }
    }

    IE_ProfileType ie_type = m_ieViewVec[0]->getModel()->getIE_ProfileType();
    int answer = init(ie_type);
    if(answer)
        return answer;
    return 0;
}

void ImageEditor::menuInit()
{
  QMenuBar *pMenuBar = menuBar();

  pMenuBar->clear();

    QMenu *oneMenu = new QMenu("Файл");

    IE_Model * pModel = m_ieViewVec[0]->getModel();

    QAction *pActionNewFile = new QAction("Сохранить");
    connect(pActionNewFile, &QAction::triggered, [this, pModel]()
    {
        if(!(pModel->saveModel()))
        {
            emit this->wasSaved(pModel->get_TSP_patientData());
        }
    });
    oneMenu->addAction(pActionNewFile);

    pActionNewFile = new QAction("Сохранить изображение");
    oneMenu->addAction(pActionNewFile);

    pMenuBar->addAction(oneMenu->menuAction());

    oneMenu = new QMenu("Правка");

   pMenuBar->addAction(oneMenu->menuAction());

   oneMenu = new QMenu("Вычисления");
   pActionNewFile = new QAction("Масштаб изображения");
   connect(pActionNewFile, &QAction::triggered, [this](){
       makeCalibration();
   });
   oneMenu->addAction(pActionNewFile);
   pActionNewFile = new QAction("Настройки");
   connect(pActionNewFile, &QAction::triggered, [pModel](){emit(pModel->setInputArgs());});
   oneMenu->addAction(pActionNewFile);

   oneMenu->addSeparator();

   pActionNewFile = new QAction("Плотность волос");
   connect(pActionNewFile, &QAction::triggered, [pModel](){emit(pModel->makeHairDensityComputeWithWidget());});
   oneMenu->addAction(pActionNewFile);
   pActionNewFile = new QAction("Диаметр волос");
   connect(pActionNewFile, &QAction::triggered, [pModel](){emit(pModel->makeHairDiameterComputeWithWidget());});
   oneMenu->addAction(pActionNewFile);

   pMenuBar->addAction(oneMenu->menuAction());




   oneMenu = new QMenu("Отчет");
   pActionNewFile = new QAction("Экспортировать...");
   pActionNewFile->setDisabled(true);
   oneMenu->addAction(pActionNewFile);
   oneMenu->addSeparator();
   pActionNewFile = new QAction("Трихоскопия");
   pActionNewFile->setDisabled(false);
   oneMenu->addAction(pActionNewFile);
   pActionNewFile = new QAction("Фототрихограмма");
   pActionNewFile->setDisabled(true);
   oneMenu->addAction(pActionNewFile);
   pActionNewFile = new QAction("Трихограмма");
   pActionNewFile->setDisabled(true);
   oneMenu->addAction(pActionNewFile);
   pActionNewFile = new QAction("Дерматоскопия");
   pActionNewFile->setDisabled(true);
   oneMenu->addAction(pActionNewFile);

   pMenuBar->addAction(oneMenu->menuAction());


   oneMenu = new QMenu("Трихоскопия");
   pActionNewFile = new QAction("Отчет");
   pActionNewFile->setDisabled(true);
   oneMenu->addAction(pActionNewFile);
   pActionNewFile = new QAction("Сохранить отчет");
   pActionNewFile->setDisabled(true);
   oneMenu->addAction(pActionNewFile);
   oneMenu->addSeparator();

   QActionGroup * alignmentGroup = new QActionGroup(this);
   alignmentGroup->setExclusive(true);

   pActionNewFile = new QAction("Активный раздел:");
   pActionNewFile->setDisabled(true);
   oneMenu->addAction(pActionNewFile);

   pActionNewFile = new QAction("Плотность волос");
   alignmentGroup->addAction(pActionNewFile);
   pActionNewFile->setCheckable(true);
   pActionNewFile->setChecked(true);
   pActionNewFile = new QAction("Оценка кожи головы");
   pActionNewFile->setCheckable(true);
//   pActionNewFile->setDisabled(true);
   alignmentGroup->addAction(pActionNewFile);
   pActionNewFile = new QAction("Оценка корней");
   pActionNewFile->setCheckable(true);
//   pActionNewFile->setDisabled(true);
   alignmentGroup->addAction(pActionNewFile);
   pActionNewFile = new QAction("Оценка стержней");
   pActionNewFile->setCheckable(true);
//   pActionNewFile->setDisabled(true);
   alignmentGroup->addAction(pActionNewFile);
   pActionNewFile = new QAction("Дерматоскопия новообразований");
   pActionNewFile->setCheckable(true);
//   pActionNewFile->setDisabled(true);
   alignmentGroup->addAction(pActionNewFile);
   oneMenu->addActions(alignmentGroup->actions());

   pMenuBar->addAction(oneMenu->menuAction());

   setMenuBar(pMenuBar);
}

void ImageEditor::clearIEViewVec()
{
    foreach (IE_View* ie_view, m_ieViewVec)
    {
        m_stackedWidget.removeWidget(ie_view);
        delete ie_view;
    }

    m_ieViewVec.clear();
}

void ImageEditor::makeCalibration        ()
{
    qDebug() << "getMeasureIndex = " << m_ieViewVec[0]->getModel()->getMeasureIndex();
    IE_ToolCalibration* calib = new IE_ToolCalibration(nullptr, m_ieViewVec[0]->getModel()->getMeasureIndex());
    calib->show();
    connect(calib, &IE_ToolCalibration::saveChangedMeasureIndex,
            [this, calib](){
        m_ieViewVec[0]->getModel()->setMeasureIndex(calib->getMeasureIndex());
    });
}

int ImageEditor::open(TSP_PatientData patientData)
{
    int answer = initModels(patientData);
    if(answer)
        return answer;
    //! \todo только при открытии обследования необходимо по-другому обрабатывать.  записывать  связанные модели в TabArray ???

    //! инициализация происходит только после прочтения модели. Так как необходим IEM_type и зависимости с другими моделями.
    return 0;
}

int ImageEditor::makeNew(TSP_PatientData patientData, IE_ProfileType ie_type)
{
    int answer = initModelsAsNew(patientData, ie_type);
    if(answer)
        return answer;
    return 0;
}

void ImageEditor::changeTab(int viewIndex)
{
    if(viewIndex >= m_ieViewVec.count())
        return;
    if(viewIndex < 0)
        return;

    IE_View * pIE_view = m_ieViewVec[m_currentTab];
    IE_Model * pIE_model = pIE_view->getModel();
    ToolsController * pIE_toolCnt = pIE_view->getToolsController();

    pIE_toolCnt->hide();
    pIE_toolCnt->getPDock()->hide();
    pIE_model->getFieldOfViewControllerInfoDock()->hide();
    switch (pIE_model->getIEM_type())
    {
        case IEM_type::TrichoscopyPatterns:
            if(pIE_model->getImageBaseDockWidget())
                pIE_model->getImageBaseDockWidget()->hide();
            break;
    }


    m_currentTab = viewIndex;
    m_stackedWidget.setCurrentIndex(m_currentTab);
    pIE_view = m_ieViewVec[m_currentTab];
    pIE_model = pIE_view->getModel();
    pIE_toolCnt = pIE_view->getToolsController();

    pIE_toolCnt->show();

    pIE_model->getFieldOfViewControllerInfoDock()->show();
    switch (pIE_model->getIEM_type())
    {
        case IEM_type::TrichoscopyPatterns:
        if(pIE_model->getImageBaseDockWidget())
        {
            pIE_model->getImageBaseDockWidget()->show();
//            tabifyDockWidget(pIE_toolCnt->getPDock(), pIE_model->getImageBaseDockWidget());
        }
            break;

    }
    pIE_toolCnt->getPDock()->show();


    menuInit();
}


