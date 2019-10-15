#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pm_test_patientList.h"
#include "../_header_tsp.h"

class PatientManager : public QMainWindow
{
    Q_OBJECT
public:
    PatientManager(QWidget *parent = nullptr);
    ~PatientManager();
private:
    PM_test_patientList *pTestPatientList;
signals:

    void needToRunNewImageEditor(_Model_patientData patientData);
    void needToRunSavedImageEditor(_Model_patientData patientData);
public slots:

    void addSavedSession(_Model_patientData patientData)
    {emit pTestPatientList->addSavedSession(patientData);}
};

#endif // MAINWINDOW_H
