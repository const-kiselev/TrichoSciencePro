#include "PatientManager.h"

#include <QMenuBar>

PatientManager::PatientManager(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("TrichoScience Pro :: Менеджер пациентов");
    setMenuBar(new QMenuBar());
}

PatientManager::~PatientManager()
{

}

int PatientManager::init()
{
    m_pPatientIndexCnt = new PM_PatientIndexCnt();
    connect(m_pPatientIndexCnt, &PM_PatientIndexCnt::availableActionsWasChanged, [this]()
    {
        QMenuBar * pMenuBar = this->menuBar();
        pMenuBar->clear();
        QList<QMenu*> menuList = m_pPatientIndexCnt->getAvailableActions();
        foreach(QMenu * subMenu, menuList)
            pMenuBar->addMenu(subMenu);
        this->setMenuBar(pMenuBar);
    });

    connect(m_pPatientIndexCnt, &PM_PatientIndexCnt::needToRunImageEditor, [this](TSP_PatientData patientData, IE_ProfileType ie_type)
    {
        emit needToRunImageEditor(patientData, ie_type);
    });

    if(m_pPatientIndexCnt->init())
        return 1;
    m_pPatientIndexCntWidget = m_pPatientIndexCnt->getMainWidget();

    setCentralWidget(m_pPatientIndexCntWidget);

    return 0;
}
