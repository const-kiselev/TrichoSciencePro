#include "PatientManager.h"

PatientManager::PatientManager(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("TrichoScience Pro :: Менеджер пациентов");
    pTestPatientList = new PM_test_patientList();
    connect(pTestPatientList, &PM_test_patientList::needToRunNewImageEditor,
            this, &PatientManager::needToRunNewImageEditor);
    connect(pTestPatientList, &PM_test_patientList::needToRunSavedImageEditor,
            this, &PatientManager::needToRunSavedImageEditor);
    setCentralWidget(pTestPatientList);
    setMenuBar(pTestPatientList->getMenuBar());


}

PatientManager::~PatientManager()
{

}
