#ifndef PM_PATIENT_H
#define PM_PATIENT_H

#include <QObject>
#include <QMenu>
#include "pm_medicalRecord.h"
#include "pm_documentIndex.h"
//#include "pm_patientWidget.h"

//struct PatientRecord
//{
//    enum HairType {EuroCaucasian, Mediterranean, AsianPacific, AfricanCaribbean};
//    enum GenderType {Male, Female};
//    PatientCard card;
//    QDate registrationDate;
//    HairType hairType;
//    GenderType gender;
//    QDir workDir;
//};

//! \brief обрабатывает всё, что связано с данными о пациенте
//!
class PM_Patient : public QObject
{
    Q_OBJECT
public:
    explicit PM_Patient(QDir workDir = QDir("data/patients"), QObject *parent = nullptr);
    int initAsNew_Dialog();

    static TSP_PatientData getTSP_PatientData(QDir patientDir);
    TSP_PatientData getTSP_PatientData();
    static uint getUIDFromDirName(QDir patientDir);

    QWidget * getWidget();

signals:
    void goBack();
    void needToRunImageEditor(TSP_PatientData patientData, IE_ProfileType ie_type);
    void dataWasChanged();
public slots:
    int open(uint patientUID);
    QList<QMenu*> getAvailableActions();
    void removeAllData(); //! Удаление данных о пациенте c подтверждением
private:
    int init();
    int initAnNew();
    int initWidget();
    int newPatientDialog();
    int loadMedicalRecord();
    int makePatientDirectory();

    PM_MedicalRecordCnt * m_pMedicalRecordCnt;
    PM_DocumentIndexCnt * m_pDocumentIndexCnt;
    QDir m_workDir;
//    PM_PatientWidget * m_pPatientWidget;
    QWidget * m_pWidget;

};

#endif // PM_PATIENT_H
