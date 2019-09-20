#ifndef IE_TOOL_H
#define IE_TOOL_H

#include <QObject>
#include <QDebug>
//#include <QGraphicsItem>
#include "ie_header.h"
#include <QJsonObject>


class ie_tool
{

public:
//    ie_tool();
//    virtual ~ie_tool();
    virtual void    mouseFirstPress(QPointF)    =0;
    virtual int     mouseMove(QPointF)          =0;
    virtual void    wheelMode(QWheelEvent*)     =0;
    virtual void    release(QPointF)            =0;


//    virtual ToolType getToolType()=0;


    virtual QWidget*getWidgetPtr()              =0;
    virtual void    makeCompute()               =0;
    virtual bool    activateEditMode()          =0;
    virtual bool    deactivateEditMode()        =0;

    virtual void    read(const QJsonObject &json) =0;
    virtual void    write(QJsonObject &json)const =0;


};


#endif // IE_TOOL_H
