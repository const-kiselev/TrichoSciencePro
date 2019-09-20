#include "TrichoSciencePro.h"

TrichoSciencePro::TrichoSciencePro(QObject *parent) : QObject(parent)
{

}

void TrichoSciencePro::start()
{
    pPatientManager = new PatientManager();

    connect(pPatientManager, &PatientManager::needToRunImageEditor, this, &TrichoSciencePro::runImageEditor);
    pPatientManager->show();
}

void TrichoSciencePro::runImageEditor(_Model_patientData patientData)
{
    TSPimageEditor *pImageEditor = new TSPimageEditor(patientData);
    connect(pImageEditor, &TSPimageEditor::wasSaved,
            pPatientManager, &PatientManager::addSavedSession);
    pImageEditor->show();
//    connect(pImageEditor, &TSPimageEditor::wasClosed, [pImageEditor]()
//    {
//        //delete pImageEditor;
//    });

}
