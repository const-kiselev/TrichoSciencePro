#ifndef PM_MEDICALRECORD_H
#define PM_MEDICALRECORD_H

#include <QObject>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include "../_header_tsp.h"

static const char * TSP_JSON_MEDICALRECORD_TSP_docType = "TSP_JSON_medicalRecord";


class PM_MedicalRecord
{
public:

    enum HairType
    {
       EuroCaucasian = 0,
       Mediterranean,
       AsianPacific,
       AfricanCaribbean
    };
    enum GenderType {Male = 0, Female};

    PM_MedicalRecord();
    void initAsNew( PM_MedicalRecord medRec );
    int    read(const QJsonObject &json);
    int    write(QJsonObject &json)const;

    TSP_PatientData getPatientData()
    {
        TSP_PatientData pd;
        pd.patient_ID = m_patient_ID;
        pd.patient_UID = m_patient_UID;
        pd.patient_fullName = m_patient_fullName;
        pd.patient_nameAlias = getNameAlias();
        return pd;
    }
    //! \todo переписать генератор сокращения имени

    QString getNameAlias()
    {
        return m_patient_fullName;
    }
    uint getPatient_UID() const;

    uint getPatient_ID() const;
    void setPatient_ID(const uint &patient_ID);

    QString getPatient_fullName() const;
    void setPatient_fullName(const QString &patient_fullName);

    QDateTime getRegistrationDateTime() const;
    void setRegistrationDateTime(const QDateTime &registrationDateTime);

    HairType getHairType() const;
    void setHairType(const HairType &hairType);

    GenderType getGender() const;
    void setGender(const GenderType &gender);

private:
    uint    m_patient_UID,
            m_patient_ID;
    QString m_patient_fullName;
    QDateTime m_registrationDateTime;
    HairType m_hairType;
    GenderType m_gender;
};

class PM_MedicalRecordCnt : public QObject
{
    Q_OBJECT
public:
    explicit PM_MedicalRecordCnt(QDir patientDir, QObject *parent = nullptr);

    int initAsNew(PM_MedicalRecord medRec = PM_MedicalRecord() );


    TSP_PatientData getTSP_PatientData();
    static TSP_PatientData getTSP_PatientData(QDir patientDir);
    static uint getUIDFromDirName(QDir patientDir);
    static QString getFileName(QDir patientDir);

    PM_MedicalRecord getMedicalRecord(){return m_medicalRecord;}
    uint getPatientUID(){return m_medicalRecord.getPatient_UID();}

    void setMedicalRecord(PM_MedicalRecord mr);

    int save();
    int open();

signals:

public slots:
private:
    PM_MedicalRecord m_medicalRecord;
    QDir m_patientDir;

    static int readMedicalRecord(const QDir patientDir,  QJsonObject &json);
};

#endif // PM_MEDICALRECORD_H
