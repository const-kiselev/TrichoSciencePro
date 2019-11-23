#include "pm_patient.h"

#include <QGroupBox>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

PM_Patient::PM_Patient(QDir workDir, QObject *parent) : QObject(parent), m_workDir(workDir)
{
    m_pMedicalRecordCnt = nullptr;
    m_pDocumentIndexCnt = nullptr;
    m_pWidget = nullptr;
}

int PM_Patient::initAsNew_Dialog()
{
    init();
    m_pMedicalRecordCnt->initAsNew();
    int answer = newPatientDialog();
    if( answer )
        return answer;

    answer = makePatientDirectory();
    if( answer )
        return answer;

    m_pMedicalRecordCnt->save();
    m_workDir.cd(QString().number( m_pMedicalRecordCnt->getPatientUID() ) );


    return answer;
}


//static
TSP_PatientData PM_Patient::getTSP_PatientData(QDir patientDir)
{
    return PM_MedicalRecordCnt::getTSP_PatientData(patientDir);
}

TSP_PatientData PM_Patient::getTSP_PatientData()
{
    return m_pMedicalRecordCnt->getTSP_PatientData();
}

//static
uint PM_Patient::getUIDFromDirName(QDir patientDir)
{
    return patientDir.dirName().toUInt();
}

QWidget *PM_Patient::getWidget()
{
    if(!m_pWidget)
        initWidget();
    return m_pWidget;
}

void PM_Patient::removeAllData()
{
    int n = QMessageBox::warning(nullptr,
                                 "Предупреждение",
                                 "Вы уверены, что хотите удалить данного пациента?"
                                 "\nЭто повлечет удаление всех данных, связанных с ним.",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::No
                                 );
    if(n == QMessageBox::No)
        return;

    m_workDir.removeRecursively();
    emit dataWasChanged();
    emit goBack();
}

int PM_Patient::open(uint patientUID)
{
    if( m_pMedicalRecordCnt )
        delete m_pMedicalRecordCnt;



    if(!m_workDir.cd( QString().number(patientUID)) )
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось найти папку пациента %1 .").arg( m_workDir.filePath( QString().number(patientUID) ) ));
        qWarning() << "Couldn't open patient directory `"<< m_workDir.filePath( QString().number(patientUID) ) <<"` .";
        return 1;
    }
    init();
    m_pMedicalRecordCnt->open();

    if(!m_pDocumentIndexCnt)
        delete m_pDocumentIndexCnt;
    m_pDocumentIndexCnt = new PM_DocumentIndexCnt(m_workDir);
    m_pDocumentIndexCnt->open();
    connect(m_pDocumentIndexCnt, &PM_DocumentIndexCnt::openImageEditor, [this](QString path)
    {
        TSP_PatientData pd;
        pd.modelFilePath = path;
        pd.modelDir = path;
        emit needToRunImageEditor(pd, IE_ProfileType::None);
    });

    return 0;
}

QList<QMenu *> PM_Patient::getAvailableActions()
{
    QList<QMenu*> menuList;
    QMenu * pMenu = new QMenu("Файл");
    QAction *pAction = new QAction("Экспорт...");
    pAction->setDisabled(true);
    pMenu->addAction(pAction);
    pAction = new QAction("Удалить");
    connect(pAction, &QAction::triggered, this, &PM_Patient::removeAllData);
    pMenu->addAction(pAction);

    menuList.append(pMenu);

//    pMenu = new QMenu("Правка");

//    pAction = new QAction("Удалить");
//    pMenu->addAction(pAction);

//    menuList.append(pMenu);

    pMenu = new QMenu("Действия");

    pAction = new QAction("Трихоскопия...");
    connect(pAction, &QAction::triggered,[this]()
    {
        emit needToRunImageEditor(m_pMedicalRecordCnt->getTSP_PatientData(), IE_ProfileType::Trichoscopy);
    });
    pMenu->addAction(pAction);
    pAction = new QAction("Фототрихограмма...");
    connect(pAction, &QAction::triggered,[this]()
    {
        emit needToRunImageEditor(m_pMedicalRecordCnt->getTSP_PatientData(), IE_ProfileType::Phototrichogram);
    });
    pMenu->addAction(pAction);
    pAction = new QAction("Трихограмма...");
    connect(pAction, &QAction::triggered,[this]()
    {
        emit needToRunImageEditor(m_pMedicalRecordCnt->getTSP_PatientData(), IE_ProfileType::Trichogram);
    });
    pMenu->addAction(pAction);
    pAction = new QAction("Дерматоскопия...");
    pAction->setDisabled(true);
    pMenu->addAction(pAction);
    pAction = new QAction("Шкала тревоги и депрессии...");
    pAction->setDisabled(true);
    pMenu->addAction(pAction);
    pAction = new QAction("Рекомендации...");
    pAction->setDisabled(true);
    pMenu->addAction(pAction);




    menuList.append(pMenu);

    pMenu = new QMenu("Документы");
    pAction = new QAction("Найти все");
    connect(pAction, &QAction::triggered,[this]()
    {
        emit m_pDocumentIndexCnt->findAllDocuments();
    });
    pMenu->addAction(pAction);

    menuList.append(pMenu);


    return menuList;
}



int PM_Patient::init()
{
    m_pWidget = nullptr;
    m_pMedicalRecordCnt = new PM_MedicalRecordCnt( m_workDir );
    return 0;
}

int PM_Patient::initWidget()
{
    m_pWidget = new QWidget();
    QHBoxLayout * phbLayout = new QHBoxLayout(m_pWidget);
    QVBoxLayout * pvbLayout = new QVBoxLayout(m_pWidget);
    PM_MedicalRecord medRec = m_pMedicalRecordCnt->getMedicalRecord();


    QPushButton * pPButton = new QPushButton("Назад", m_pWidget);
    pvbLayout->addWidget(pPButton);
    connect(pPButton, &QPushButton::clicked, this, &PM_Patient::goBack);

    pvbLayout->addWidget(new QLabel(medRec.getPatient_fullName(), m_pWidget));
    pvbLayout->addWidget(new QLabel( QString("uid: %1").arg(medRec.getPatient_UID()) , m_pWidget));
    pvbLayout->addWidget(new QLabel( QString("id: %1").arg(medRec.getPatient_ID()) , m_pWidget));
//    pvbLayout->addStretch(1);

    pPButton = new QPushButton("Открыть мед.карту", m_pWidget);
    pvbLayout->addWidget(pPButton);
    pPButton = new QPushButton("Добавить протокол осмотра", m_pWidget);
    pvbLayout->addWidget(pPButton);



//    QLabel * pLabel = new QLabel(medRec.getPatient_fullName(), m_pWidget);
    phbLayout->addItem(pvbLayout);


    if(m_pDocumentIndexCnt)
    {
        pvbLayout = new QVBoxLayout(m_pWidget);
        QWidget * tmp = m_pDocumentIndexCnt->getWidget();
        tmp->setParent(m_pWidget);
        pvbLayout->addWidget(tmp );
        phbLayout->addItem(pvbLayout);
    }

    pvbLayout = new QVBoxLayout(m_pWidget);
//    pvbLayout->addStretch(1);

    pPButton = new QPushButton("Трихоскопия", m_pWidget);
    pvbLayout->addWidget(pPButton);
    connect(pPButton, &QPushButton::clicked,[this]()
    {
        emit needToRunImageEditor(m_pMedicalRecordCnt->getTSP_PatientData(), IE_ProfileType::Trichoscopy);
    });
    pPButton = new QPushButton("Фототрихограмма", m_pWidget);
    pvbLayout->addWidget(pPButton);
    connect(pPButton, &QPushButton::clicked,[this]()
    {
        emit needToRunImageEditor(m_pMedicalRecordCnt->getTSP_PatientData(), IE_ProfileType::Phototrichogram);
    });
    pPButton = new QPushButton("Трихограмма", m_pWidget);
    pvbLayout->addWidget(pPButton);
    connect(pPButton, &QPushButton::clicked,[this]()
    {
        emit needToRunImageEditor(m_pMedicalRecordCnt->getTSP_PatientData(), IE_ProfileType::Trichogram);
    });
    pPButton = new QPushButton("Дерматоскопия", m_pWidget);
    pPButton->setDisabled(true);
    pvbLayout->addWidget(pPButton);
    pPButton = new QPushButton("Шкала тревоги и депрессии", m_pWidget);
    pPButton->setDisabled(true);
    pvbLayout->addWidget(pPButton);
    pPButton = new QPushButton("Рекомендации", m_pWidget);
    pPButton->setDisabled(true);
    pvbLayout->addWidget(pPButton);
    phbLayout->addItem(pvbLayout);

    m_pWidget->setLayout(phbLayout);

    return 0;
}


int PM_Patient::newPatientDialog()
{
    PM_MedicalRecord medRec = m_pMedicalRecordCnt->getMedicalRecord();

    QDialog *pDialog = new QDialog();

    QVBoxLayout *pLayout = new QVBoxLayout();

    QLabel *pLabel = new QLabel("UID: "+QString().number( medRec.getPatient_UID() ));
    pLayout->addWidget(pLabel);

    pLabel = new QLabel("ФИО:");
    QLineEdit *pFullName = new QLineEdit();
    pLabel->setBuddy(pFullName);
    pLayout->addWidget(pLabel);
    pLayout->addWidget(pFullName);

    pLabel = new QLabel("ID:");
    QLineEdit * pID = new QLineEdit(QString().number( medRec.getPatient_UID() ));
    pLabel->setBuddy(pID);
    pLayout->addWidget(pLabel);
    pLayout->addWidget(pID);

    QRadioButton* pradMale   = new QRadioButton("Мужской");
    QRadioButton* pradFemale = new QRadioButton("Женский");

    {
        QGroupBox *pGenderGroupBox = new QGroupBox("Пол");
        QHBoxLayout* phbxLayout = new QHBoxLayout;
        phbxLayout->addWidget(pradMale);
        phbxLayout->addWidget(pradFemale);
        pGenderGroupBox->setLayout(phbxLayout);
        pLayout->addWidget(pGenderGroupBox);
    }

    QRadioButton* pradEuroCaucasian   = new QRadioButton("Европейский/Кавказский");
    QRadioButton* pradMediterranean = new QRadioButton("Средиземноморский");
    QRadioButton* pradAsianPacific   = new QRadioButton("Азиатский/Тихоокеанский");
    QRadioButton* pradAfricanCaribbean = new QRadioButton("Аффрикано-Карибский тип");

    {
        QGroupBox *pHairTypeGroupBox = new QGroupBox("Тип волос");

        QVBoxLayout* pvbxLayout = new QVBoxLayout;
        pvbxLayout->addWidget(pradEuroCaucasian);
        pvbxLayout->addWidget(pradMediterranean);
        pvbxLayout->addWidget(pradAsianPacific);
        pvbxLayout->addWidget(pradAfricanCaribbean);
        pHairTypeGroupBox->setLayout(pvbxLayout);
        pLayout->addWidget(pHairTypeGroupBox);
    }
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    pLayout->addWidget(line);

    QPushButton *pcmdOk = new QPushButton("Добавить");
    QPushButton *pcmdCancel = new QPushButton("Отменить");
    connect(pcmdOk, &QPushButton::clicked, [ pDialog, pFullName, pradEuroCaucasian,
                                            pradMediterranean, pradAsianPacific, pradAfricanCaribbean, pradMale, pradFemale]()
    {
        bool error = false;
        if(pFullName->text() == "")
            error = true;
        else if( !( pradEuroCaucasian->isChecked() ||
                    pradMediterranean->isChecked() ||
                    pradAsianPacific->isChecked() ||
                    pradAfricanCaribbean->isChecked() ) )
                error = true;
        else if( !(pradMale->isChecked() || pradFemale->isChecked()) )
            error = true;

        if(error)
            QMessageBox::warning(nullptr,
                                 "Warning",
                                 "Все поля обязательны для заполнения!",
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        else
            emit pDialog->accept();
    });
    connect(pcmdCancel, SIGNAL(clicked()),pDialog, SLOT(reject()));
    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(pcmdOk);
    phbxLayout->addWidget(pcmdCancel);
    pLayout->addLayout(phbxLayout);

    pDialog->setLayout(pLayout);

    if (pDialog->exec() == QDialog::Accepted)
    {
        medRec.setPatient_ID( pID->text().toUInt() );
        medRec.setPatient_fullName( pFullName->text() );
        medRec.setGender( pradMale->isChecked() ?
                              PM_MedicalRecord::GenderType::Male :
                              PM_MedicalRecord::GenderType::Female
                        );
        if(pradEuroCaucasian->isChecked())
            medRec.setHairType( PM_MedicalRecord::HairType::EuroCaucasian );
        else if(pradMediterranean->isChecked())
            medRec.setHairType( PM_MedicalRecord::HairType::Mediterranean );
        else if(pradAsianPacific->isChecked())
            medRec.setHairType( PM_MedicalRecord::HairType::AsianPacific );
        else if(pradAfricanCaribbean->isChecked())
            medRec.setHairType( PM_MedicalRecord::HairType::AfricanCaribbean );
        delete pDialog;

        m_pMedicalRecordCnt->setMedicalRecord(medRec);
        return 0;
    }

    delete pDialog;
    return 1;
}

int PM_Patient::loadMedicalRecord()
{

}

int PM_Patient::makePatientDirectory()
{
    m_workDir = m_workDir.filePath( QString().number( m_pMedicalRecordCnt->getPatientUID() ) );
    if ( m_workDir.exists() )
        m_workDir.mkpath(".");

    if (!m_workDir.exists("./res"))
        m_workDir.mkpath("./res");
    return 0;
}
