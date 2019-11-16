#include "pm_medicalRecord.h"

#include <QJsonDocument>
#include <QMessageBox>

PM_MedicalRecordCnt::PM_MedicalRecordCnt(QObject *parent) : QObject(parent)
{

}

TSP_PatientData PM_MedicalRecordCnt::getTSP_PatientData(QDir patientDir)
{
    PM_MedicalRecord mr;
    QJsonObject medicalRecordJsonObj;
    if(readMedicalRecord(patientDir, medicalRecordJsonObj))
    {
        return mr.getPatientData();
    }
    mr.read(medicalRecordJsonObj);
    return mr.getPatientData();
}

uint PM_MedicalRecordCnt::getUIDFromDirName(QDir patientDir)
{
    return patientDir.dirName().toUInt();
}

QString PM_MedicalRecordCnt::getFileName(QDir patientDir)
{
    return QString("medicalRecord_%1.json").arg( getUIDFromDirName(patientDir) );
}

// static
int PM_MedicalRecordCnt::readMedicalRecord(const QDir patientDir, QJsonObject &json)
{
    QFile medicalRecordFile;
    medicalRecordFile.setFileName( patientDir
                                  .filePath( getFileName(patientDir) )
                                  );
    if(!medicalRecordFile.exists())
    {
        qWarning( "PM_MedicalRecordCnt::readMedicalRecord ->> File doesn't exist." ) ;
    }
    if(!medicalRecordFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось открыть медицинскую карту %1.")
                                                                .arg(medicalRecordFile.fileName()));
        qWarning("Couldn't open file.");
        return 1;
    }

    QByteArray saveData = medicalRecordFile.readAll();
    medicalRecordFile.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject jsonObj = loadDoc.object();
    if(jsonObj["TSP_docType"] != TSP_JSON_MEDICALRECORD_TSP_docType)
    {
        qDebug() << medicalRecordFile.fileName() << "isn't " << TSP_JSON_MEDICALRECORD_TSP_docType << ".";
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось прочитать файл %1 с пациентами.")
                                                                .arg(medicalRecordFile.fileName()));
        return 1;
    }
    json = jsonObj;

    return 0;
}

int PM_MedicalRecord::read(const QJsonObject &json)
{
    m_patient_ID = json["patient_ID"].toString().toUInt();
    m_patient_UID = json["patient_UID"].toString().toUInt();
    m_patient_fullName = json["patient_fullName"].toString();
}

int PM_MedicalRecord::write(QJsonObject &json) const
{
    json["patient_ID"] = QString().number(m_patient_ID);
    json["patient_UID"] = QString().number(m_patient_UID);
    json["patient_fullName"] = m_patient_fullName;
}
