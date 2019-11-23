#include "pm_medicalRecord.h"

#include <QJsonDocument>
#include <QMessageBox>

PM_MedicalRecordCnt::PM_MedicalRecordCnt(QDir patientDir, QObject *parent) : QObject(parent), m_patientDir(patientDir)
{

}

int PM_MedicalRecordCnt::initAsNew(PM_MedicalRecord medRec)
{
    m_medicalRecord.initAsNew( medRec );
    QDir tmpDir = QString("%1/%2").arg(m_patientDir.path()).arg( m_medicalRecord.getPatient_UID() );
    if(tmpDir.path().contains( m_medicalRecord.getPatient_UID() ))
        return 0;
    m_patientDir = tmpDir;
    return 0;
}

TSP_PatientData PM_MedicalRecordCnt::getTSP_PatientData()
{
    TSP_PatientData pd = m_medicalRecord.getPatientData();
    pd.patientDir = m_patientDir.path();
    return pd;
}
// static
TSP_PatientData PM_MedicalRecordCnt::getTSP_PatientData(QDir patientDir)
{
    PM_MedicalRecord mr;
    QJsonObject medicalRecordJsonObj;
    if(readMedicalRecord(patientDir, medicalRecordJsonObj))
    {
        return mr.getPatientData();
    }
    mr.read(medicalRecordJsonObj);
    TSP_PatientData pd = mr.getPatientData();
    pd.patientDir = patientDir.path();
    return pd;
}
// static
uint PM_MedicalRecordCnt::getUIDFromDirName(QDir patientDir)
{
    return patientDir.dirName().toUInt();
}
// static
QString PM_MedicalRecordCnt::getFileName(QDir patientDir)
{
    return QString("medicalRecord_%1.json").arg( getUIDFromDirName(patientDir) );
}

void PM_MedicalRecordCnt::setMedicalRecord(PM_MedicalRecord mr)
{
    m_medicalRecord.initAsNew(mr);
}

int PM_MedicalRecordCnt::save()
{
    if ( !m_patientDir.exists() )
        m_patientDir.mkpath( "." );

    QFile medicalRecordFile;
    medicalRecordFile.setFileName( m_patientDir.filePath( getFileName(m_patientDir) ) );
    if(!medicalRecordFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open file.");
        return 0;
    }
    QJsonObject medicalRecordJSONObj;
    medicalRecordJSONObj["TSP_docType"] = "TSP_JSON_medicalRecord";
    m_medicalRecord.write(medicalRecordJSONObj);
    QJsonDocument saveDoc(medicalRecordJSONObj);
    medicalRecordFile.write(saveDoc.toJson());
    medicalRecordFile.close();

    return 0;
}

int PM_MedicalRecordCnt::open()
{
    QFile medicalRecordFile;
    medicalRecordFile.setFileName( m_patientDir.filePath( getFileName(m_patientDir) ) );

    if(!medicalRecordFile.exists())
        return 1;
    if(!medicalRecordFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось открыть файл %1 с пациентами.")
                                                                .arg(medicalRecordFile.fileName()));
        qWarning("Couldn't open file.");
        return 1;
    }

    QByteArray saveData = medicalRecordFile.readAll();
    medicalRecordFile.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));


    QJsonObject jsonDoc = loadDoc.object();

    if(jsonDoc["TSP_docType"] != "TSP_JSON_medicalRecord")
    {
        qDebug() << medicalRecordFile.fileName() << "isn't TSP_JSON_medicalRecord.";
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось прочитать файл %1 с пациентами.")
                                                                .arg(medicalRecordFile.fileName()));
        return 1;
    }
    return m_medicalRecord.read(jsonDoc);
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

PM_MedicalRecord::PM_MedicalRecord()
{
    m_patient_UID = m_patient_ID = 0;
    m_patient_fullName = "";
    m_registrationDateTime = QDateTime::currentDateTime();
    m_hairType = HairType::EuroCaucasian;
    m_gender = GenderType::Male;
}

void PM_MedicalRecord::initAsNew(PM_MedicalRecord medRec)
{
    if( !medRec.getPatient_UID() )
    {
        m_patient_UID = QDateTime::currentDateTime().toTime_t()- 1560000000;
        m_patient_UID *= 1000;
        m_patient_UID += (QTime::currentTime().msec())%1000;
    }
    else
       m_patient_UID = medRec.getPatient_UID();

    m_patient_ID = medRec.getPatient_ID();
    m_patient_fullName = medRec.getPatient_fullName();
    m_registrationDateTime = medRec.getRegistrationDateTime();
    m_hairType = medRec.getHairType();
    m_gender = getGender();
}

int PM_MedicalRecord::read(const QJsonObject &json)
{
    m_patient_ID = json["patient_ID"].toString().toUInt();
    m_patient_UID = json["patient_UID"].toString().toUInt();
    m_patient_fullName = json["patient_fullName"].toString();
    m_registrationDateTime = QDateTime::fromString( json["registrationDateTime"].toString(), Qt::DateFormat::ISODate );
    m_hairType = (HairType) json["hairType"].toInt();
    m_gender = (GenderType) json["gender"].toInt();
    return 0;
}

int PM_MedicalRecord::write(QJsonObject &json) const
{
    json["patient_ID"] = QString().number(m_patient_ID);
    json["patient_UID"] = QString().number(m_patient_UID);
    json["patient_fullName"] = m_patient_fullName;
    json["registrationDateTime"] = m_registrationDateTime.toString(Qt::DateFormat::ISODate);
    json["hairType"] = m_hairType;
    json["gender"] = m_gender;
    return 0;
}

uint PM_MedicalRecord::getPatient_UID() const
{
    return m_patient_UID;
}

uint PM_MedicalRecord::getPatient_ID() const
{
    return m_patient_ID;
}

void PM_MedicalRecord::setPatient_ID(const uint &patient_ID)
{
    m_patient_ID = patient_ID;
}

QString PM_MedicalRecord::getPatient_fullName() const
{
    return m_patient_fullName;
}

void PM_MedicalRecord::setPatient_fullName(const QString &patient_fullName)
{
    m_patient_fullName = patient_fullName;
}

QDateTime PM_MedicalRecord::getRegistrationDateTime() const
{
    return m_registrationDateTime;
}

void PM_MedicalRecord::setRegistrationDateTime(const QDateTime &registrationDateTime)
{
    m_registrationDateTime = registrationDateTime;
}

PM_MedicalRecord::HairType PM_MedicalRecord::getHairType() const
{
    return m_hairType;
}

void PM_MedicalRecord::setHairType(const HairType &hairType)
{
    m_hairType = hairType;
}

PM_MedicalRecord::GenderType PM_MedicalRecord::getGender() const
{
    return m_gender;
}

void PM_MedicalRecord::setGender(const GenderType &gender)
{
    m_gender = gender;
}
