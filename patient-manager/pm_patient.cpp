#include "pm_patient.h"

#include <QJsonDocument>
#include <QMessageBox>

PM_Patient::PM_Patient(QObject *parent) : QObject(parent)
{

}

TSP_PatientData PM_Patient::getTSP_PatientData(QDir patientDir)
{

}

uint PM_Patient::getUIDFromDirName(QDir patientDir)
{
    return patientDir.dirName().toUInt();
}






//bool PM_patientRecord::initAsNew(bool withUI)
//{
//    _patientRecord.card.UID = QDateTime::currentDateTime().toTime_t() - 1560000000;
//    if(newPatientWidget())
//        return 1;

//    makePatientDirectory();
//    return 0;
//}

//void PM_patientRecord::makePatientDirectory()
//{
//    _patientRecord.workDir = "data/patients/"+QString().number(_patientRecord.card.UID);
//    if (!_patientRecord.workDir.exists())
//        _patientRecord.workDir.mkpath(".");

//    if (!_patientRecord.workDir.exists("./res"))
//        _patientRecord.workDir.mkpath("./res");
//}


//int PM_patientRecord::newPatientWidget()
//{
//    QDialog *pDialog = new QDialog();

//    QVBoxLayout *pLayout = new QVBoxLayout();

//    QLabel *pLabel = new QLabel("UID: "+QString().number(_patientRecord.card.UID));
//    pLayout->addWidget(pLabel);

//    pLabel = new QLabel("ФИО:");
//    QLineEdit *pFullName = new QLineEdit();
//    pLabel->setBuddy(pFullName);
//    pLayout->addWidget(pLabel);
//    pLayout->addWidget(pFullName);

//    pLabel = new QLabel("ID:");
//    QLineEdit * pID = new QLineEdit(QString().number(_patientRecord.card.UID));
//    pLabel->setBuddy(pID);
//    pLayout->addWidget(pLabel);
//    pLayout->addWidget(pID);

//    QRadioButton* pradMale   = new QRadioButton("Мужской");
//    QRadioButton* pradFemale = new QRadioButton("Женский");

//    {
//        QGroupBox *pGenderGroupBox = new QGroupBox("Пол");
//        QHBoxLayout* phbxLayout = new QHBoxLayout;
//        phbxLayout->addWidget(pradMale);
//        phbxLayout->addWidget(pradFemale);
//        pGenderGroupBox->setLayout(phbxLayout);
//        pLayout->addWidget(pGenderGroupBox);
//    }

//    QRadioButton* pradEuroCaucasian   = new QRadioButton("Европейский/Кавказский");
//    QRadioButton* pradMediterranean = new QRadioButton("Средиземноморский");
//    QRadioButton* pradAsianPacific   = new QRadioButton("Азиатский/Тихоокеанский");
//    QRadioButton* pradAfricanCaribbean = new QRadioButton("Аффрикано-Карибский тип");

//    {
//        QGroupBox *pHairTypeGroupBox = new QGroupBox("Тип волос");

//        QVBoxLayout* pvbxLayout = new QVBoxLayout;
//        pvbxLayout->addWidget(pradEuroCaucasian);
//        pvbxLayout->addWidget(pradMediterranean);
//        pvbxLayout->addWidget(pradAsianPacific);
//        pvbxLayout->addWidget(pradAfricanCaribbean);
//        pHairTypeGroupBox->setLayout(pvbxLayout);
//        pLayout->addWidget(pHairTypeGroupBox);
//    }
//    QFrame* line = new QFrame();
//    line->setFrameShape(QFrame::HLine);
//    line->setFrameShadow(QFrame::Sunken);
//    pLayout->addWidget(line);

//    QPushButton *pcmdOk = new QPushButton("Добавить");
//    QPushButton *pcmdCancel = new QPushButton("Отменить");
//    connect(pcmdOk, &QPushButton::clicked, [ pDialog, pFullName, pradEuroCaucasian,
//                                            pradMediterranean, pradAsianPacific, pradAfricanCaribbean, pradMale, pradFemale]()
//    {
//        bool error = false;
//        if(pFullName->text() == "")
//            error = true;
//        else if( !( pradEuroCaucasian->isChecked() ||
//                    pradMediterranean->isChecked() ||
//                    pradAsianPacific->isChecked() ||
//                    pradAfricanCaribbean->isChecked() ) )
//                error = true;
//        else if( !(pradMale->isChecked() || pradFemale->isChecked()) )
//            error = true;

//        if(error)
//            QMessageBox::warning(nullptr,
//                                 "Warning",
//                                 "Все поля обязательны для заполнения!",
//                                 QMessageBox::Ok,
//                                 QMessageBox::Ok);
//        else
//            emit pDialog->accept();
//    });
//    connect(pcmdCancel, SIGNAL(clicked()),pDialog, SLOT(reject()));
//    QHBoxLayout* phbxLayout = new QHBoxLayout;
//    phbxLayout->addWidget(pcmdOk);
//    phbxLayout->addWidget(pcmdCancel);
//    pLayout->addLayout(phbxLayout);

//    pDialog->setLayout(pLayout);

//    if (pDialog->exec() == QDialog::Accepted)
//    {
//        _patientRecord.card.ID = pID->text().toUInt();
//        _patientRecord.card.fullName = pFullName->text();
//        _patientRecord.gender  = pradMale->isChecked()?PatientRecord::GenderType::Male:PatientRecord::GenderType::Female;
//        if(pradEuroCaucasian->isChecked())
//            _patientRecord.hairType = PatientRecord::HairType::EuroCaucasian;
//        else if(pradMediterranean->isChecked())
//            _patientRecord.hairType = PatientRecord::HairType::Mediterranean;
//        else if(pradAsianPacific->isChecked())
//            _patientRecord.hairType = PatientRecord::HairType::AsianPacific;
//        else if(pradAfricanCaribbean->isChecked())
//            _patientRecord.hairType = PatientRecord::HairType::AfricanCaribbean;
//        delete pDialog;
//        return 0;
//    }

//    delete pDialog;
//    return 1;
//}
