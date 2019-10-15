#ifndef TSPIMAGEEDITORVIEW_H
#define TSPIMAGEEDITORVIEW_H

#include <QtWidgets>
#include "ie_model.h"
#include "ie_toolController.h"

class IE_View : public QGraphicsView
{
Q_OBJECT
public:
    IE_View(IE_Model *pModel);

    QString getStatusBarInfoDataForUser() const;
    IE_Model *getPImgModel() const;
    void setPImgModel(IE_Model *value);

    ToolsController *getPToolsController() const;
    void resize(int w, int h);

    int checkTheMousePosViaImageBorder(QMouseEvent *pe);

    QPointF computeSceneRelativelyPosition(const QPointF point);

    IE_Model *getPModel() const;
    void setPModel(IE_Model *value);

    QDockWidget *getPDockInfo() const;

    QDockWidget *getPDockDebugInfo() const;
    QDockWidget * initDockDebugWidget();



protected:
    void resizeEvent(QResizeEvent *event) override;

    virtual void mousePressEvent  (QMouseEvent* pe) override;
    virtual void mouseReleaseEvent(QMouseEvent* pe) override;
    virtual void mouseMoveEvent   (QMouseEvent* pe) override;

    virtual void wheelEvent (QWheelEvent *pe) override;

    _global_ie *p_ie_global_data() const;
private:
    QString statusBarData;
    IE_Model *pModel;
    ToolsController *pToolsController;
    QDockWidget *pDockInfo, *pDockDebugInfo;
    QTableWidget *pdockTableWidget, *pDockInfoTable, *pDockLayersWidget;

    qreal currentScale;
    QPoint currentViewedModelLeftTopPoint; // Координата относительно Модели изображения видимой самой верхней левой точки

    _global_ie *_p_ie_global_data;

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
