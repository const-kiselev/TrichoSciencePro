#ifndef TSPIMAGEEDITORVIEW_H
#define TSPIMAGEEDITORVIEW_H

#include <QtWidgets>
#include "ieModel.h"
#include "toolController.h"

class TSPImageEditorView : public QGraphicsView
{
Q_OBJECT
public:
    TSPImageEditorView();
    TSPImageEditorView(TSPImageEditorModel *pModel);

    QString getStatusBarInfoDataForUser() const;
    TSPImageEditorModel *getPImgModel() const;
    void setPImgModel(TSPImageEditorModel *value);

    ToolsController *getPToolsController() const;
    void resize(int w, int h);

    int checkTheMousePosViaImageBorder(QMouseEvent *pe);

    QPointF computeSceneRelativelyPosition(const QPointF point);

    TSPImageEditorModel *getPModel() const;
    void setPModel(TSPImageEditorModel *value);

    QDockWidget *getPDockInfo() const;

    QDockWidget *getPDockDebugInfo() const;
    QDockWidget * initDockDebugWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;

    virtual void mousePressEvent  (QMouseEvent* pe) override;
    virtual void mouseReleaseEvent(QMouseEvent* pe) override;
    virtual void mouseMoveEvent   (QMouseEvent* pe) override;

    virtual void wheelEvent (QWheelEvent *pe) override;

private:
    QString statusBarData;
    TSPImageEditorModel *pModel;
    ToolsController *pToolsController;
    QDockWidget *pDockInfo, *pDockDebugInfo;
    QTableWidget *pdockTableWidget, *pDockInfoTable, *pDockLayersWidget;

    qreal currentScale;
    QPoint currentViewedModelLeftTopPoint; // Координата относительно Модели изображения видимой самой верхней левой точки

//    QLabel  *pImageOfModelSize, *pZoomScale,
//            *pCurrentSizeOfViewWidget, *pLabelMousePos,
//            *pLabelMouseLocalPos, *pLabelMousePosMapToScene;


    void initInfoDock();
    void init();
signals:
    void statusBarInfoUPD();
    void changedScale(qreal currentScale);
    void changedSizeOfView(int ix, int iy);
    void mousePos(QPointF point);
public slots:
    void zoomSlot(float delta, QPointF point);
    void sceneChanged();
};

#endif // TSPIMAGEEDITORVIEW_H


/// Восзможности:
///     - увеличение/уменьшение масштаба:
///             -- можно реализовать не только инстурмент "лупа",
///                 но и как в фотошопе (при нажатии на ALT+колесико мыши)
///
/// Zoom относится к отображению, поэтому реализация его должна быть здесь.
/// но вызов происходи по средствам технологии сигналов и слотов.Так же стоит учесть тот фак,
/// что zoom с использованием клавиши ALt будет обрабатываться здесь! Это логично!
