#ifndef IE_TOOLCALIBRATION_H
#define IE_TOOLCALIBRATION_H

#include <QtWidgets>

#include "toolController.h"
#include "ieView.h"

class IE_ToolCalibration: public QWidget
{
Q_OBJECT
public:
    IE_ToolCalibration(QWidget *parent = nullptr, qreal mIndex = 0);


    qreal getMeasureIndex() const;

protected:
private:
    TSPImageEditorModel *pModel;
//    QGraphicsView *pView;
    TSPImageEditorView *pView;
    ToolsController *pToolController;
    QGraphicsLineItem *line;
    QLineEdit *pMeasureLineEdit;
    qreal measureIndex;

signals:
    void itemsInModelWasChanged();
    void saveChangedMeasureIndex();

private slots:
    void modelItemsControl();
    void openImage();
    qreal calculateMeasure(int measureLenght);



};

#endif // IE_TOOLCALIBRATION_H
