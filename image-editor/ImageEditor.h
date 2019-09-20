#ifndef TSPIMAGEEDITOR_H
#define TSPIMAGEEDITOR_H

#include <QObject>
#include <QtWidgets>
#include "ieView.h"
#include "ie_toolCalibration.h"

class TSPimageEditor: public QMainWindow
{
    Q_OBJECT
public:
    explicit TSPimageEditor();
    TSPimageEditor(_Model_patientData patientData);
    ~TSPimageEditor() override;
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    //QMainWindow *pEditorWidget;

    void init();

    QWidget *pSceneWidget;

    TSPImageEditorView *pView;
    TSPImageEditorModel *pModel;
    ToolsController *pIEToolController;

    void menuInit();

    // History Item
private slots:

public slots:
    void makeCalibration(qreal mIndex);
signals:
    void wasClosed();
    void wasSaved(_Model_patientData patientData);
};

#endif // TSPIMAGEEDITOR_H

/// реализовать виджет быстрого выбора изображения,
/// где в таблице указываются типы снимков, их даты итд...
