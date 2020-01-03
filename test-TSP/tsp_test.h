#ifndef TSP_TEST_H
#define TSP_TEST_H

#include <QMainWindow>
#include "../image-editor/ImageEditor.h"

class TSP_test : public QMainWindow
{
    Q_OBJECT

public:
    TSP_test(QWidget *parent = nullptr);
    ~TSP_test();

private:
    ImageEditor *m_ie;
};
#endif // TSP_TEST_H
