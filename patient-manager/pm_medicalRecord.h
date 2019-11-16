#ifndef PM_MEDICALRECORD_H
#define PM_MEDICALRECORD_H

#include <QObject>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include "../_header_tsp.h"

const char * TSP_JSON_MEDICALRECORD_TSP_docType = "TSP_JSON_medicalRecord";


class PM_MedicalRecord
{
public:
    PM_MedicalRecord()
    {
        m_patient_UID = m_patient_ID = 0;
        m_patient_fullName = "";
    }
    int    read(const QJsonObject &json);
    int    write(QJsonObject &json)const;
    TSP_PatientData getPatientData()
    {
        TSP_PatientData pd;
        pd.patient_ID = m_patient_ID;
        pd.patient_UID = m_patient_UID;
        pd.patient_fullName = m_patient_fullName;
        return pd;
    }
    //! \todo переписать генератор сокращения имени
    QString getNameAlias()
    {
        return m_patient_fullName;
    }
private:

    uint    m_patient_UID,
            m_patient_ID;
    QString m_patient_fullName;

};

class PM_MedicalRecordCnt : public QObject
{
    Q_OBJECT
public:
    explicit PM_MedicalRecordCnt(QObject *parent = nullptr);


    static TSP_PatientData getTSP_PatientData(QDir patientDir);
    static uint getUIDFromDirName(QDir patientDir);
    static QString getFileName(QDir patientDir);

    int    read(const QJsonObject &json);
    int    write(QJsonObject &json)const;

signals:

public slots:
private:
    static int readMedicalRecord(const QDir patientDir,  QJsonObject &json);
};

#endif // PM_MEDICALRECORD_H
