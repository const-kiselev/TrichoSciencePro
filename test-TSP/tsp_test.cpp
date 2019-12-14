#include "tsp_test.h"

TSP_test::TSP_test(QWidget *parent)
    : QMainWindow(parent)
{
    m_pImageBase = new IE_ImageBaseCnt(QDir("/Users/const/Library/Application Support/TrichoSciencePro/data/imageBase"));
    m_pImageBase->getDockWidgetWithAllImages()->show();
    m_pImageBase->setUserChoiceListSize(1);
    m_pImageBase->setCurrentUserChoiceList(0);
}

TSP_test::~TSP_test()
{
    delete m_pImageBase;
}

