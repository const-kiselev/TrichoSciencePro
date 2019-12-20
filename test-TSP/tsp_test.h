#ifndef TSP_TEST_H
#define TSP_TEST_H

#include <QMainWindow>
#include "../image-editor/ie_imageBase.h"

class TSP_test : public QMainWindow
{
    Q_OBJECT

public:
    TSP_test(QWidget *parent = nullptr);
    ~TSP_test();

private:
    IE_ImageBaseCnt * m_pImageBase;
    void sendTestLayerTitlesList();
};
#endif // TSP_TEST_H
