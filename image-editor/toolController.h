#ifndef TOOLSCONTROLLER_H
#define TOOLSCONTROLLER_H

#include <QtWidgets>
#include "ie_toolLine.h"
#include "ie_modelLayer.h"
#include "ie_toolMarker.h"
#include "ie_toolRuler.h"
#include "ie_header.h"
#include "ie_line_dd.h"
#include "ie_tool_follicularUnit.h"
#include "ie_tool_image.h"



class ToolsController : public QToolBar
{
    Q_OBJECT
public:
    ToolsController();

    IE_ModelLayer *getPActiveTool() const;

    void setPActiveToolForEditing(IE_ModelLayer *);

    ToolType getActiveToolType() const;
    ToolSet getToolSetType() const;
    void setToolSetType(const ToolSet &value);
    void initToolActions();
    void setMeasureIndexInActiveTool(qreal input);
    void resetEditingMode();
    QDockWidget* initInfoDock();

protected:
    virtual void mousePressEvent  (QMouseEvent* pe)override;
    virtual void mouseReleaseEvent(QMouseEvent* pe)override;
    virtual void mouseMoveEvent   (QMouseEvent* pe)override;
    virtual void wheelEvent (QWheelEvent *pe) override;
    virtual void keyPressEvent (QKeyEvent *pe) override;
private:
    IE_ModelLayer *pActiveTool;


    ToolType activeToolType;
    bool doNextMousePressEvent;
    QTableWidget *pDockTableWidget;
    QDockWidget *pDock;
    ToolSet toolSetType;
    QWidget* pEmptyDockInfoWidget;

    bool editModeActive;

    void initToolBarView();
    void changeTool(ToolType tt, QAction* pa);
    void toolEventFilter(QInputEvent *pe);


public slots:
    void updateToolInfoDockWidget();

signals:
    void needMeasureIndex();
    void startUsingNewTool();
    void stopUsingTool();
    void zoomSignal(float delta, QPointF point);

};

#endif // TOOLSCONTROLLER_H

