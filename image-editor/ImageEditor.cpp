#include "ImageEditor.h"


ImageEditor::ImageEditor():QMainWindow(nullptr)
{
    init();
#ifdef _IE_DEBUG_NEW_MODEL_
    _Model_patientData patientData;
    patientData.modelPath = "data/0";
    patientData.modelDir = "data/0";
    pModel->initAsNewModel(patientData);
#endif // _DEBUG_NEW_MODEL_
#ifdef _IE_DEBUG_OPEN_MODEL_
    pModel->initWithModel(_Model_patientData());
#endif // _DEBUG_OPEN_MODEL_

    /// \todo после профилирования необходимо IE связать с модулем вычислений

    //QWidget *tmpPWidget = new QWidget();
    //QBoxLayout *pbxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    //pbxLayout->addWidget(pView);
    //pbxLayout->setContentsMargins(0,0,0,0);
    //tmpPWidget->setLayout(pbxLayout);
    //setCentralWidget(tmpPWidget);



}

ImageEditor::~ImageEditor()
{

//    delete pModel;
//    pModel = nullptr;
}

void ImageEditor::closeEvent(QCloseEvent *)
{
    close();
    delete pModel;
    pModel = nullptr;
    delete pView;
    pView = nullptr;
    pIEToolController = nullptr;
    emit wasClosed();
}

void ImageEditor::init()
{
    setWindowTitle("TrichoScience Pro :: Редактор изображения");
    pModel = new IE_Model();
    pView = new IE_View(pModel);

    pIEToolController = pView->getPToolsController();
    pIEToolController->setToolSetType(ToolSet::AllTools);
    pModel->setPToolCnt(pIEToolController);
    connect(pIEToolController, &ToolsController::startUsingNewTool,
            pModel, &IE_Model::addLayerViaToolCnt);
    addToolBar(Qt::LeftToolBarArea, pView->getPToolsController());


    addDockWidget(Qt::RightDockWidgetArea,pModel->initToolInfoDock());
   // addDockWidget(Qt::RightDockWidgetArea, pView->initDockDebugWidget());
    addDockWidget(Qt::RightDockWidgetArea, pModel->initLayersDock());
   // addDockWidget(Qt::RightDockWidgetArea, pModel->initComputeDock());
    setCentralWidget(pView);

}

void ImageEditor::menuInit()
{
  QMenuBar *pMenuBar;


    #ifdef Q_OS_MAC
       pMenuBar = new QMenuBar(this);
    #else
       pMenuBar = new QMenuBar();
    #endif
    QMenu *oneMenu = new QMenu("Файл");

    QAction *pActionNewFile = new QAction("Сохранить");
    connect(pActionNewFile, &QAction::triggered, [this]()
    {
        if(!(this->pModel->saveModel()))
        {
            emit this->wasSaved(this->pModel->get_Model_patientData());
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
   connect(pActionNewFile, &QAction::triggered, [this](){emit(this->pModel->setInputArgs());});
   oneMenu->addAction(pActionNewFile);

   oneMenu->addSeparator();

   pActionNewFile = new QAction("Плотность волос");
   connect(pActionNewFile, &QAction::triggered, [this](){emit(this->pModel->makeHairDensityComputeWithWidget());});
   oneMenu->addAction(pActionNewFile);
   pActionNewFile = new QAction("Диаметр волос");
   connect(pActionNewFile, &QAction::triggered, [this](){emit(this->pModel->makeHairDiameterComputeWithWidget());});
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


}

void        ImageEditor::makeCalibration        ()
{
    qDebug() << "getMeasureIndex = " << pModel->getMeasureIndex();
    IE_ToolCalibration* calib = new IE_ToolCalibration(nullptr, pModel->getMeasureIndex());
    calib->show();
    connect(calib, &IE_ToolCalibration::saveChangedMeasureIndex,
            [this, calib](){
        pModel->setMeasureIndex(calib->getMeasureIndex());
    });
}

void ImageEditor::open(_Model_patientData patientData)
{
    pModel->initWithModel(patientData);
    menuInit();
}

void ImageEditor::openNew(_Model_patientData patientData)
{
    pModel->initAsNewModel(patientData);
    menuInit();
}


