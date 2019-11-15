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
    patientDataTmp.ie_type = IE_ProfileType::Trichoscopy;
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
