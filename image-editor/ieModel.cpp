#include "ieModel.h"

            TSPImageEditorModel::TSPImageEditorModel()
{
    pCompMod = new IE_ComputeModule();
    connect(&IE_GLOBAL_DATA, &_global_ie::changed,
            pCompMod, &IE_ComputeModule::makeCompute);


    connect(pCompMod, &IE_ComputeModule::needListOfElements, [this](){
        for (QList<IE_ModelLayer *>::iterator tmpIter = layersList.begin();
             tmpIter!=layersList.end();tmpIter++)
        {
            pCompMod->addElementToCompute(*tmpIter);
        }

    });


    connect(this, &TSPImageEditorModel::changed, this,
            &TSPImageEditorModel::layersController);


    // ключ к глобальному объекту с информацией о масштабе итд
    qsrand(100000);
    globalDataKey = qrand();
    IE_GLOBAL_DATA.setOwner(globalDataKey);
}

            TSPImageEditorModel::~TSPImageEditorModel()
{
    _modelData.tmpDir.removeRecursively();
//    QGraphicsScene::~QGraphicsScene();
}

int        TSPImageEditorModel::saveModel()
{
    enum SaveFormat
    {Json, Bin}saveFormat;
    saveFormat = SaveFormat::Json;

    if (!_modelData.modelDir.exists()) {
        _modelData.modelDir.mkpath(".");
    }

    _modelData.modelDir.mkpath("./res");


    QString filePAth = _modelData.modelDir.path() +"/"+QString().number(_modelData.model_ID)+"_model.json";
    QFile saveFile(saveFormat == Json
            ? filePAth
            : QStringLiteral("model.dat"));

        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
            return 1;
        }

        QJsonObject gameObject;
        write(gameObject);
        QJsonDocument saveDoc(gameObject);
        saveFile.write(saveFormat == Json
            ? saveDoc.toJson()
            : saveDoc.toBinaryData());
        saveFile.close();
        return 0;
}

void        TSPImageEditorModel::saveModelAsImage()
{

}

void        TSPImageEditorModel::read(const QJsonObject &json)
{

}

void        TSPImageEditorModel::write(QJsonObject &json) const
{
    QJsonArray layerArray;
    foreach(IE_ModelLayer* layer, layersList)
    {
        QJsonObject layerObject;
        layer->write(layerObject);
        layerArray.append(layerObject);
    }
    json["layers"] = layerArray;
}

void        TSPImageEditorModel::initAsNewModel(QString imageFilePath)
{

#ifdef QT_DEBUG
    _modelData.activateTestData();
    _modelData.initNew(_modelData.modelDir.path());
#endif
#ifndef QT_DEBUG
    _modelData.initNew("");
#endif

    if (!_modelData.tmpDir.exists()) {
        _modelData.tmpDir.mkpath(".");
    }
    _modelData.tmpDir.mkpath("./res");
    _modelData.resDir.setPath("res");

    setMainImage(imageFilePath);

}

void TSPImageEditorModel::initAsNewModel(_Model_patientData patientData)
{
    _modelData.initNew(patientData);

    if (!_modelData.tmpDir.exists()) {
        _modelData.tmpDir.mkpath(".");
    }
    _modelData.tmpDir.mkpath("./res");
    _modelData.resDir.setPath("res");

    setMainImage("");
}

void        TSPImageEditorModel::initWithModel(QString modelFilePath)
{

}

// ------- GETTERS and SETTERS

QRectF      TSPImageEditorModel::getImageRect() const
{
    return imageRect;
}
QGraphicsPixmapItem *
            TSPImageEditorModel::getPMainImage() const
{
    return pMainImage;
}
int         TSPImageEditorModel::setMainImage(QString imageFilePath)
{
    IE_Tool_Image *pToolImage_mainImage = new IE_Tool_Image(_modelData.modelDir.path()+"/"+_modelData.resDir.path(),
                                                            _modelData.tmpDir.path()+"/"+_modelData.resDir.path(),
                                                            _modelData.resDir.path(),
                                                            imageFilePath,
                                                            ToolType::MainImage);
    pToolImage_mainImage->setPos(0,0);

    pMainImageLayer = new IE_ModelLayer(ToolType::MainImage, pToolImage_mainImage);

    imageRect = pToolImage_mainImage->boundingRect();

    for (QList<IE_ModelLayer*>::iterator iter = layersList.begin();iter!=layersList.end();iter++)
        if((*iter)->getToolType() == ToolType::MainImage)
            iter = layersList.erase(iter);

    addLayer(pMainImageLayer);

    return 1;
}
qreal       TSPImageEditorModel::getMeasureIndex() const
{
    return IE_GLOBAL_DATA.getMeasureIndex();
}
void        TSPImageEditorModel::setMeasureIndex(const qreal &value)
{
    IE_GLOBAL_DATA.setMeasureIndex(value, globalDataKey);
    IE_GLOBAL_DATA.setUnitType(UnitType::mm, globalDataKey);
    emit(measureIndexChanged(value));
}
ToolsController *
            TSPImageEditorModel::getPToolCnt() const
{
    return pToolCnt;
}
void        TSPImageEditorModel::setPToolCnt(ToolsController *value)
{
    pToolCnt = value;
}
QDockWidget*TSPImageEditorModel::getPDockLayers() const
{
    return pDockLayers;
}
QList<IE_ModelLayer*>::const_iterator
            TSPImageEditorModel::getLayersListIter() const
{
    if(layersList.isEmpty())
        return layersList.end();
    return layersList.begin();
}

QList<IE_ModelLayer *>
            TSPImageEditorModel::getLayersList() const
{
    return layersList;
}
IE_ComputeModule *
            TSPImageEditorModel::getPCompMod() const
{
    return pCompMod;
}
IE_ModelLayer *
            TSPImageEditorModel::getLayerByListIndex(int listIndex)
{
    QList<IE_ModelLayer*>::iterator iterResult = getLayerIteratorByListIndex(listIndex);
    return iterResult == layersList.end() ? nullptr : *iterResult;
}
QList<IE_ModelLayer*>::iterator
            TSPImageEditorModel::getLayerIteratorByListIndex(int listIndex)
{
    if(listIndex<0 || listIndex >= layersList.count())
        return layersList.end();

    QList<IE_ModelLayer*>::iterator iter = layersList.begin();
    for (int i=0;i<listIndex;i++, iter++);
    return iter;
}

// ------- END GETTERS and SETTERS

// !!!!!! ------- TODO:
QDockWidget*TSPImageEditorModel::initInfoDock()
{
}
QDockWidget*TSPImageEditorModel::initComputeDock()
{
    return pCompMod->initViewTable();
}
QDockWidget*TSPImageEditorModel::initToolInfoDock()
{
    return pToolCnt->initInfoDock();
}

void        TSPImageEditorModel::makeHairDensityComputeWithWidget()
{
    int i = 0, termin = 0, well = 0;
    qreal penWidth;
    for (QList<IE_ModelLayer *>::iterator tmpIter = layersList.begin();
         tmpIter!=layersList.end();tmpIter++)
    {
        if( tmpIter.i->t()->getToolType() == ToolType::DensityAndDiameter)
        {
            if(!tmpIter.i->t()->isVisible())
                continue;
            i++;
            penWidth = dynamic_cast<IELine*>(tmpIter.i->t()->getToolPtr())->getPenWidth();
                if(IE_GLOBAL_DATA.convertWithForamtF(penWidth, (UnitType)IE_GLOBAL_DATA.getIndexThreshold_terminal_wellus_unitType()) >=
                        IE_GLOBAL_DATA.getThreshold_TW())
                termin++;
            else
                well++;


        }
    }

    qreal square = computeSquare()/(IE_GLOBAL_DATA.getMeasureIndex()*IE_GLOBAL_DATA.getMeasureIndex());
    qreal totalHairInCm2 = round(i/square*100.0);
    qreal percentTermin = termin*1.0/i*100.0;
    qreal terminHairInCm2 = round(termin/square*100.0);
    qreal percentWellus = well*1.0/i*100.0;
    qreal wellusHairInCm2 = round(well/square*100.0);



    QChart *chart = new QChart();
    QChartView *chartView = new QChartView(chart);

    QBarSet *set0 = new QBarSet("Всего волос");
    *set0 << totalHairInCm2;
    QBarSet *set1 = new QBarSet("Терминальных волос");
    *set1 << terminHairInCm2;
    QBarSet *set2 = new QBarSet("Веллус");
    *set2 << wellusHairInCm2;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);

    series->setLabelsFormat("@value");
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setLabelsVisible();

    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chartView->setRenderHint(QPainter::Antialiasing);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,300);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);



    QCategoryAxis *axisY3 = new QCategoryAxis;
    axisY3->append(" ", 190);
    axisY3->append("Норма", 230);
    axisY3->append(" ", 300);
    axisY3->setShadesPen(Qt::NoPen);
    axisY3->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 0x55)));
    axisY3->setShadesVisible(true);

    chart->addAxis(axisY3, Qt::AlignRight);
    series->attachAxis(axisY3);

    QWidget* pTmp = new QWidget();
    pTmp->setWindowFlag(Qt::Tool);
    pTmp->setWindowTitle("[Отчет] Плотность волос");
    QVBoxLayout *pVertBoxLayout = new QVBoxLayout(pTmp);
    pVertBoxLayout->addWidget(chartView);


//    length->setNum(0);
//    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(this)






    QLabel *pLabel;
//    pLabel->setNum(i);
    pLabel = new QLabel(QString("Всего измеренно волос: %1. Это %2 на см.кв.").arg(i).arg(totalHairInCm2), pTmp);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Из них терминальных волос: %1. Это %2 на см.кв. или %3 %").arg(termin).
                                                                                            arg(terminHairInCm2).
                                                                                            arg(percentTermin), pTmp);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Веллус: %1. Это %2 на см.кв. или %3 %"). arg(well).arg(wellusHairInCm2).
                                                                                            arg(percentWellus), pTmp);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Площадь изображения: %1 %2.кв.").arg(square).arg(UnitTypeTitle[IE_GLOBAL_DATA.getUnitType()]), pTmp);
    pVertBoxLayout->addWidget(pLabel);

    pTmp->setMinimumWidth(500);
    pTmp->setMinimumHeight(500);
    pTmp->show();
}

qreal       TSPImageEditorModel::computeSquare()
{
    qreal sq = 1;
    for (QList<IE_ModelLayer *>::iterator tmpIter = layersList.begin();
         tmpIter!=layersList.end();tmpIter++)
    {
        if( tmpIter.i->t()->getToolType() == ToolType::MainImage)
            sq = tmpIter.i->t()->boundingRect().width() * tmpIter.i->t()->boundingRect().height();
        else if( tmpIter.i->t()->getToolType() == ToolType::ComputingArea )
        {
            sq = tmpIter.i->t()->boundingRect().width() * tmpIter.i->t()->boundingRect().height();
            break;
        }
    }
    return sq;
}

void        TSPImageEditorModel::setInputArgs()
{
    QPushButton *ppbSave = new QPushButton();
    ppbSave->setText("Сохранить");

    QWidget* pTmp = new QWidget();
    pTmp->setMinimumWidth(300);
    pTmp->setMinimumHeight(300);
    //pTmp->setMaximumSize(QSize(300,300));
    pTmp->setWindowTitle("Настройки вычислений");
    pTmp->setWindowFlags(Qt::Tool);
    QVBoxLayout *pVertBoxLayout = new QVBoxLayout(pTmp);

//    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(this);
    QLabel *pLabel = new QLabel("Единицы измерения:");

    QComboBox    *pcbo = new QComboBox();
    pLabel->setBuddy(pcbo);
    QStringList  lst;

    lst << UnitTypeTitle[0] << UnitTypeTitle[1] << UnitTypeTitle[2] << UnitTypeTitle[3] << UnitTypeTitle[4];
    pcbo->addItems(lst);
    pcbo->setCurrentIndex(IE_GLOBAL_DATA.getUnitType());
    pVertBoxLayout->addWidget(pLabel);
    pVertBoxLayout->addWidget(pcbo);

    connect(ppbSave, &QPushButton::released, [this, pcbo]()
    {
        IE_GLOBAL_DATA.setIndexUnitType(pcbo->currentIndex(), globalDataKey);

    });

    pLabel = new QLabel("Порог для Терминальных/Веллусных:");


    QSpinBox *pSpinBox = new QSpinBox(pTmp);
    pLabel->setBuddy(pSpinBox);
    //pSpinBox->setEnabled(false);
    pSpinBox->setRange(1,100);
    pSpinBox->setValue(IE_GLOBAL_DATA.getThreshold_TW());
    pSpinBox->setWrapping(true); // циклический режим
    pSpinBox->setButtonSymbols(QSpinBox::PlusMinus);
    pLabel->setBuddy(pSpinBox);
    QHBoxLayout *phbl = new QHBoxLayout(pTmp);
    phbl->addWidget(pSpinBox,3);

    pcbo = new QComboBox(pTmp);
    pcbo->addItems(lst);
    pcbo->setCurrentIndex(IE_GLOBAL_DATA.getIndexThreshold_terminal_wellus_unitType());
    phbl->addWidget(pcbo,1);
    pVertBoxLayout->addWidget(pLabel);
    pVertBoxLayout->addItem(phbl);

    connect(ppbSave, &QPushButton::released, [this, pSpinBox, pcbo]()
    {
        IE_GLOBAL_DATA.setThreshold_TW(pSpinBox->value(), globalDataKey);
        IE_GLOBAL_DATA.setThreshold_terminal_wellus_unitType(pcbo->currentIndex(), globalDataKey);

    });


    pVertBoxLayout->addStretch(1);
    pVertBoxLayout->addWidget(ppbSave);
    pTmp->setLayout(pVertBoxLayout);

    pTmp->show();


}




// ------- Layer manipulations

QDockWidget*TSPImageEditorModel::initLayersDock()
{
    pDockLayers = new QDockWidget("Слои");
    pDockLayersTableView = new QTableView();
    pDockLayersTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(pDockLayersTableView, &QTableView::customContextMenuRequested,
            [this](const QPoint &pos){
        QMenu contextMenu("Context menu");

        int row = pDockLayersTableView->rowAt(pos.y()),
                col = pDockLayersTableView->columnAt(pos.x());
        if(row==-1 || col==-1)
            return;

           QAction action1("Удалить слой");
           connect(&action1, &QAction::triggered, [this, row](){
                this->eraseLayer(row);
           });
           contextMenu.addAction(&action1);

           bool visible = layersList.at(row)->isVisible();

           QAction action2;

           if(visible)
               action2.setText("Скрыть слой");
           else
               action2.setText("Отобразить слой");
           connect(&action2, &QAction::triggered, [this, row, visible](){
                if(visible)
                    this->hideLayer(row);
                else {
                    this->showLayer(row);
                }
           });
           contextMenu.addAction(&action2);


           contextMenu.exec(pDockLayersTableView->mapToGlobal(pos));
    });

    pDockLayersTableModel = new IE_LayersTableModel();
    pDockLayersTableModel->setPLayersList(&layersList);
    pDockLayersTableView->setModel(pDockLayersTableModel);

    //layersDockController();
//    connect(this, &TSPImageEditorModel::changed, pDockLayersTableModel, &IE_LayersTableModel::toChange);
    connect(this, &TSPImageEditorModel::changed, [this](){
        delete pDockLayersTableModel;
//        pToolCnt->resetEditingMode();
        pDockLayersTableModel = new IE_LayersTableModel();
        pDockLayersTableModel->setPLayersList(&layersList);
        pDockLayersTableView->setModel(pDockLayersTableModel);
        connect(pDockLayersTableView, &QTableView::clicked,
                pDockLayersTableModel, &IE_LayersTableModel::onTableClicked);
    });

    connect(pDockLayersTableView, &QTableView::clicked,
            pDockLayersTableModel, &IE_LayersTableModel::onTableClicked);
    connect(pDockLayersTableView, &QTableView::clicked, [this](const QModelIndex &tmp){
        if(tmp.isValid())
            pToolCnt->setPActiveToolForEditing(layersList.at(tmp.row()));
        else
            pToolCnt->resetEditingMode();
    });
    pDockLayers->setWidget(pDockLayersTableView);
    return pDockLayers;
}

void        TSPImageEditorModel::addLayerViaToolCnt()
{
    if(pToolCnt)
    {

        addLayer(pToolCnt->getPActiveTool());
    }
}

void        TSPImageEditorModel::addLayer(ToolType toolType, QGraphicsItem *item)
{
//    addItem(item);
//    IE_ModelLayer *layerData = new IE_ModelLayer(toolType, item);
//    layersList.append(layerData);
}

void        TSPImageEditorModel::addLayer(IE_ModelLayer* layerToAdd)
{
    addItem(layerToAdd->parentItem());
    layersList.append(layerToAdd);
}

void        TSPImageEditorModel::showLayer(int listIndex)
{
    IE_ModelLayer* foundLayer = getLayerByListIndex(listIndex);
    if(!foundLayer)
    {
        qDebug() << "TSPImageEditorModel::showLayer(int listIndex) >> getLayerByListIndex returns nullptr.";
        return;
    }

    foundLayer->unhide();
}
void        TSPImageEditorModel::hideLayer(int listIndex)
{
    IE_ModelLayer* foundLayer = getLayerByListIndex(listIndex);
    if(!foundLayer)
    {
        qDebug() << "TSPImageEditorModel::hideLayer(int listIndex) >> getLayerByListIndex returns nullptr.";
        return;
    }

    foundLayer->hide();
}

// !!!!!! ------- TODO:
void        TSPImageEditorModel::layersController()
{
//    static QList<QGraphicsItem*> itemsList;
//    itemsList = items(Qt::AscendingOrder);

//    for (QListIterator<QGraphicsItem*> listIter(itemsList);listIter.hasNext();)
//    {
//        static QGraphicsItem* current;
//        current = listIter.next();

//        if(layersMap.find(current) == layersMap.end() ) // удалить
//        {
//            LayersData *newLayer = new LayersData();
////            newLayer->
////            layersList.append()
//        }
//        else // добавить
//        {

//        }
//    }

}

void        TSPImageEditorModel::eraseLayer(int listIndex)
{
    QList<IE_ModelLayer*>::iterator iterResult = getLayerIteratorByListIndex(listIndex);
    if(iterResult == layersList.end())
        return;
    eraseLayer(iterResult);
}
void        TSPImageEditorModel::eraseLayer(QList<IE_ModelLayer*>::iterator iter)
{
    QGraphicsItem *pGraphicsItem = *iter;

    if((*iter)->getToolType() == ToolType::MainImage)
        return;

    for (QList<IE_ModelLayer *>::iterator tmpIter = layersList.begin();
         tmpIter!=layersList.end();tmpIter++)
    {

        if(*iter == *tmpIter)
        {
            pGraphicsItem = (QGraphicsItem*) *tmpIter;
            layersList.erase(tmpIter);
            break;
        }
    }

    this->removeItem(pGraphicsItem->parentItem());
    delete pGraphicsItem;
    pGraphicsItem = nullptr;
}

// !!!!!! ------- TODO:
void        TSPImageEditorModel::selectedLayer(int row)
{

}

// ------- END Layer manipulations ------- //

// !!!!!! ------- TODO:
void        TSPImageEditorModel::save(QString modelFilePath)
{

}
// !!!!!! ------- TODO:
void        TSPImageEditorModel::close(QString modelFilePath)
{
    static QList<QGraphicsItem*> itemsList;
    itemsList = items(Qt::AscendingOrder);
    for (QListIterator<QGraphicsItem*> listIter(itemsList);listIter.hasNext();)
    {
        static QGraphicsItem* current = listIter.next();
    }
}



