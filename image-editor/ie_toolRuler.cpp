#include "ie_toolRuler.h"

IERuler::IERuler(QObject *parent):IELine(parent, ToolType::Ruler, LineSettings::None)
{
    selectionOffset = 10;
}

void IERuler::wheelMode(QWheelEvent *pe)
{

}

void IERuler::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    IELine::paint(painter, option, widget);
    QPen locPen = painter->pen();
    locPen.setWidth(1);
    painter->setPen(locPen);
    qreal radAngle = line().angle()* M_PI / 180;
    qreal dx = selectionOffset * sin(radAngle);
    qreal dy = selectionOffset * cos(radAngle);
    QPointF offset1 = QPointF(dx, dy);
    QPointF offset2 = QPointF(-dx, -dy);

    painter->drawLine(line().p1() + offset1, line().p1() + offset2);
    painter->drawLine(line().p2() + offset2, line().p2() + offset1);
    widget->update();
}

QRectF IERuler::boundingRect() const
{
    qreal radAngle = line().angle()* M_PI / 180;
    qreal dx = selectionOffset * sin(radAngle);
    qreal dy = selectionOffset * cos(radAngle);
    QPointF offset1 = QPointF(dx, dy);
    QPointF offset2 = QPointF(-dx, -dy);
    QPolygonF polf;
    polf << line().p1() + offset1
                 << line().p1() + offset2
                 << line().p2() + offset2
                 << line().p2() + offset1;
    return polf.boundingRect();

}

void IERuler::read(const QJsonObject &json)
{
    IELine::read(json);
}

void IERuler::write(QJsonObject &json) const
{
    IELine::write(json);
    json["typeTitle"] = getToolTitle(ToolType::Ruler);
}


