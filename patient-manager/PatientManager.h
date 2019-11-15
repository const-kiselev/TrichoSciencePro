#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../_header_tsp.h"
#include "pm_patientIndexCnt.h"

class PatientManager : public QMainWindow
{
    Q_OBJECT
public:
    PatientManager(QWidget *parent = nullptr);
    ~PatientManager();

    int init();


signals:

    void needToRunNewImageEditor(_Model_patientData patientData);
    void needToRunSavedImageEditor(_Model_patientData patientData);
public slots:



private:
    QWidget         * m_pPatientIndexCntWidget;
    PM_PatientIndexCnt * m_pPatientIndexCnt;
};

#endif // MAINWINDOW_H
