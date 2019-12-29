#ifndef TSPIMAGEEDITORVIEW_H
#define TSPIMAGEEDITORVIEW_H

#include <QtWidgets>
#include "ie_model.h"
#include "ie_toolController.h"


/*! Восзможности:
     - увеличение/уменьшение масштаба:
             -- можно реализовать не только инстурмент "лупа",
                 но и как в фотошопе (при нажатии на ALT+колесико мыши)

 Zoom относится к отображению, поэтому реализация его должна быть здесь.
 но вызов происходи по средствам технологии сигналов и слотов.Так же стоит учесть тот фак,
 что zoom с использованием клавиши ALt будет обрабатываться здесь! Это логично!
 */

class IE_View : public QGraphicsView
{
Q_OBJECT
public:
    IE_View(IE_Model *pModel);
    ~IE_View() override;

    QString         getStatusBarInfoDataForUser() const;

    ToolsController*getToolsController() const;
    void resize(int w, int h);

    int             checkTheMousePosViaImageBorder(QMouseEvent *pe);

    QPointF         computeSceneRelativelyPosition(const QPointF point);

    IE_Model *      getModel() const;

    QDockWidget *   getDockInfo() const;

    QDockWidget *   getDockDebugInfo() const;
    QDockWidget *   initDockDebugWidget();

    void setToolCntType(ToolSet ts);

signals:
    void statusBarInfoUPD();
    void changedScale(qreal currentScale);
    void changedSizeOfView(int ix, int iy);
    void mousePos(QPointF point);

public slots:
    void zoomSlot(float delta, QPointF point);
    void sceneChanged();
    void changeCurrentViewRect(QRectF rectOfView);

protected:
    void            resizeEvent(QResizeEvent *event) override;

    virtual void    mousePressEvent  (QMouseEvent* pe) override;
    virtual void    mouseReleaseEvent(QMouseEvent* pe) override;
    virtual void    mouseMoveEvent   (QMouseEvent* pe) override;

    virtual void    wheelEvent (QWheelEvent *pe) override;
    virtual void    drawBackground(QPainter *painter, const QRectF &rect) override;
    virtual void    drawForeground(QPainter *painter, const QRectF &rect) override;

    _global_ie *p_ie_global_data() const;
private:
    QRectF currentView;
    QString statusBarData;
    IE_Model *pModel;
    ToolsController *pToolsController;
    QDockWidget *pDockInfo, *pDockDebugInfo;
    QTableWidget *pdockTableWidget, *pDockInfoTable, *pDockLayersWidget;

    qreal currentScale;
    QPoint currentViewedModelLeftTopPoint; // Координата относительно Модели изображения видимой самой верхней левой точки

    _global_ie *_p_ie_global_data;


    void initInfoDock();
    void init();

};

#endif // TSPIMAGEEDITORVIEW_H



