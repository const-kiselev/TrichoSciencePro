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
    m_ieType = ie_type;

    setWindowTitle(getIE_ProfileType(m_ieType)); //! \todo зависит от типа

    m_pTopToolBar = new QToolBar(this);
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
            IE_Model * pIE_model = pIEView->getPModel();
            ToolsController * pIE_toolCnt = pIEView->getPToolsController();
            locTabBar->addTab(UI_getIEM_type_title(pIE_model->getIEM_type()));

            addToolBar(Qt::LeftToolBarArea, pIE_toolCnt);
            pIE_toolCnt->hide();
            addDockWidget(Qt::RightDockWidgetArea,pIE_toolCnt->getPDock());
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
        m_stackedWidget.addWidget(pIEView);

    connect(m_ieViewVec[0]->getPModel(), &IE_Model::wasSaved, [this]()
    {
        foreach(IE_View * pIEView, m_ieViewVec)
        {
            if(pIEView == m_ieViewVec[0])
                continue;
            pIEView->getPModel()->saveModel();
        }
    });


    m_pTopToolBar->setMovable(false);
    this->setUnifiedTitleAndToolBarOnMac(true);
    addToolBar(Qt::TopToolBarArea, m_pTopToolBar);

    m_currentTab = 0;
    m_stackedWidget.setCurrentIndex(m_currentTab);
    changeTab(0);

    return 0;
}

int ImageEditor::initModelsAsNew(_Model_patientData patientData)
{

    clearIEViewVec();

    m_ieType = patientData.ie_type;
    switch(m_ieType)
    {
    case IE_ProfileType::None:
    {
        break;
    }
    case IE_ProfileType::Trichoscopy:
    {
        _Model_patientData relatePatienData = patientData;
        relatePatienData = patientData;
        for (int i=0; i<6;i++)
        {
            QTime dieTime= QTime::currentTime().addMSecs(10);
               while (QTime::currentTime() < dieTime)
                   QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            IE_Model * pIE_model = new IE_Model();
            IE_View * pIEView = new IE_View(pIE_model);

            switch (i)
            {
            case 0:
            {
                pIE_model->initAsNewModel(patientData, IEM_type::HairDencity);
//                relatePatienData.modelPath = pIE_model->getPGlobal_data()->getModelResDirPath();
                relatePatienData.modelDir = pIE_model->getPGlobal_data()->getModelResDirPath();
                break;
            }
            case 1:
            {
                pIE_model->initAsNewModel(relatePatienData, IEM_type::TrichoscopyPatterns);
                m_ieViewVec[0]->getPModel()->addRelatedModel( QString("%1/%2_IE_model.json")
                                                              .arg(pIE_model->getPath())
                                                              .arg(pIE_model->get_Model_patientData().model_ID)
                                                            );
                break;
            }
            case 2:
            {
                pIE_model->initAsNewModel(relatePatienData, IEM_type::AssessmentOfScalp);
                m_ieViewVec[0]->getPModel()->addRelatedModel( QString("%1/%2_IE_model.json")
                                                              .arg(pIE_model->getPath())
                                                              .arg(pIE_model->get_Model_patientData().model_ID)
                                                            );
                break;
            }
            case 3:
            {
                pIE_model->initAsNewModel(relatePatienData, IEM_type::AssessmentOfHairRoots);
                m_ieViewVec[0]->getPModel()->addRelatedModel( QString("%1/%2_IE_model.json")
                                                              .arg(pIE_model->getPath())
                                                              .arg(pIE_model->get_Model_patientData().model_ID)
                                                            );
                break;
            }
            case 4:
            {
                pIE_model->initAsNewModel(relatePatienData, IEM_type::AssessmentOfHairRods);
                m_ieViewVec[0]->getPModel()->addRelatedModel( QString("%1/%2_IE_model.json")
                                                              .arg(pIE_model->getPath())
                                                              .arg(pIE_model->get_Model_patientData().model_ID)
                                                            );
                break;
            }
            case 5:
            {
                pIE_model->initAsNewModel(relatePatienData, IEM_type::DermatoscopyOfNeoplasms);
                m_ieViewVec[0]->getPModel()->addRelatedModel( QString("%1/%2_IE_model.json")
                                                              .arg(pIE_model->getPath())
                                                              .arg(pIE_model->get_Model_patientData().model_ID)
                                                            );
                break;
            }

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
        pIE_model->initAsNewModel(patientData, IEM_type::HairDencity);
        m_ieViewVec.push_front(pIEView);
        break;
    }


    }
    init(patientData.ie_type);

    return 0;
}

int ImageEditor::initModels(_Model_patientData patientData)
{
    clearIEViewVec();

    IE_View * pIEView = new IE_View(new IE_Model());

    if(pIEView->getPModel()->initWithModel(patientData))
    {
        delete pIEView;
        return 1;
    }
    m_ieViewVec.push_front(pIEView);

    if(!m_ieViewVec[0]->getPModel()->getRelatedModelList().isEmpty())
    {
        QStringList relatedModelList = m_ieViewVec[0]->getPModel()->getRelatedModelList();
        foreach(QString modelPath, relatedModelList)
        {
            IE_Model * pIE_model = new IE_Model();
            IE_View * pIEView = new IE_View(pIE_model);

            _Model_patientData relatedPatientData = patientData;
            patientData.modelPath = modelPath;
            if(pIE_model->initWithModel(patientData))
                delete pIEView;
            else
                m_ieViewVec.push_back(pIEView);
        }
    }

    patientData.ie_type = m_ieViewVec[0]->getPModel()->get_Model_patientData().ie_type;
    init(patientData.ie_type);
    return 0;
}

void ImageEditor::menuInit()
{
  QMenuBar *pMenuBar = menuBar();

  pMenuBar->clear();

    QMenu *oneMenu = new QMenu("Файл");

    IE_Model * pModel = m_ieViewVec[m_currentTab]->getPModel();

    QAction *pActionNewFile = new QAction("Сохранить");
    connect(pActionNewFile, &QAction::triggered, [this, pModel]()
    {
        if(!(pModel->saveModel()))
        {
            emit this->wasSaved(pModel->get_Model_patientData());
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
   pActionNewFile->setDisabled(true);
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
    qDebug() << "getMeasureIndex = " << m_ieViewVec[0]->getPModel()->getMeasureIndex();
    IE_ToolCalibration* calib = new IE_ToolCalibration(nullptr, m_ieViewVec[0]->getPModel()->getMeasureIndex());
    calib->show();
    connect(calib, &IE_ToolCalibration::saveChangedMeasureIndex,
            [this, calib](){
        m_ieViewVec[0]->getPModel()->setMeasureIndex(calib->getMeasureIndex());
    });
}

int ImageEditor::open(_Model_patientData patientData)
{
    initModels(patientData);
    //! \todo только при открытии обследования необходимо по-другому обрабатывать.  записывать  связанные модели в TabArray ???

    //! инициализация происходит только после прочтения модели. Так как необходим IEM_type и зависимости с другими моделями.
    return 0;
}

int ImageEditor::makeNew(_Model_patientData patientData)
{
    initModelsAsNew(patientData);
    return 0;
}

void ImageEditor::changeTab(int viewIndex)
{
    if(viewIndex >= m_ieViewVec.count())
        return;
    if(viewIndex < 0)
        return;

    IE_View * pIE_view = m_ieViewVec[m_currentTab];
    IE_Model * pIE_model = pIE_view->getPModel();
    ToolsController * pIE_toolCnt = pIE_view->getPToolsController();

    pIE_toolCnt->hide();
    pIE_toolCnt->getPDock()->hide();
    pIE_model->getFieldOfViewControllerInfoDock()->hide();


    m_currentTab = viewIndex;
    m_stackedWidget.setCurrentIndex(m_currentTab);

    pIE_view = m_ieViewVec[m_currentTab];
    pIE_model = pIE_view->getPModel();
    pIE_toolCnt = pIE_view->getPToolsController();

    pIE_toolCnt->show();
    pIE_toolCnt->getPDock()->show();
    pIE_model->getFieldOfViewControllerInfoDock()->show();


    menuInit();
}


