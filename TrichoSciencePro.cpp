#include "TrichoSciencePro.h"

TrichoSciencePro::TrichoSciencePro(QObject *parent) : QObject(parent)
{

}

void TrichoSciencePro::start()
{
    pPatientManager = new PatientManager();

    connect(pPatientManager, &PatientManager::needToRunNewImageEditor, this, &TrichoSciencePro::runNewImageEditor);
    connect(pPatientManager, &PatientManager::needToRunSavedImageEditor, this, &TrichoSciencePro::runSavedImageEditor);
    pPatientManager->show();
}

void TrichoSciencePro::runNewImageEditor(_Model_patientData patientData)
{
    ImageEditor *pImageEditor = new ImageEditor();
    pImageEditor->openNew(patientData);
    connect(pImageEditor, &ImageEditor::wasSaved,
            pPatientManager, &PatientManager::addSavedSession);
    pImageEditor->show();
    connect(pImageEditor, &ImageEditor::wasClosed, [pImageEditor]()
    {
        pImageEditor->close();
        pImageEditor->deleteLater();
        //delete pImageEditor;
    });
}

void TrichoSciencePro::runSavedImageEditor(_Model_patientData patientData)
{
    ImageEditor *pImageEditor = new ImageEditor();
    pImageEditor->open(patientData);
    pImageEditor->show();
    connect(pImageEditor, &ImageEditor::wasClosed, [pImageEditor]()
    {
        pImageEditor->close();
        pImageEditor->deleteLater();
        //delete pImageEditor;
    });
}


