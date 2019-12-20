#include "tsp_test.h"

TSP_test::TSP_test(QWidget *parent)
    : QMainWindow(parent)
{
    m_pImageBase = new IE_ImageBaseCnt(QDir("/Users/const/Library/Application Support/TrichoSciencePro/data/imageBase"));
    m_pImageBase->getDockWidgetWithAllImages()->show();
    m_pImageBase->setUserChoiceListSize(1);
    m_pImageBase->setCurrentUserChoiceList(0);
    connect(m_pImageBase, &IE_ImageBaseCnt::needLayerTitlesList,
            this, &TSP_test::sendTestLayerTitlesList);
}

TSP_test::~TSP_test()
{
    delete m_pImageBase;
}

void TSP_test::sendTestLayerTitlesList()
{
    QStringList layerTitlesList;
    layerTitlesList << "PeripilarSign_SpikyHair";
    m_pImageBase->makeCorellation(layerTitlesList);
}

