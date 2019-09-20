#ifndef IE_TOOLMARKER_H
#define IE_TOOLMARKER_H

#include <QtWidgets>
#include "ie_tool.h"

class IE_Tool_Marker : public QObject, public QGraphicsItem, public ie_tool
{
public:
    explicit        IE_Tool_Marker( QObject* parent=nullptr,
                                    ToolType tt=ToolType::Marker,
                                    MarkerType mt = MarkerType::None);
    ~IE_Tool_Marker()override{}
    QRectF          boundingRect() const                override;

    void            paint(  QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)            override;

    void            mouseFirstPress(QPointF point)      override;
    int             mouseMove(QPointF point)            override;
    virtual void    wheelMode(QWheelEvent *pe)          override {}
    void            release(QPointF point)              override {}

    bool            activateEditMode()                  override {}
    bool            deactivateEditMode()                override {}
    void            makeCompute()                       override {}
    QWidget*        getWidgetPtr()                      override {return nullptr;}

    void    read(const QJsonObject &json)           override;
    void    write(QJsonObject &json)const           override;
private:
    void drawStar(QPainter *painter);
    void drawArrow(QPainter *painter);


    MarkerType _markerType;
    ToolType _toolType;
};

#endif // IE_TOOLMARKER_H
