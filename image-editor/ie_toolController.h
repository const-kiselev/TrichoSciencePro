#ifndef TOOLSCONTROLLER_H
#define TOOLSCONTROLLER_H

#include <QtWidgets>
#include "ie_header.h"
#include "ie_modelLayer.h"

#include "ie_toolLine.h"
#include "ie_toolmarker.h"
#include "ie_toolRuler.h"
#include "ie_line_dd.h"
#include "ie_tool_follicularUnit.h"
#include "ie_tool_image.h"



class ToolsController : public QToolBar
{
    Q_OBJECT
public:
    ToolsController(_global_ie *gi);

    IE_ModelLayer *getPActiveTool() const;

    void setPActiveToolForEditing(IE_ModelLayer *);

    ToolType getActiveToolType() const;
    ToolSet getToolSetType() const;
    void setToolSetType(const ToolSet &value);
    void initToolActions();
    void resetEditingMode();
    QDockWidget* initInfoDock();

    void setP_ie_global_data(_global_ie *p_ie_global_data);

    QDockWidget *getPDock() const;

protected:
    virtual void mousePressEvent  (QMouseEvent* pe)override;
    virtual void mouseReleaseEvent(QMouseEvent* pe)override;
    virtual void mouseMoveEvent   (QMouseEvent* pe)override;
    virtual void wheelEvent (QWheelEvent *pe) override;
    virtual void keyPressEvent (QKeyEvent *pe) override;
    void groupItem(ToolType tt, QAction* pa);
    _global_ie *p_ie_global_data() const;
private:
    IE_ModelLayer *pActiveTool;


    ToolType activeToolType;
    bool doNextMousePressEvent;
    QTableWidget *pDockTableWidget;
    QDockWidget *pDock;
    ToolSet toolSetType;
    QWidget* pEmptyDockInfoWidget;

    bool editModeActive;

    _global_ie *_p_ie_global_data;

    void initToolBarView();
    void changeTool(ToolType tt, QAction* pa);
    void toolEventFilter(QInputEvent *pe);


public slots:
    void updateToolInfoDockWidget();
    void resetPActiveTool();

signals:
    void startUsingNewTool();
    void stopUsingTool();
    void zoomSignal(float delta, QPointF point);

};

#endif // TOOLSCONTROLLER_H

