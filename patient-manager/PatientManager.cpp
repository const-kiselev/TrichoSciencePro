#include "PatientManager.h"

PatientManager::PatientManager(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("TrichoScience Pro :: Менеджер пациентов");

}

PatientManager::~PatientManager()
{

}

int PatientManager::init()
{
    m_pPatientIndexCnt = new PM_PatientIndexCnt();
    if(m_pPatientIndexCnt->init())
        return 1;
    m_pPatientIndexCntWidget = m_pPatientIndexCnt->getMainWidget();

    setCentralWidget(m_pPatientIndexCntWidget);

    return 0;
}
