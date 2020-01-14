#ifndef IE_TOOL_GAUGE_H
#define IE_TOOL_GAUGE_H

#include <QtWidgets>
#include "ie_tool.h"

class IE_Tool_Gauge: public QObject, public QGraphicsLineItem, public ie_tool
{
public:
    IE_Tool_Gauge();

//    void            mouseFirstPress(QPointF point)  override;
//    int             mouseMove(QPointF point)        override;
//    virtual void    wheelMode(QWheelEvent *pe)      override;
//    void            release(QPointF point)          override;

//    bool            activateEditMode()              override;
//    bool            deactivateEditMode()            override;
//    void            makeCompute()                   override;
//    QWidget*        getWidgetPtr()                  override;


//    int    read(const QJsonObject &json)           override;
//    int    write(QJsonObject &json)const           override;

protected:
//    QRectF          boundingRect() const                override;

//    virtual void    paint(  QPainter *painter,
//                            const QStyleOptionGraphicsItem *option,
//                            QWidget *widget)        override;

};

#endif // IE_TOOL_GAUGE_H
