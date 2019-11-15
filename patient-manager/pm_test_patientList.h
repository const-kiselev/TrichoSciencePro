#ifndef PM_TEST_PATIENTLIST_H
#define PM_TEST_PATIENTLIST_H

/// \todo :
// - при создании базы пациентов проводить ревизию существующих директорий и из них извлекать данные.

//#include <QWidget>
#include <QtWidgets>
#include "pm_patientRecordCnt.h"
#include "../_header_tsp.h"

class PM_test_patientList: public QWidget
{
    Q_OBJECT
public:
    PM_test_patientList(QWidget *parent = nullptr);
    QMenuBar * getMenuBar() {return m_pMenuBar;}

private:
    QMenuBar *m_pMenuBar;

    QListWidget *pFullPatientList,
                *pPatientSessions;
    QVBoxLayout *pPatientActions;

    QList<PM_patientRecord::PatientCard> patientList;
    PM_patientRecord::PatientCard currentPatientCard;

    QPushButton *m_pButtonCreateSession,
                *m_pButtonNewVisit,
                *m_pButtonPatientRecord;

    void readPatientBase();
    void writePatientBase();
    void addPatientToJSONBase(const PM_patientRecord::PatientCard card);
    void initMenuBar();

    void activatePatientActions(bool activate = true);

    void readPatientSessionList(QString sessionsWorkPath);
    void writePatientSessionBase(QString sessionsWorkPath);

signals:
    void needToRunNewImageEditor(_Model_patientData patientData);
    void needToRunSavedImageEditor(_Model_patientData patientData);
private slots:
    void createNewPatient();
    void currentPatientInListWasChanged(int row);

    void runImageEditorNew();
    void runImageEditorWithModel(QString modelUID);

    //void initImageEditorRun();
public slots:
    void addSavedSession(_Model_patientData patientData);

};

#endif // PM_TEST_PATIENTLIST_H
