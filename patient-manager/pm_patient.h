#ifndef PM_PATIENT_H
#define PM_PATIENT_H

#include <QObject>
#include "pm_medicalRecord.h"

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
    explicit PM_Patient(QObject *parent = nullptr);
    void makeAsNew_Dialog();


    int save();
    int open();

    static TSP_PatientData getTSP_PatientData(QDir patientDir);
    static uint getUIDFromDirName(QDir patientDir);

signals:

public slots:
private:

};

#endif // PM_PATIENT_H
