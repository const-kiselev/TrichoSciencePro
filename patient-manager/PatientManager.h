#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../_header_tsp.h"
#include "pm_patientIndex.h"

class PatientManager : public QMainWindow
{
    Q_OBJECT
public:
    PatientManager(QWidget *parent = nullptr);
    ~PatientManager();

    int init();


signals:

    void needToRunImageEditor(TSP_PatientData patientData, IE_ProfileType ie_type);
public slots:
    void updateDocumentData(TSP_PatientData patientData);


private:
    QWidget         * m_pPatientIndexCntWidget;
    PM_PatientIndexCnt * m_pPatientIndexCnt;
};

#endif // MAINWINDOW_H
