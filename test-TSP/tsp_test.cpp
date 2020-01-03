#include "tsp_test.h"

#include <QResource>

TSP_test::TSP_test(QWidget *parent)
    : QMainWindow(parent)
{
    m_ie = new ImageEditor();
    TSP_PatientData patientData;
    patientData.modelDir = "/Users/const/Library/Application Support/TrichoSciencePro/data/patients/452479384/IE_models/461477261";
    patientData.modelFilePath="/Users/const/Library/Application Support/TrichoSciencePro/data/patients/452479384/IE_models/461477261/461477261_IE_model.json";
    m_ie->open(patientData);
    m_ie->show();
}

TSP_test::~TSP_test()
{
    delete m_ie;
}



