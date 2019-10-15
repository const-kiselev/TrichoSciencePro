#include "ie_model.h"

            IE_Model::IE_Model    ()
{
    // ключ к глобальному объекту с информацией о масштабе итд
    qsrand(100000);
    globalDataKey = qrand();
    __global_data = new _global_ie(globalDataKey);





    connect(this, &IE_Model::changed, this,
            &IE_Model::layersController);

}

            IE_Model::~IE_Model   ()
{
    if(_modelData.tmpDir.exists())
        _modelData.tmpDir.removeRecursively();
//    QGraphicsScene::~QGraphicsScene();
}

int         IE_Model::saveModel              ()
{
    enum SaveFormat
    {Json, Bin}saveFormat;
    saveFormat = SaveFormat::Json;
    if (!_modelData.modelDir.exists()) {
        _modelData.modelDir.mkpath(".");
    }

    _modelData.modelDir.mkpath("./res");


    QString filePAth = QString("%1/%2%3.%4").arg(_modelData.modelDir.path()).
                                        arg(_modelData.model_ID).
                                        arg(IE_MODEL_FILE_NAME_POSTFIX).
                                        arg(IE_MODEL_FILE_NAME_EXTENSION);


    QFile saveFile(saveFormat == Json
            ? filePAth
            : QStringLiteral("model.dat"));

        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
            return 1;
        }

        QJsonObject jsonObj;
        jsonObj["TSP_docType"] = "TSP_JSON_IE_model";
        write(jsonObj);
        QJsonDocument saveDoc(jsonObj);
        saveFile.write(saveFormat == Json
            ? saveDoc.toJson()
            : saveDoc.toBinaryData());
        saveFile.close();
        return 0;
}

int         IE_Model::saveModelAsImage       ()
{

}

int         IE_Model::read                   (const QJsonObject &json)
{
    _modelData.read(json);

    __global_data->setMeasureIndex(json["measureIndex"].toDouble(),globalDataKey);
    //! \todo Добавить пороги!
    QJsonArray layersArray = json["layers"].toArray();
    for (int layerIndex = 0; layerIndex < layersArray.size(); ++layerIndex) {
            QJsonObject layerObject = layersArray[layerIndex].toObject();
            IE_ModelLayer* pLayer = nullptr;

            {
                QGraphicsItem* pParentItem = nullptr;
                switch (convertToolTitleToToolType(layerObject["typeTitle"].toString()))
                {
                case ToolType::MainImage:
                {
                    pParentItem = new IE_Tool_Image();
                    dynamic_cast<IE_Tool_Image*>(pParentItem)->setDirs(QString("%1/%2")
                                                                            .arg(_modelData.modelDir.path())
                                                                            .arg(_modelData.resDir.path()),
                                                                       _modelData.modelDir.path(),
                                                                       _modelData.tmpDir.path()
                                                                       );
                    break;
                }
                case ToolType::DensityAndDiameter:
                {
                    pParentItem = new IE_Line_DD(__global_data);
                    break;
                }
                case ToolType::Ruler:
                {
                    pParentItem = new IERuler(__global_data);
                    break;
                }
                case ToolType::SimpleLine:
                {
                    pParentItem = new IELine(__global_data);
                    break;
                }
                case ToolType::Marker_FollicularUnit:
                {
                    pParentItem = new IE_Tool_FollicularUnit();
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
                    pParentItem = new IE_Tool_Marker();
                    break;
                }
                }
                pLayer = new IE_ModelLayer(pParentItem);
            }
            pLayer->read(layerObject);
            addLayer(pLayer);/*
            if(convertToolTitleToToolType(layerObject["typeTitle"].toString()) == ToolType::MainImage)
            {
                pMainImageLayer = pLayer;
                imageRect = pLayer->boundingRect();
            }*/
        }
    return 0;
}

int         IE_Model::write                  (QJsonObject &json) const
{
    _modelData.write(json);
    json["measureIndex"] = __global_data->getMeasureIndex();
    //! \todo Добавить пороги!
    QJsonArray layerArray;
    foreach(IE_ModelLayer* layer, layersList)
    {
        QJsonObject layerObject;
        layer->write(layerObject);
        layerArray.append(layerObject);
    }
    json["layers"] = layerArray;
    return 0;
}

int         IE_Model::initAsNewModel         (_Model_patientData patientData)
{
    _modelData.initNew(patientData);

    if (!_modelData.tmpDir.exists()) {
        _modelData.tmpDir.mkpath(".");
    }
    _modelData.tmpDir.mkpath("./res");

    if(makeDialogForSetupModelAsNew() != QDialog::Accepted)
    {
        qDebug() << "makeDialogForSetupModelAsNew() was rejected.";
        return 1;
    }

    if( m_pFieldOfViewCnt->makeDialogForSetupAsNew() != QDialog::Accepted)
    {
        qDebug() << "m_pFieldOfViewCnt->makeDialogForSetupAsNew() was rejected.";
        return 1;
    }


    //! \todo загрузить !!!!!!

    //setMainImage("");
}

/*void        TSPImageEditorModel::initWithModel(QString modelFilePath)
{

}*/

int         IE_Model::initWithModel          (_Model_patientData patientData)
{

    if(patientData.modelPath == "Empty")
    {
        patientData.modelPath = QFileDialog::getOpenFileName(nullptr,
                                                             "Выбор модели изображения",
                                                             QStandardPaths::displayName(
                                                                 QStandardPaths::HomeLocation ) );

    }

    QFile modelFile;
    modelFile.setFileName(patientData.modelPath);
    if(!modelFile.exists())
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Файл %1 не существует.").arg(patientData.modelPath));
        return 1;
    }
    if(!modelFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Файл %1 не удается прочитать.").arg(patientData.modelPath));
        qWarning("Couldn't open file.");
        return 1;
    }

    QByteArray saveData = modelFile.readAll();
    modelFile.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject jsonObj = loadDoc.object();

    if(jsonObj["TSP_docType"] != "TSP_JSON_IE_model")
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Файл %1 не является моделью изображения.").arg(patientData.modelPath));
        qWarning() << modelFile.fileName() << "isn't TSP_JSON_IE_model.";
//        patientBaseFile.rename("data/patients/!!!patientBase_ERROR.json");
        return 1;
    }

    _modelData.init(patientData);
    read(jsonObj);
    return 0;
}

// ------- GETTERS and SETTERS

QRectF      IE_Model::getImageRect           () const
{
    return imageRect;
}

/*int         IE_Model::setMainImage           (QString imageFilePath)
{
    IE_Tool_Image *pToolImage_mainImage = new IE_Tool_Image(QString("%1/%2").arg(_modelData.modelDir.path())
                                                                            .arg(_modelData.resDir.path()),
                                                            _modelData.modelDir.path(),
                                                            QString("%1/%2").arg(_modelData.tmpDir.path())
                                                                            .arg(IE_MODEL_RES_DIR_NAME),
                                                            imageFilePath,
                                                            ToolType::MainImage
                                                            );
    pToolImage_mainImage->setPos(0,0);

    pMainImageLayer = new IE_ModelLayer(ToolType::MainImage, pToolImage_mainImage);

    imageRect = pToolImage_mainImage->boundingRect();

    for (QList<IE_ModelLayer*>::iterator iter = layersList.begin();iter!=layersList.end();iter++)
        if((*iter)->getToolType() == ToolType::MainImage)
            iter = layersList.erase(iter);

    addLayer(pMainImageLayer);

    return 1;
}*/


qreal       IE_Model::getMeasureIndex        () const
{
    return __global_data->getMeasureIndex();
}
void        IE_Model::setMeasureIndex        (const qreal &value)
{
    __global_data->setMeasureIndex(value, globalDataKey);
    __global_data->setUnitType(UnitType::mm, globalDataKey);
    emit(measureIndexChanged(value));
}
ToolsController *
            IE_Model::getPToolCnt            () const
{
    return pToolCnt;
}
void        IE_Model::setPToolCnt            (ToolsController *value)
{
    pToolCnt = value;
}
QDockWidget*IE_Model::getPDockLayers         () const
{
    return pDockLayers;
}
QList<IE_ModelLayer*>::const_iterator
            IE_Model::getLayersListIter      () const
{
    if(layersList.isEmpty())
        return layersList.end();
    return layersList.begin();
}

QList<IE_ModelLayer *>
            IE_Model::getLayersList          () const
{
    return layersList;
}


_Model_patientData
            IE_Model::get_Model_patientData()
{
    return _modelData.to_Model_patientData();
}
IE_ModelLayer *
            IE_Model::getLayerByListIndex    (int listIndex)
{
    QList<IE_ModelLayer*>::iterator iterResult = getLayerIteratorByListIndex(listIndex);
    return iterResult == layersList.end() ? nullptr : *iterResult;
}
QList<IE_ModelLayer*>::iterator
            IE_Model::getLayerIteratorByListIndex
                                                        (int listIndex)
{
    if(listIndex<0 || listIndex >= layersList.count())
        return layersList.end();

    QList<IE_ModelLayer*>::iterator iter = layersList.begin();
    for (int i=0;i<listIndex;i++, iter++);
    return iter;
}

// ------- END GETTERS and SETTERS

/// \todo !!!!!! ------- :
QDockWidget*IE_Model::initInfoDock           ()
{
}
QDockWidget*IE_Model::initToolInfoDock       ()
{
    return pToolCnt->initInfoDock();
}

void        IE_Model::makeHairDensityComputeWithWidget
                                                        ()
{
    //! \bug Утечка памяти!
    IE_Report * pReport = new IE_Report(__global_data);
    IE_Compute comp(__global_data,&layersList);

    pReport->makeHairDensityReport_dialog(comp.compute(IE_Compute::ComputeType::HairDensity));
}

void        IE_Model::makeHairDiameterComputeWithWidget
                                                        ()
{
    //! \bug Утечка памяти!
    IE_Report * pReport = new IE_Report(__global_data);
    IE_Compute comp(__global_data,&layersList);

    pReport->makeHairDiameterReport_dialog(comp.compute(IE_Compute::ComputeType::HairDiameter));
}



qreal       IE_Model::computeSquare          ()
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

void        IE_Model::setInputArgs           ()
{
    //! \bug утечка данных
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
    pcbo->setCurrentIndex(__global_data->getUnitType());
    pVertBoxLayout->addWidget(pLabel);
    pVertBoxLayout->addWidget(pcbo);

    connect(ppbSave, &QPushButton::released, [this, pcbo]()
    {
        __global_data->setIndexUnitType(pcbo->currentIndex(), globalDataKey);
    });



    pLabel = new QLabel("Порог для Терминальных/Веллусных:");

    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(pTmp);
    QLineEdit *pLineEdit = new QLineEdit(pTmp);
    pLabel->setBuddy(pLineEdit);
    //pSpinBox->setEnabled(false);
    pLineEdit->setText(QString().number(__global_data->getThreshold_TW()));

    pVertBoxLayout->addWidget(pLabel);
    pHorBoxLayout->addWidget(pLineEdit);

    QLabel *chosenUnit = new QLabel();
    chosenUnit->setText(pcbo->currentText());
    connect(pcbo, &QComboBox::currentTextChanged, chosenUnit, &QLabel::setText);

    pHorBoxLayout->addWidget(chosenUnit);
    pVertBoxLayout->addLayout(pHorBoxLayout);



    connect(ppbSave, &QPushButton::released, [this, pLineEdit]()
    {
        __global_data->setThreshold_TW(QString(pLineEdit->text()).toDouble(), globalDataKey);
    });


    pLabel = new QLabel("Верхний порог для <b>тонкого</b> волоса:");
    pLabel->setTextFormat( Qt::RichText );

    pHorBoxLayout = new QHBoxLayout(pTmp);
    pLineEdit = new QLineEdit(pTmp);
    pLabel->setBuddy(pLineEdit);
    pLineEdit->setText(QString().number(__global_data->getThreshold_thinHair()));

    pVertBoxLayout->addWidget(pLabel);
    pHorBoxLayout->addWidget(pLineEdit);

    chosenUnit = new QLabel();
    chosenUnit->setText(pcbo->currentText());
    connect(pcbo, &QComboBox::currentTextChanged, chosenUnit, &QLabel::setText);
    pHorBoxLayout->addWidget(chosenUnit);
    pVertBoxLayout->addLayout(pHorBoxLayout);

    connect(ppbSave, &QPushButton::released, [this, pLineEdit]()
    {
        __global_data->setThreshold_thinHair(QString(pLineEdit->text()).toDouble(), globalDataKey);
    });

    pLabel = new QLabel("Верхний порог для <b>среднего</b> волоса:");
    pLabel->setTextFormat( Qt::RichText );

    pHorBoxLayout = new QHBoxLayout(pTmp);
    pLineEdit = new QLineEdit(pTmp);
    pLabel->setBuddy(pLineEdit);
    pLineEdit->setText(QString().number(__global_data->getThreshold_mediumHair()));

    pVertBoxLayout->addWidget(pLabel);
    pHorBoxLayout->addWidget(pLineEdit);

    chosenUnit = new QLabel();
    chosenUnit->setText(pcbo->currentText());
    connect(pcbo, &QComboBox::currentTextChanged, chosenUnit, &QLabel::setText);
    pHorBoxLayout->addWidget(chosenUnit);
    pVertBoxLayout->addLayout(pHorBoxLayout);

    connect(ppbSave, &QPushButton::released, [this, pLineEdit]()
    {
        __global_data->setThreshold_mediumHair(QString(pLineEdit->text()).toDouble(), globalDataKey);
    });


    pVertBoxLayout->addStretch(1);
    pVertBoxLayout->addWidget(ppbSave);
    pTmp->setLayout(pVertBoxLayout);

    pTmp->show();
    pTmp->raise();
    pTmp->activateWindow();


}




// ------- Layer manipulations

QDockWidget*IE_Model::initLayersDock         ()
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
        /// \bug : при удалении слоя все вылетает, когда нажимаешь на другой слой
//           QAction action1("Удалить слой");
//           connect(&action1, &QAction::triggered, [this, row](){
//                this->eraseLayer(row);
//           });
//           contextMenu.addAction(&action1);

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
    connect(this, &IE_Model::changed, [this](){
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

void        IE_Model::addLayerViaToolCnt     ()
{
    if(pToolCnt)
    {

        addLayer(pToolCnt->getPActiveTool());
    }
}

QDialog::DialogCode IE_Model::makeDialogForSetupModelAsNew()
{
    QDialog * locPDialog = new QDialog();
    QVBoxLayout * pMainLayout = new QVBoxLayout(locPDialog);
    pMainLayout->addWidget(new QLabel("Первичные настройки для создания нового обследования:",locPDialog));
    QComboBox    *pcboProfile;
    if(_modelData.profile == IE_ProfileType::None)
    {
        QLabel *pLabel = new QLabel("Тип обследования:", locPDialog);

        pcboProfile = new QComboBox(locPDialog);
        pLabel->setBuddy(pcboProfile);
        QStringList  lst;

        lst << "Трихограмма" << "Трихоскопия" << "Фототрихограмма";
        pcboProfile->addItems(lst);
        pcboProfile->setCurrentIndex( IE_FieldOfView_Controller::getStandartQuantity(_modelData.profile)  );
        pMainLayout->addWidget(pLabel);
        pMainLayout->addWidget(pcboProfile);

    }

    QLabel *pLabel = new QLabel("Количество полей зрения:", locPDialog);

    QComboBox    *pcboQuiantityFV = new QComboBox(locPDialog);
    pLabel->setBuddy(locPDialog);
    QStringList  lst;

    lst << QString().number(IE_FieldOfView_Controller::Quantity::One)
        << QString().number(IE_FieldOfView_Controller::Quantity::Two)
        << QString().number(IE_FieldOfView_Controller::Quantity::Three)
        << QString().number(IE_FieldOfView_Controller::Quantity::Four)
        << QString().number(IE_FieldOfView_Controller::Quantity::Six);
    pcboQuiantityFV->addItems(lst);
    pcboQuiantityFV->setCurrentIndex( IE_FieldOfView_Controller::getStandartQuantity(_modelData.profile)  );

    pMainLayout->addWidget(pLabel);
    pMainLayout->addWidget(pcboQuiantityFV);


    locPDialog->setLayout(pMainLayout);
    locPDialog->setModal(true);

    locPDialog->show();
    int answer =  locPDialog->exec();

    if(answer == QDialog::Accepted)
    {
        switch (pcboProfile->currentIndex()) {
        case (IE_ProfileType::Trichogram + 1):
        {
            _modelData.profile = IE_ProfileType::Trichogram;
            break;
        }
        case (IE_ProfileType::Trichoscopy + 1):
        {
            _modelData.profile = IE_ProfileType::Trichoscopy;
            break;
        }
        case (IE_ProfileType::Phototrichogram + 1):
        {
            _modelData.profile = IE_ProfileType::Phototrichogram;
            break;
        }
        }
        m_pFieldOfViewCnt->init((IE_FieldOfView_Controller::Quantity)pcboQuiantityFV->currentText().toInt());
    }
    delete locPDialog;
    return (QDialog::DialogCode)answer;
}

_global_ie *IE_Model::getPGlobal_data() const
{
    return __global_data;
}

void        IE_Model::addLayer               (IE_ModelLayer* layerToAdd)
{

    for (QList<IE_ModelLayer*>::iterator iter = layersList.begin();iter!=layersList.end();iter++)
        if((*iter) == layerToAdd)
        {
            qWarning() << "Tring to add existed layer.";
            return;
        }

    addItem(layerToAdd->parentItem());
    layersList.append(layerToAdd);
}

void        IE_Model::showLayer              (int listIndex)
{
    IE_ModelLayer* foundLayer = getLayerByListIndex     (listIndex);
    if(!foundLayer)
    {
        qDebug() << "TSPImageEditorModel::showLayer(int listIndex) >> getLayerByListIndex returns nullptr.";
        return;
    }

    foundLayer->unhide();
}
void        IE_Model::hideLayer              (int listIndex)
{
    IE_ModelLayer* foundLayer = getLayerByListIndex(listIndex);
    if(!foundLayer)
    {
        qDebug() << "TSPImageEditorModel::hideLayer(int listIndex) >> getLayerByListIndex returns nullptr.";
        return;
    }

    foundLayer->hide();
}

/// \todo !!!!!! ------- :
void        IE_Model::layersController       ()
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

void        IE_Model::eraseLayer             (int listIndex)
{
    QList<IE_ModelLayer*>::iterator iterResult = getLayerIteratorByListIndex(listIndex);
    if(iterResult == layersList.end())
        return;
    eraseLayer(iterResult);
}
void        IE_Model::eraseLayer             (QList<IE_ModelLayer*>::iterator iter)
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
            this->removeItem(pGraphicsItem->parentItem());
            delete pGraphicsItem;
            pGraphicsItem = nullptr;
            break;
        }
    }


}

/// \todo !!!!!! ------- :
void        IE_Model::selectedLayer          (int row)
{

}

// ------- END Layer manipulations ------- //

/// \todo !!!!!! ------- :
void        IE_Model::save                   (QString modelFilePath)
{

}
/// \todo !!!!!! ------- :
void        IE_Model::close                  (QString modelFilePath)
{
//    static QList<QGraphicsItem*> itemsList;
//    itemsList = items(Qt::AscendingOrder);
//    for (QListIterator<QGraphicsItem*> listIter(itemsList);listIter.hasNext();)
//    {
//        static QGraphicsItem* current = listIter.next();
//    }
}



