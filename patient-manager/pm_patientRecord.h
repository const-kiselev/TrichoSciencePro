#ifndef PM_PATIENTRECORD_H
#define PM_PATIENTRECORD_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QtWidgets>

/// \todo
// - необходимо организовать проверку несохраненных сессий.



class PM_patientRecord: public QObject
{
    Q_OBJECT
public:
    struct PatientCard
    {
        QString fullName;
        uint UID, ID;
        void reset()
        {
            fullName = "";
            UID = 0;
            ID = 0;
        }
    };

    struct PatientRecord
    {
        enum HairType {EuroCaucasian, Mediterranean, AsianPacific, AfricanCaribbean};
        enum GenderType {Male, Female};
        PatientCard card;
        QDate registrationDate;
        HairType hairType;
        GenderType gender;
        QDir workDir;
    };



    explicit PM_patientRecord(QObject* parent=nullptr);
    explicit PM_patientRecord(uint patient_UID, QObject* parent=nullptr);

    bool initAsNew(bool withUI = true);
    bool initWithUID(uint patient_UID);

    void newSession() {}
    void newVisit() {}
    void save() {}
    static PatientCard readPatientCard(const QJsonObject &json);
    static void writePatientCard( QJsonObject &json, const PatientCard card);
    PatientCard getPatientCard() { return _patientRecord.card; }
private:
    void init(){};

    void readPatientRecord_JSON();
    void makePatientDirectory();
    void writePatientRecord_JSON();

    int newPatientWidget();

    PatientRecord _patientRecord;

};

#endif // PM_PATIENTRECORD_H
