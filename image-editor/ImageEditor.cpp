#include "imageEditor.h"


TSPimageEditor::TSPimageEditor():QMainWindow(nullptr)
{
    init();
    pModel->initAsNewModel("/Users/const/main/Develop/Trichoscience/projects/tmp1/3.jpg");

    // после профилирования необходимо IE связать с модулем вычислений

    //QWidget *tmpPWidget = new QWidget();
    //QBoxLayout *pbxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    //pbxLayout->addWidget(pView);
    //pbxLayout->setContentsMargins(0,0,0,0);
    //tmpPWidget->setLayout(pbxLayout);
    //setCentralWidget(tmpPWidget);



}

TSPimageEditor::TSPimageEditor(_Model_patientData patientData)
{
    init();
    pModel->initAsNewModel(patientData);
}

TSPimageEditor::~TSPimageEditor()
{


}

void TSPimageEditor::closeEvent(QCloseEvent *)
{
    close();
    delete pModel;
    emit wasClosed();
}

void TSPimageEditor::init()
{
    setWindowTitle("TrichoScience Pro :: Редактор изображения");
    pModel = new TSPImageEditorModel();
    pView = new TSPImageEditorView(pModel);

    pIEToolController = pView->getPToolsController();
    pIEToolController->setToolSetType(ToolSet::AllTools);
    pModel->setPToolCnt(pIEToolController);
    connect(pIEToolController, &ToolsController::startUsingNewTool,
            pModel, &TSPImageEditorModel::addLayerViaToolCnt);
    addToolBar(Qt::LeftToolBarArea, pView->getPToolsController());


    addDockWidget(Qt::RightDockWidgetArea,pModel->initToolInfoDock());
   // addDockWidget(Qt::RightDockWidgetArea, pView->initDockDebugWidget());
    addDockWidget(Qt::RightDockWidgetArea, pModel->initLayersDock());
   // addDockWidget(Qt::RightDockWidgetArea, pModel->initComputeDock());
    setCentralWidget(pView);
    menuInit();
}

void TSPimageEditor::menuInit()
{
  QMenuBar *pMenuBar;


   #ifdef Q_OS_MAC
       pMenuBar = new QMenuBar(0);
   #else
       pMenuBar = new QMenuBar();
   #endif
   QMenu *oneMenu = new QMenu("Файл");
    QAction *pActionNewFile = new QAction("Сохранить");
    connect(pActionNewFile, &QAction::triggered, [this](){
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

   pActionNewFile = new QAction("Масштаб изображения");
   connect(pActionNewFile, &QAction::triggered, [this](){
       emit(this->makeCalibration(IE_GLOBAL_DATA.getMeasureIndex()));
   });
   oneMenu->addAction(pActionNewFile);

   pMenuBar->addAction(oneMenu->menuAction());

   oneMenu = new QMenu("Вычисления");
   pActionNewFile = new QAction("Настройки");
   connect(pActionNewFile, &QAction::triggered, [this](){emit(this->pModel->setInputArgs());});
   oneMenu->addAction(pActionNewFile);
   oneMenu->addSeparator();
   pActionNewFile = new QAction("Плотность волос");
   connect(pActionNewFile, &QAction::triggered, [this](){emit(this->pModel->makeHairDensityComputeWithWidget());});
   oneMenu->addAction(pActionNewFile);
   pActionNewFile = new QAction("Диаметр волос");
   pActionNewFile->setDisabled(true);
   oneMenu->addAction(pActionNewFile);

   pMenuBar->addAction(oneMenu->menuAction());


}

void TSPimageEditor::makeCalibration(qreal mIndex)
{
    IE_ToolCalibration* calib = new IE_ToolCalibration(nullptr, IE_GLOBAL_DATA.getMeasureIndex());
    calib->show();
    connect(calib, &IE_ToolCalibration::saveChangedMeasureIndex,
            [this, calib](){
        pModel->setMeasureIndex(calib->getMeasureIndex());
    });
}

