#include "TrichoSciencePro.h"

TrichoSciencePro::TrichoSciencePro(QObject *parent) : QObject(parent)
{

}

void TrichoSciencePro::start()
{
    pPatientManager = new PatientManager();
    if(pPatientManager->init())
        return;

    connect(pPatientManager, &PatientManager::needToRunImageEditor, this, &TrichoSciencePro::runImageEditor);
    pPatientManager->show();
}

void TrichoSciencePro::runImageEditor(TSP_PatientData patientData, IE_ProfileType ie_type)
{
    if(!patientData.modelDir.isEmpty())
    {
        ImageEditor *pImageEditor = new ImageEditor();
            if( pImageEditor->open(patientData) )
            {
                delete pImageEditor;
                pImageEditor = nullptr;
                return;
            }
            pImageEditor->show();
            connect(pImageEditor, &ImageEditor::wasClosed, [pImageEditor]()
            {
                pImageEditor->close();
                pImageEditor->deleteLater();
                //delete pImageEditor;
            });
    }
    else
    {
        ImageEditor *pImageEditor = new ImageEditor();
        if( pImageEditor->makeNew(patientData, ie_type) )
        {
            delete pImageEditor;
            pImageEditor = nullptr;
            return;
        }
        pImageEditor->show();
        connect(pImageEditor, &ImageEditor::wasClosed, [pImageEditor]()
        {
            pImageEditor->close();
            pImageEditor->deleteLater();
            //delete pImageEditor;
        });
    }
}

//void TrichoSciencePro::runSavedImageEditor(TSP_PatientData patientData)
//{
//
//}


