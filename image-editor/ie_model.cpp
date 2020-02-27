#include "ie_model.h"

            IE_Model::IE_Model    ()
{
    // ключ к глобальному объекту с информацией о масштабе итд
    qsrand(100000);
    m_ieGlobalData = new _global_ie();
    m_pFieldOfViewCnt = new IE_FieldOfView_Controller(m_modelLayerCnt.getPublicConstPtrToList(),
                                                      m_ieGlobalData
                                                      );

    m_modelLayerCnt.m_pGScene = this;

    initFieldOfViewControllerInfoDock();
//    initLayersDock();



    connect(&_modelData, &IE_ModelData::updated, [this]()
    {
        m_ieGlobalData->setModelDirPath(_modelData.getModelDir().path());
        m_ieGlobalData->setModelResDirPath(QString("%1/%2")
                                          .arg(_modelData.getModelDir().path())
                                          .arg(_modelData.getResDir().path())
                                          );
        m_ieGlobalData->setTmp_modelDirPath(_modelData.getTmpDir().path());
        m_ieGlobalData->setTmp_modelResDirPath(QString("%1/%2")
                                              .arg(_modelData.getTmpDir().path())
                                              .arg(_modelData.getResDir().path())
                                              );
    });
    connect(m_pFieldOfViewCnt, &IE_FieldOfView_Controller::addNewLayer, this, &IE_Model::addLayer);
    connect(m_pFieldOfViewCnt, &IE_FieldOfView_Controller::boundingRectWasChanged, [this](QRectF rectF)
    {
        modelRect = rectF;
        emit boundingRectWasChanged(modelRect);
        pToolCnt->resetPActiveTool();

//        setSceneRect(modelRect);
    });
    connect(m_pFieldOfViewCnt, &IE_FieldOfView_Controller::layerAction,this, &IE_Model::layerAction);
    connect(this, &IE_Model::layerListWasChanged, m_pFieldOfViewCnt, &IE_FieldOfView_Controller::checkLayerList);

    // соединения для IE_ModelLayer_Controller

    connect(&m_modelLayerCnt,
            &IE_ModelLayer_Controller::layerListWasChanged,
            this,
            &IE_Model::layerListWasChanged
            );
}

            IE_Model::~IE_Model   ()
{
    if(_modelData.getTmpDir().exists() && _modelData.getTmpDir() != _modelData.getModelDir())
        _modelData.getTmpDir().removeRecursively();
//    QGraphicsScene::~QGraphicsScene();
}

int         IE_Model::saveModel              ()
{
    enum SaveFormat
    {Json, Bin}saveFormat;
    saveFormat = SaveFormat::Json;
    if (!_modelData.getModelDir().exists()) {
        _modelData.getModelDir().mkpath(".");
    }

    _modelData.getModelDir().mkpath("./res");


    QString filePAth = QString("%1/%2%3.%4").arg(_modelData.getModelDir().path()).
                                        arg(_modelData.getModel_ID()).
                                        arg(IE_MODEL_FILE_NAME_POSTFIX).
                                        arg(IE_MODEL_FILE_NAME_EXTENSION);


    QFile saveFile(saveFormat == Json
            ? filePAth
            : QStringLiteral("model.dat"));

        if (!saveFile.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось сохранить модель, так как невозможно создать файл для записи %1 .").arg( filePAth));
            qWarning("Couldn't open file for writing.");
            return 1;
        }
        _modelData.setSaveDateTime(QDateTime::currentDateTime());
        QJsonObject jsonObj;
        jsonObj["TSP_docType"] = "TSP_JSON_IE_model";
        write(jsonObj);
        QJsonDocument saveDoc(jsonObj);
        saveFile.write(saveFormat == Json
            ? saveDoc.toJson()
            : saveDoc.toBinaryData());
        saveFile.close();
        emit wasSaved();
        return 0;
}

int         IE_Model::saveModelAsImage       ()
{

}

int         IE_Model::read                   (const QJsonObject &json)
{
    _modelData.read(json);

    m_ieGlobalData->setMeasureIndex(json["measureIndex"].toDouble());
    //! \todo Добавить пороги!
    QJsonArray layerArray = json["layerArray"].toArray();
    for (int layerIndex = 0; layerIndex < layerArray.size(); ++layerIndex) {
            QJsonObject layerObject = layerArray[layerIndex].toObject();
            IE_ModelLayer* pLayer = nullptr;

            {
                QGraphicsItem* pParentItem = nullptr;
                switch (convertToolTitleToToolType(layerObject["typeTitle"].toString()))
                {
                case ToolType::MainImage:
                {
                    pParentItem = new IE_Tool_Image();
                    dynamic_cast<IE_Tool_Image*>(pParentItem)->setDirs(m_ieGlobalData->getModelResDirPath(),
                                                                       m_ieGlobalData->getModelDirPath(),
                                                                       m_ieGlobalData->getTmp_modelResDirPath()
                                                                       );
                    break;
                }
                case ToolType::DensityAndDiameter:
                {
                    pParentItem = new IE_Line_DD(m_ieGlobalData);
                    break;
                }
                case ToolType::Ruler:
                {
                    pParentItem = new IERuler(m_ieGlobalData);
                    break;
                }
                case ToolType::SimpleLine:
                {
                    pParentItem = new IE_Tool_Line(m_ieGlobalData);
                    break;
                }
                case ToolType::Marker_FollicularUnit:
                {
                    pParentItem = new IE_Tool_FollicularUnit();
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

    m_pFieldOfViewCnt->read(json);

    if(json.contains("relatedModelsArray"))
    {
        m_relatedModelList.clear();
        QJsonArray relatedModelsArray = json["relatedModelsArray"].toArray();
        for (int i=0; i < relatedModelsArray.size(); i++)
            m_relatedModelList.push_back(QString("%1/%2").arg(_modelData.getModelDir().path()).arg(relatedModelsArray.at(i).toString()) );
    }

    return 0;
}

int         IE_Model::write                  (QJsonObject &json) const
{
    _modelData.write(json);
    json["measureIndex"] = m_ieGlobalData->getMeasureIndex();
    //! \todo Добавить пороги!
    QJsonArray layerArray,  relatedModelsArray;
    for(IE_ModelLayer::PublicList::const_iterator iter = m_modelLayerCnt.m_constLayerList.begin();
        iter!=m_modelLayerCnt.m_constLayerList.end();
        iter++
        ){
        QJsonObject layerObject;

        IE_ModelLayer* pLayer = const_cast<IE_ModelLayer*>(iter.i->t());

        pLayer->write(layerObject);
        layerArray.append(layerObject);
    }


//    foreach(IE_ModelLayer* layer, layersList)
//    {
//        QJsonObject layerObject;
//        layer->write(layerObject);
//        layerArray.append(layerObject);
//    }
    json["layerArray"] = layerArray;
    if(!m_relatedModelList.isEmpty())
    {
        foreach(QString str, m_relatedModelList)
            relatedModelsArray.push_back(QJsonValue(str.replace(_modelData.getModelDir().path()+"/","")));
        json["relatedModelsArray"] = relatedModelsArray;
    }
    m_pFieldOfViewCnt->write(json);
    return 0;
}

int         IE_Model::initAsNewModel         (TSP_PatientData patientData,
                                              IEM_type iem_type,
                                              IE_ProfileType ie_type,
                                              bool dialog
                                              )
{
    _modelData.initNew(patientData);
    _modelData.setIem_type(iem_type);
    _modelData.setProfile(ie_type);

    switch (iem_type)
    {
    case IEM_type::HairDencity:
    {
        pToolCnt->setToolSetType(ToolSet::HairDencity);
        break;
    }
    case IEM_type::TrichoscopyPatterns:
    {
        m_pFieldOfViewCnt->initImageBase();
        pToolCnt->setToolSetType(ToolSet::TrichoscopyPatterns);
        break;
    }
    case IEM_type::AssessmentOfScalp:
    case IEM_type::AssessmentOfHairRoots:
    case IEM_type::AssessmentOfHairRods:
    case IEM_type::DermatoscopyOfNeoplasms:
    {

        pToolCnt->setToolSetType(ToolSet::Simple);
        break;
    }

    }

    switch (ie_type)
    {
    case IE_ProfileType::Trichogram:
        break;
    case IE_ProfileType::Trichoscopy:
        break;
    case IE_ProfileType::Phototrichogram:
        break;
    case IE_ProfileType::Simple:
        break;
    case IE_ProfileType::MeasureIndex:
        pToolCnt->setToolSetType(ToolSet::MeasureIndex);
        break;
    case IE_ProfileType::Full:
        break;
    case IE_ProfileType::OnlyImage:
        break;
    }


    if (!_modelData.getTmpDir().exists()) {
        _modelData.getTmpDir().mkpath(".");
    }
    _modelData.getTmpDir().mkpath("./res");

    if(dialog)
    {
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
    }
    return 0;
}

int         IE_Model::initWithModel          (TSP_PatientData patientData)
{

    if(patientData.modelFilePath == "Empty")
    {
        patientData.modelFilePath = QFileDialog::getOpenFileName(nullptr,
                                                             "Выбор модели изображения",
                                                             m_ieGlobalData->getLastSelectedDirByUser(),
                                                             QString("*%1.%2")  .arg(IE_MODEL_FILE_NAME_POSTFIX)
                                                                                .arg(IE_MODEL_FILE_NAME_EXTENSION)
                                                             );

    }

    QFile modelFile;
    modelFile.setFileName(patientData.modelFilePath);
    if(!modelFile.exists())
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Файл %1 не существует.").arg(patientData.modelFilePath));
        return 1;
    }
    if(!modelFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Файл %1 не удается прочитать.").arg(patientData.modelFilePath));
        qWarning("Couldn't open file.");
        return 1;
    }

    QByteArray saveData = modelFile.readAll();
    modelFile.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject jsonObj = loadDoc.object();

    if(jsonObj["TSP_docType"] != "TSP_JSON_IE_model")
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Файл %1 не является моделью изображения.").arg(patientData.modelFilePath));
        qWarning() << modelFile.fileName() << "isn't TSP_JSON_IE_model.";
//        patientBaseFile.rename("data/patients/!!!patientBase_ERROR.json");
        return 1;
    }

    _modelData.init(patientData);
    read(jsonObj);

    switch (_modelData.getIem_type())
    {
    case IEM_type::HairDencity:
    {
        pToolCnt->setToolSetType(ToolSet::HairDencity);
        break;
    }
    case IEM_type::TrichoscopyPatterns:
    {
        m_pFieldOfViewCnt->initImageBase();
        pToolCnt->setToolSetType(ToolSet::TrichoscopyPatterns);
        break;
    }
    case IEM_type::AssessmentOfScalp:
    case IEM_type::AssessmentOfHairRoots:
    case IEM_type::AssessmentOfHairRods:
    case IEM_type::DermatoscopyOfNeoplasms:
    {
        pToolCnt->setToolSetType(ToolSet::Simple);
        break;
    }
    }

    return 0;
}

// ------- GETTERS and SETTERS

QRectF      IE_Model::getModelRect           () const
{
    return modelRect;
}

qreal       IE_Model::getMeasureIndex        () const
{
    return m_ieGlobalData->getMeasureIndex();
}
void        IE_Model::setMeasureIndex        (const qreal &value)
{
    m_ieGlobalData->setMeasureIndex(value);
    m_ieGlobalData->setUnitType(UnitType::mm);
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
//QList<IE_ModelLayer*>::const_iterator
//            IE_Model::getLayersListIter      () const
//{
//    if(layersList.isEmpty())
//        return layersList.end();
//    return layersList.begin();
//}

IE_ModelLayer::PublicConstPtrToList IE_Model::getConstModelLayerListConstPtr() const
{
    return m_modelLayerCnt.getPublicConstPtrToList();
}

//QList<IE_ModelLayer *>
//            IE_Model::getLayersList          () const
//{
//    return layersList;
//}

IE_ProfileType IE_Model::getIE_ProfileType() const
{
    return _modelData.getProfile();
}


TSP_PatientData
            IE_Model::get_TSP_patientData()
{
    return _modelData.to_TSP_patientData();
}
//IE_ModelLayer *
//            IE_Model::getLayerByListIndex    (int listIndex)
//{
//    QList<IE_ModelLayer*>::iterator iterResult = getLayerIteratorByListIndex(listIndex);
//    return iterResult == layersList.end() ? nullptr : *iterResult;
//}
//QList<IE_ModelLayer*>::iterator
//            IE_Model::getLayerIteratorByListIndex
//                                                        (int listIndex)
//{
//    if(listIndex<0 || listIndex >= layersList.count())
//        return layersList.end();

//    QList<IE_ModelLayer*>::iterator iter = layersList.begin();
//    for (int i=0;i<listIndex;i++, iter++);
//    return iter;
//}
_global_ie *IE_Model::getPGlobal_data() const
{
    return m_ieGlobalData;
}
QStringList IE_Model::getRelatedModelList() const
{
    return m_relatedModelList;
}

QString IE_Model::getPath()
{
    return m_ieGlobalData->getModelDirPath();
}

IEM_type IE_Model::getIEM_type()
{
    return _modelData.getIem_type();
}

QDockWidget *IE_Model::getImageBaseDockWidget()
{
    if(m_pFieldOfViewCnt)
        return m_pFieldOfViewCnt->getImageBaseDockWidget();
    return nullptr;
}

QWidget *IE_Model::getFieldOfViewFastManagerWidget()
{
    if(m_pFieldOfViewCnt)
        return m_pFieldOfViewCnt->getFastManagerWidget();
    return nullptr;
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

QDockWidget *IE_Model::initFieldOfViewControllerInfoDock()
{
    m_pFVCDockWidget = new QDockWidget("Параметры полей зрения");
    if(!m_pFieldOfViewCnt)
        return m_pFVCDockWidget;
    if(!m_pFieldOfViewCnt->getInfoWidget())
        return m_pFVCDockWidget;
    m_pFVCDockWidget->setWidget(m_pFieldOfViewCnt->getInfoWidget());
    return m_pFVCDockWidget;
}

void IE_Model::makeReport(IE_ReportType rt)
{
    IE_Report::makeReport(this->parent(), m_ieGlobalData, IE_Compute::InputData(m_pFieldOfViewCnt->getPublicFieldOfViewList(), rt));
}

/*void        IE_Model::makeHairDensityComputeWithWidget
                                                        ()
{
    //! \bug Утечка памяти!
    IE_Report * pReport = new IE_Report(m_ieGlobalData);
    IE_Compute comp(m_ieGlobalData,&layersList);

    pReport->makeHairDensityReport_dialog(comp.compute(IE_Compute::ComputeType::HairDensity));
}*/

/*void        IE_Model::makeHairDiameterComputeWithWidget
                                                        ()
{
    //! \bug Утечка памяти!
    IE_Report * pReport = new IE_Report(m_ieGlobalData);
    IE_Compute comp(m_ieGlobalData,&layersList);

    pReport->makeHairDiameterReport_dialog(comp.compute(IE_Compute::ComputeType::HairDiameter));
}*/





void        IE_Model::setInputArgs           ()
{
    //! \bug утечка данных
    QPushButton *ppbSave = new QPushButton();
    ppbSave->setText("Сохранить");

    QDialog * locPDialog = new QDialog(nullptr, Qt::Sheet);
    locPDialog->setMinimumWidth(300);
    locPDialog->setMinimumHeight(300);
    //pTmp->setMaximumSize(QSize(300,300));
    locPDialog->setWindowTitle("Настройки вычислений");
    locPDialog->setWindowFlags(Qt::Tool);
    QVBoxLayout *pVertBoxLayout = new QVBoxLayout(locPDialog);

//    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(this);
    QLabel *pLabel = new QLabel("Единицы измерения:");

    QComboBox    *pcbo = new QComboBox();
    pLabel->setBuddy(pcbo);
    QStringList  lst;

    lst << UnitTypeTitle[0] << UnitTypeTitle[1] << UnitTypeTitle[2] << UnitTypeTitle[3] << UnitTypeTitle[4];
    pcbo->addItems(lst);
    pcbo->setCurrentIndex(m_ieGlobalData->getUnitType());
    pVertBoxLayout->addWidget(pLabel);
    pVertBoxLayout->addWidget(pcbo);

    connect(ppbSave, &QPushButton::released, [this, pcbo]()
    {
        m_ieGlobalData->setIndexUnitType(pcbo->currentIndex());
    });



    pLabel = new QLabel("Порог для Терминальных/Веллусных:");

    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(locPDialog);
    QLineEdit *pLineEdit = new QLineEdit(locPDialog);
    pLabel->setBuddy(pLineEdit);
    //pSpinBox->setEnabled(false);
    pLineEdit->setText(QString().number(m_ieGlobalData->getThreshold_TW()));

    pVertBoxLayout->addWidget(pLabel);
    pHorBoxLayout->addWidget(pLineEdit);

    QLabel *chosenUnit = new QLabel();
    chosenUnit->setText(pcbo->currentText());
    connect(pcbo, &QComboBox::currentTextChanged, chosenUnit, &QLabel::setText);

    pHorBoxLayout->addWidget(chosenUnit);
    pVertBoxLayout->addLayout(pHorBoxLayout);



    connect(ppbSave, &QPushButton::released, [this, pLineEdit]()
    {
        m_ieGlobalData->setThreshold_TW(QString(pLineEdit->text()).toDouble());
    });


    pLabel = new QLabel("Верхний порог для <b>тонкого</b> волоса:");
    pLabel->setTextFormat( Qt::RichText );

    pHorBoxLayout = new QHBoxLayout(locPDialog);
    pLineEdit = new QLineEdit(locPDialog);
    pLabel->setBuddy(pLineEdit);
    pLineEdit->setText(QString().number(m_ieGlobalData->getThreshold_thinHair()));

    pVertBoxLayout->addWidget(pLabel);
    pHorBoxLayout->addWidget(pLineEdit);

    chosenUnit = new QLabel();
    chosenUnit->setText(pcbo->currentText());
    connect(pcbo, &QComboBox::currentTextChanged, chosenUnit, &QLabel::setText);
    pHorBoxLayout->addWidget(chosenUnit);
    pVertBoxLayout->addLayout(pHorBoxLayout);

    connect(ppbSave, &QPushButton::released, [this, pLineEdit]()
    {
        m_ieGlobalData->setThreshold_thinHair(QString(pLineEdit->text()).toDouble());
    });

    pLabel = new QLabel("Верхний порог для <b>среднего</b> волоса:");
    pLabel->setTextFormat( Qt::RichText );

    pHorBoxLayout = new QHBoxLayout(locPDialog);
    pLineEdit = new QLineEdit(locPDialog);
    pLabel->setBuddy(pLineEdit);
    pLineEdit->setText(QString().number(m_ieGlobalData->getThreshold_mediumHair()));

    pVertBoxLayout->addWidget(pLabel);
    pHorBoxLayout->addWidget(pLineEdit);

    chosenUnit = new QLabel();
    chosenUnit->setText(pcbo->currentText());
    connect(pcbo, &QComboBox::currentTextChanged, chosenUnit, &QLabel::setText);
    pHorBoxLayout->addWidget(chosenUnit);
    pVertBoxLayout->addLayout(pHorBoxLayout);

    connect(ppbSave, &QPushButton::released, [this, pLineEdit]()
    {
        m_ieGlobalData->setThreshold_mediumHair(QString(pLineEdit->text()).toDouble());
    });


    pVertBoxLayout->addStretch(1);
    pVertBoxLayout->addWidget(ppbSave);
    locPDialog->setLayout(pVertBoxLayout);

    locPDialog->show();
    locPDialog->raise();
    locPDialog->activateWindow();


}

QDialog::DialogCode IE_Model::makeDialogForSetupModelAsNew()
{
    if(_modelData.getProfile() == IE_ProfileType::MeasureIndex)
    {
        m_pFieldOfViewCnt->init(IE_FieldOfView_Controller::Quantity::One);
        return QDialog::Accepted;
    }


    QDialog * locPDialog = new QDialog();
    QVBoxLayout * pMainLayout = new QVBoxLayout(locPDialog);
    pMainLayout->addWidget(new QLabel("Первичные настройки для создания нового обследования:",locPDialog));
    QComboBox    *pcboProfile;
    if(_modelData.getProfile() == IE_ProfileType::None)
    {
        QLabel *pLabel = new QLabel("Тип обследования:", locPDialog);

        pcboProfile = new QComboBox(locPDialog);
        pLabel->setBuddy(pcboProfile);
        QStringList  lst;

        lst << "Трихограмма" << "Трихоскопия" << "Фототрихограмма";
        pcboProfile->addItems(lst);
        pcboProfile->setCurrentIndex( IE_FieldOfView_Controller::getStandartQuantity(_modelData.getProfile())  );
        pMainLayout->addWidget(pLabel);
        pMainLayout->addWidget(pcboProfile);

    }

    QLabel *pLabel = new QLabel("Количество полей зрения:", locPDialog);

    QComboBox    *pcboQuiantityFV = new QComboBox(locPDialog);
    pLabel->setBuddy(pcboQuiantityFV);
    QStringList  lst;

    lst << QString().number(IE_FieldOfView_Controller::Quantity::One)
        << QString().number(IE_FieldOfView_Controller::Quantity::Two)
        << QString().number(IE_FieldOfView_Controller::Quantity::Three)
        << QString().number(IE_FieldOfView_Controller::Quantity::Four)
        << QString().number(IE_FieldOfView_Controller::Quantity::Six);
    pcboQuiantityFV->addItems(lst);
    pcboQuiantityFV->setCurrentIndex( IE_FieldOfView_Controller::getStandartQuantity(_modelData.getProfile())  );

    pMainLayout->addWidget(pLabel);
    pMainLayout->addWidget(pcboQuiantityFV);

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                         | QDialogButtonBox::Cancel,locPDialog);
    pMainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, locPDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, locPDialog, &QDialog::reject);


    locPDialog->setLayout(pMainLayout);
    locPDialog->setModal(true);

    locPDialog->show();
    int answer =  locPDialog->exec();

    if(answer == QDialog::Accepted)
    {
        if(pcboProfile)
            switch (pcboProfile->currentIndex()) {
            case (IE_ProfileType::Trichogram - 1):
            {
                _modelData.setProfile(IE_ProfileType::Trichogram);
                break;
            }
            case (IE_ProfileType::Trichoscopy - 1):
            {
                _modelData.setProfile(IE_ProfileType::Trichoscopy);
                break;
            }
            case (IE_ProfileType::Phototrichogram - 1):
            {
                _modelData.setProfile(IE_ProfileType::Phototrichogram);
                break;
            }
            }
        m_pFieldOfViewCnt->init((IE_FieldOfView_Controller::Quantity)pcboQuiantityFV->currentText().toInt());
    }
    delete locPDialog;
    return (QDialog::DialogCode)answer;
}




// ------- Layer manipulations


void        IE_Model::addLayerViaToolCnt     ()
{
    qDebug() << "IE_Model::addLayerViaToolCnt";
    if(!pToolCnt)
        return;
    qDebug() << "IE_Model::addLayerViaToolCnt 2";
    addLayer(pToolCnt->getPActiveTool());
}

void        IE_Model::addLayer               (IE_ModelLayer* layerToAdd)
{
    m_modelLayerCnt.addLayer(layerToAdd);
//    for (QList<IE_ModelLayer*>::iterator iter = layersList.begin();iter!=layersList.end();iter++)
//        if((*iter) == layerToAdd)
//        {
//            qWarning() << "Tring to add existed layer.";
//            return;
//        }

//    addItem(layerToAdd->parentItem());
//    layersList.append(layerToAdd);
    emit layerListWasChanged();
}

/*void        IE_Model::showLayer              (int listIndex)
{
    IE_ModelLayer* foundLayer = getLayerByListIndex     (listIndex);
    if(!foundLayer)
    {
        qDebug() << "TSPImageEditorModel::showLayer(int listIndex) >> getLayerByListIndex returns nullptr.";
        return;
    }

    foundLayer->unhide();
}*/
/*void        IE_Model::hideLayer              (int listIndex)
{
    IE_ModelLayer* foundLayer = getLayerByListIndex(listIndex);
    if(!foundLayer)
    {
        qDebug() << "TSPImageEditorModel::hideLayer(int listIndex) >> getLayerByListIndex returns nullptr.";
        return;
    }

    foundLayer->hide();
}*/

/*void        IE_Model::removeLayer             (int listIndex)
{
    QList<IE_ModelLayer*>::iterator iterResult = getLayerIteratorByListIndex(listIndex);
    if(iterResult == layersList.end())
        return;
    removeLayer(iterResult);
}*/








void IE_Model::layerAction(IE_ModelLayer::Action action, IE_ModelLayer::PublicType layer)
{
    switch (action)
    {
    case IE_ModelLayer::Action::Show:
        m_modelLayerCnt.showLayer(layer);
        break;
    case IE_ModelLayer::Action::Hide:
        m_modelLayerCnt.hideLayer(layer);
        break;
    case IE_ModelLayer::Action::Remove:
        m_modelLayerCnt.removeLayer(layer);
        pToolCnt->resetPActiveTool();
        break;
    }
}

/// \todo !!!!!! ------- :
void        IE_Model::selectedLayer          (int row)
{

}

// ------- END Layer manipulations ------- //

void IE_Model::addRelatedModel(QString path)
{
    m_relatedModelList.append(path);
}

QDockWidget *IE_Model::getFieldOfViewControllerInfoDock() const
{
    return m_pFVCDockWidget;
}



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

IE_ModelData::IE_ModelData()
{
    patientID = patientUID = model_ID = 0;
    patientFullName = "Empty";
    modelDir = resDir = tmpDir = QDir();
    profile = IE_ProfileType::None;
}

void IE_ModelData::initNew(TSP_PatientData patientData)
{
    QTime dieTime= QTime::currentTime().addMSecs(10);
       while (QTime::currentTime() < dieTime)
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    model_ID = QDateTime::currentDateTime().toTime_t()- 1560000000;
    model_ID*=1000;
    model_ID+=(QTime::currentTime().msec())%1000;
    init(patientData);
    modelDir = QString("%1/%2/%3") .arg(patientData.modelDir)
            .arg(IE_MODEL_DIR_NAME)
            .arg(model_ID);
    m_createDateTime = QDateTime::currentDateTime();
    update();
}

void IE_ModelData::init(TSP_PatientData patientData)
{
    patientID = patientData.patient_ID;
    patientUID = patientData.patient_UID;
    patientFullName = patientData.patient_fullName;
    modelDir = patientData.modelDir;
    patientDir = patientData.patientDir;
//    profile = patientData.ie_type;
    m_iem_type = IEM_type::None;
    if(QFile::exists(patientData.modelFilePath))
    {
        modelDir = patientData.modelFilePath;
        modelDir.cdUp();
    }
}

void IE_ModelData::update()
{
    //        modelDir.setPath(QString("%1/%2").  arg(modelDir.path()).
    //                                            arg(model_ID)
    //                         );
    tmpDir.setPath(QString("%1/%2/%3"). arg(IE_TMP_DIR_NAME).
                   arg(IE_MODEL_DIR_NAME).
                   arg(model_ID)
                   );
    resDir.setPath(QString("%1").arg(IE_MODEL_RES_DIR_NAME));
    emit updated();
}

QString IE_ModelData::getPath()
{

}

TSP_PatientData IE_ModelData::to_TSP_patientData()
{
    TSP_PatientData answer;
    answer.modelDir = modelDir.path();
    answer.model_ID = model_ID;
    answer.patient_ID = patientID;
    answer.patient_UID = patientUID;
    answer.patient_fullName = patientFullName;
    answer.patientDir = patientDir.path();
//    answer.ie_type = profile;
    return answer;
}

int IE_ModelData::read(const QJsonObject &json)
{

    // подразумивается, что patientUID может быть нуливым только тогда, когда мы читаем МОДЕЛЬ напрямую. А так, при поступлении данных о пациенте, эти данные перезаписываются и становятся актуальными (если изменился ID пациента и/или его имя).
    if(patientUID == 0)
    {
        patientID = json["patient_ID"].toString().toUInt();
        patientFullName = json["patient_fullName"].toString();
    }

    model_ID = json["model_ID"].toString().toUInt();
    patientUID = json["patient_UID"].toString().toUInt();
    profile = getIE_ProfileType(json["ie_type"].toString());
    m_iem_type = getIEM_type( json["iem_type"].toString() );
    m_createDateTime = QDateTime::fromString( json["createDateTime"].toString(), Qt::DateFormat::ISODate );
    m_saveDateTime = QDateTime::fromString( json["saveDateTime"].toString(), Qt::DateFormat::ISODate );

    update();
    return 0;
}

int IE_ModelData::write(QJsonObject &json) const
{
    json["model_ID"] = QString().number(model_ID);
    json["patient_ID"] = QString().number(patientID);
    json["patient_UID"] = QString().number(patientUID);
    json["patient_fullName"] = patientFullName;
    json["ie_type"] = getIE_ProfileType(profile);
    json["iem_type"] = getTSP_JSON_IEM_type(m_iem_type);
    json["createDateTime"] = m_createDateTime.toString(Qt::DateFormat::ISODate);
    json["saveDateTime"] = m_saveDateTime.toString(Qt::DateFormat::ISODate);
    json["alias"] = "";
    return 0;
}

void IE_ModelData::printAllData()
{
    qDebug() << patientFullName;
    qDebug() << modelDir;
    qDebug() << resDir;
    qDebug() << tmpDir;
    qDebug() << patientID;
    qDebug() << patientUID;
    qDebug() << model_ID;
    qDebug() << getIE_ProfileType(profile);

}

uint IE_ModelData::getModel_ID() const
{
    return model_ID;
}

void IE_ModelData::setModel_ID(const uint &value)
{
    model_ID = value;
    update();
}

IEM_type IE_ModelData::getIem_type() const
{
    return m_iem_type;
}

void IE_ModelData::setIem_type(const IEM_type &iem_type)
{
    m_iem_type = iem_type;
}

QDateTime IE_ModelData::getSaveDateTime() const
{
    return m_saveDateTime;
}

void IE_ModelData::setSaveDateTime(const QDateTime &saveDateTime)
{
    m_saveDateTime = saveDateTime;
}

QDateTime IE_ModelData::getCreateDateTime() const
{
    return m_createDateTime;
}

uint IE_ModelData::getPatientUID() const
{
    return patientUID;
}



uint IE_ModelData::getPatientID() const
{
    return patientID;
}



QDir IE_ModelData::getTmpDir() const
{
    return tmpDir;
}

void IE_ModelData::setTmpDir(const QDir &value)
{
    tmpDir = value;
    update();
}

QDir IE_ModelData::getResDir() const
{
    return resDir;
}

QDir IE_ModelData::getModelDir() const
{
    return modelDir;
}

QString IE_ModelData::getPatientFullName() const
{
    return patientFullName;
}



IE_ProfileType IE_ModelData::getProfile() const
{
    return profile;
}

void IE_ModelData::setProfile(const IE_ProfileType &value)
{
    profile = value;
}
