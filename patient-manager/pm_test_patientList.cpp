#include "pm_test_patientList.h"

PM_test_patientList::PM_test_patientList(QWidget *parent): QWidget (parent)
{
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);

    pFullPatientList = new QListWidget();
    pPatientSessions = new QListWidget();
    pPatientActions = new QVBoxLayout();

    QVBoxLayout *pVBLayout = new QVBoxLayout();
    pVBLayout->addWidget(new QLabel("Список пациентов:"));
    pVBLayout->addWidget(pFullPatientList);
    pMainLayout->addLayout(pVBLayout);

    connect(pFullPatientList, SIGNAL(currentRowChanged(int)),
            this, SLOT(currentPatientInListWasChanged(int)));

    pVBLayout = new QVBoxLayout();
    pVBLayout->addWidget(new QLabel("Список сеансов-исследований:"));
    pVBLayout->addWidget(pPatientSessions);
    pMainLayout->addLayout(pVBLayout);

    connect(pPatientSessions, &QListWidget::doubleClicked,[this](const QModelIndex &index)
    {
        runImageEditorWithModel(pPatientSessions->item(index.row())->text());
    });

    m_pButtonPatientRecord = new QPushButton("Амбулаторная карта");
    m_pButtonPatientRecord->setEnabled(false);
    pPatientActions->addWidget(m_pButtonPatientRecord);

    m_pButtonNewVisit = new QPushButton("Добавить посещение");
    m_pButtonNewVisit->setEnabled(false);
    pPatientActions->addWidget(m_pButtonNewVisit);

    m_pButtonCreateSession = new QPushButton("Создать сеанс-исследование");
    pPatientActions->addWidget(m_pButtonCreateSession);

    connect(m_pButtonCreateSession, &QPushButton::released, this, &PM_test_patientList::runImageEditorNew);

    pMainLayout->addLayout(pPatientActions);
    setLayout(pMainLayout);
    initMenuBar();
    readPatientBase();

    pFullPatientList->setCurrentRow(-1);
    activatePatientActions(false);
}



void PM_test_patientList::readPatientBase()
{
    QFile patientBaseFile("data/patients/patientBase.json");

//    patientBaseFile.setFileName();
    if(!patientBaseFile.exists())
        return;
    if(!patientBaseFile.open(QIODevice::ReadOnly))
        qWarning("Couldn't open file.");

    QByteArray saveData = patientBaseFile.readAll();
    patientBaseFile.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));


    QJsonObject jsonDoc = loadDoc.object();

    if(jsonDoc["TSP_docType"] != "TSP_JSON_patientBase")
    {
        qDebug() << patientBaseFile.fileName() << "isn't TSP_JSON_patientBase.";
        patientBaseFile.rename("data/patients/!!!patientBase_ERROR.json");
        return;
    }

    QJsonArray patientArray = jsonDoc["patients"].toArray();
    for (int patientIndex = 0; patientIndex < patientArray.size(); ++patientIndex) {
            QJsonObject patientObject = patientArray[patientIndex].toObject();
            PM_patientRecord::PatientCard card =
                    PM_patientRecord::readPatientCard(patientObject);
            patientList.append( card );
            pFullPatientList->addItem(card.fullName);
        }
}

void PM_test_patientList::writePatientBase()
{
    QFile patientBaseFile;
    patientBaseFile.setFileName("data/patients/patientBase.json");
    if(!patientBaseFile.exists())
    {
        if(!patientBaseFile.open(QIODevice::WriteOnly))
        {
            qWarning("Couldn't open file.");
            return;
        }
        QJsonObject jsonObj;
        jsonObj["TSP_docType"] = "TSP_JSON_patientBase";
        QJsonArray jsonArr;
        jsonObj["patients"] = jsonArr;
        QJsonDocument saveDoc(jsonObj);
        patientBaseFile.write(saveDoc.toJson());
        patientBaseFile.close();
        return;
    }
}

void PM_test_patientList::addPatientToJSONBase(const PM_patientRecord::PatientCard card)
{
    QFile patientBaseFile;
    patientBaseFile.setFileName("data/patients/patientBase.json");
    if(!patientBaseFile.exists())
        writePatientBase();

    if(!patientBaseFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open file.");
        return;
    }

    QByteArray saveData = patientBaseFile.readAll();
    patientBaseFile.close();
    if(!patientBaseFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        qWarning("Couldn't open file.");
        return;
    }
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));


    QJsonObject jsonObj = loadDoc.object();

    if(jsonObj["TSP_docType"] != "TSP_JSON_patientBase")
    {
        qDebug() << patientBaseFile.fileName() << "isn't TSP_JSON_patientBase.";
        patientBaseFile.close();
        patientBaseFile.rename("data/patients/!!!patientBase_ERROR.json");
        writePatientBase();
        return;
    }
    QJsonArray patientArray = jsonObj["patients"].toArray();
    QJsonObject patientCardJSON;
    PM_patientRecord::writePatientCard(patientCardJSON, card);
    patientArray.append(patientCardJSON);
    jsonObj["patients"]=patientArray;

    QJsonDocument saveDoc(jsonObj);
    patientBaseFile.write(saveDoc.toJson());
    patientBaseFile.close();
    pFullPatientList->addItem(card.fullName);
    patientList.append( card );
}

void PM_test_patientList::initMenuBar()
{
    #ifdef Q_OS_MAC
        m_pMenuBar = new QMenuBar(0);
    #else
        m_pMenuBar = new QMenuBar();
    #endif
    QMenu *oneMenu = new QMenu("Пациент");
    QAction *pAction = new QAction("Добавить пациента");
    oneMenu->addAction(pAction);
    connect(pAction, &QAction::triggered, this, &PM_test_patientList::createNewPatient);

    m_pMenuBar->addAction(oneMenu->menuAction());


}

void PM_test_patientList::activatePatientActions(bool activate)
{
    if(m_pButtonCreateSession)
        m_pButtonCreateSession->setEnabled(activate);
//    if(m_pButtonNewVisit)
//        m_pButtonNewVisit->setEnabled(activate);
//    if(m_pButtonPatientRecord)
    //        m_pButtonPatientRecord->setEnabled(activate);
}

void PM_test_patientList::readPatientSessionList(QString sessionsWorkPath)
{
    QFile patientBaseFile;
    patientBaseFile.setFileName(sessionsWorkPath+"/IE_models/sessionsBase.json");
    if(!patientBaseFile.exists())
        return;
    if(!patientBaseFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open file.");
        return;
    }


    QByteArray saveData = patientBaseFile.readAll();
    patientBaseFile.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));


    QJsonObject jsonDoc = loadDoc.object();

    if(jsonDoc["TSP_docType"] != "TSP_JSON_sessionsBase")
    {
        qDebug() << patientBaseFile.fileName() << "isn't TSP_JSON_sessionsBase.";
//        patientBaseFile.rename("data/patients/!!!patientBase_ERROR.json");
        return;
    }

    QJsonArray patientArray = jsonDoc["sessions"].toArray();
    for (int patientIndex = 0; patientIndex < patientArray.size(); ++patientIndex) {
            QJsonObject patientObject = patientArray[patientIndex].toObject();
            pPatientSessions->addItem(patientObject["dir"].toString());
        }
}

void PM_test_patientList::writePatientSessionBase(QString sessionsWorkPath)
{
    QDir workDir(sessionsWorkPath);
    workDir.cdUp();
    if(!workDir.exists())
        workDir.mkdir(".");

    QFile patientBaseFile;
    patientBaseFile.setFileName(workDir.path()+"/sessionsBase.json");
    if(!patientBaseFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        qWarning("Couldn't open file.");
        return;
    }


    QJsonObject jsonObj;
    jsonObj["TSP_docType"] = "TSP_JSON_sessionsBase";
    QJsonArray jsonArr;
    QStringList listDir = workDir.entryList(QDir::Dirs);
    foreach (QString subdir, listDir)
    {
        if(subdir == "." || subdir == "..")
            continue;
        QJsonObject sessionObj;
        sessionObj["dir"] = subdir;
        jsonArr.append(sessionObj);
    }
    jsonObj["sessions"] = jsonArr;
    QJsonDocument saveDoc(jsonObj);
    patientBaseFile.write(saveDoc.toJson());
    patientBaseFile.close();
}

void PM_test_patientList::createNewPatient()
{
    PM_patientRecord *newPatient = new PM_patientRecord();
    if(newPatient->initAsNew())
    {
        delete newPatient;
        return;
    }
    addPatientToJSONBase(newPatient->getPatientCard());
}

void PM_test_patientList::currentPatientInListWasChanged(int row)
{
    if(row == -1)
    {
        activatePatientActions(false);
        currentPatientCard.reset();
        return;
    }
    activatePatientActions();
    currentPatientCard = patientList.at(row);
    pFullPatientList->setCurrentRow(row);
    pPatientSessions->clear();
    readPatientSessionList(QString("data/patients/%1").arg(currentPatientCard.UID));
}

void PM_test_patientList::runImageEditorNew()
{
    if(!currentPatientCard.ID)
        return;
    _Model_patientData patientDataTmp;
    patientDataTmp.patient_ID = currentPatientCard.ID;
    patientDataTmp.patient_UID = currentPatientCard.UID;
    patientDataTmp.patient_fullName = currentPatientCard.fullName;
    patientDataTmp.modelDir = "data/patients/"+QString().number(currentPatientCard.UID);
    emit needToRunNewImageEditor(patientDataTmp);
}

void PM_test_patientList::runImageEditorWithModel(QString modelUID)
{
    if(!currentPatientCard.ID)
        return;
    _Model_patientData patientDataTmp;
    patientDataTmp.patient_ID = currentPatientCard.ID;
    patientDataTmp.patient_UID = currentPatientCard.UID;
    patientDataTmp.patient_fullName = currentPatientCard.fullName;
    patientDataTmp.modelDir = "data/patients/"+QString().number(currentPatientCard.UID);
    patientDataTmp.modelPath = QString("%1/IE_models/%2/%2_IE_model.json").arg(patientDataTmp.modelDir).arg(modelUID);
    emit needToRunSavedImageEditor(patientDataTmp);
}



void PM_test_patientList::addSavedSession(_Model_patientData patientData)
{
    writePatientSessionBase(patientData.modelDir);
}
