#include "tsp_test.h"

#include <QResource>

TSP_test::TSP_test(QWidget *parent)
    : QMainWindow(parent)
{
    pIE = new ImageEditor();
    TSP_PatientData pd;
    pd.modelDir = "/Users/const/Library/Application Support/TrichoSciencePro/data/patients/3385722153/IE_models/4097427068";
    pd.modelFilePath = "/Users/const/Library/Application Support/TrichoSciencePro/data/patients/3385722153/IE_models/4097427068/4097427068_IE_model.json";
    pIE->open(pd);
    pIE->show();
}

TSP_test::~TSP_test()
{
    delete pIE;
}

void TSP_test::sendTestLayerTitlesList()
{

}

