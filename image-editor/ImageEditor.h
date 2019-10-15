#ifndef TSPIMAGEEDITOR_H
#define TSPIMAGEEDITOR_H

#include <QObject>
#include <QtWidgets>
#include "ie_view.h"
#include "ie_calibration.h"


class ImageEditor: public QMainWindow
{
    Q_OBJECT
public:
    explicit ImageEditor();
    ~ImageEditor() override;
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    //QMainWindow *pEditorWidget;

    void init();

    QWidget *pSceneWidget;

    IE_View *pView;
    IE_Model *pModel;
    ToolsController *pIEToolController;

    void menuInit();

    // History Item
private slots:
    void makeCalibration();
public slots:
    void open(_Model_patientData patientData);
    void openNew(_Model_patientData patientData);

signals:
    void wasClosed();
    void wasSaved(_Model_patientData patientData);
};

#endif // TSPIMAGEEDITOR_H

/// реализовать виджет быстрого выбора изображения,
/// где в таблице указываются типы снимков, их даты итд...
