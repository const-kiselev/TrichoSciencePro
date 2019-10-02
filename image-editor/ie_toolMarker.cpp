#include "ie_toolMarker.h"

IE_Tool_Marker::IE_Tool_Marker( QObject* parent,
                                ToolType tt, MarkerType mt):QObject(parent),
                                                            QGraphicsItem (nullptr),
                                                            ie_tool(),
                                                            _markerType(mt),
                                                            _toolType(tt)
{

}

QRectF IE_Tool_Marker::boundingRect() const
{
    return QRectF(0, 0, 10, 10);
}

void IE_Tool_Marker::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{

    QPen locPen = painter->pen();
    switch (_toolType)
    {
    case ToolType::PeripilarSign_SpikyHair:
    {
        locPen.setColor(Qt::blue);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawArrow(painter);
        break;
    }
    case ToolType::PeripilarSign_ExclamationHair:
    {
        locPen.setColor(Qt::black);
        locPen.setWidth(4);
        painter->setPen(locPen);
        painter->drawLine(0,0, 0, -4);
        painter->drawPoint(0,7);
        break;
    }
    case ToolType::PeripilarSign_BrokenHair:
    {
        QColor color(0, 0, 0);
        locPen.setColor(color);
        locPen.setWidth(2);
        painter->setPen(locPen);
        painter->drawLine(0,0, -8, -8);
        painter->drawLine(0,0, -8, 8);
        locPen.setWidth(6);
        painter->drawPoint(-8,8);
        break;
    }
    case ToolType::PeripilarSign_CadaverizedHair:
    {
        QColor color(224, 128, 64);
        locPen.setColor(color);
        locPen.setWidth(11);
        painter->setPen(locPen);
        painter->drawEllipse(-6, -4, 12, 8);
        color.setRgb(0,0,0);
        locPen.setColor(color);
        locPen.setWidth(2);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    case ToolType::PeripilarSign_YellowDot:
    {
        locPen.setColor(Qt::yellow);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawStar(painter);
        break;
    }
    case ToolType::PeripilarSign_RedDot:
    {
        locPen.setColor(Qt::red);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawStar(painter);
        break;
    }
    case ToolType::PeripilarSign_WhiteDot:
    {
        locPen.setColor(Qt::white);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawStar(painter);
        break;
    }
    case ToolType::PeripilarSign_Hyperpigmentation:
    {
        locPen.setColor(Qt::black);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawStar(painter);
        break;
    }
    case ToolType::RootType_Anagen:
    {
        QColor color(0, 115,74);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    case ToolType::RootType_DysplasticAnagen:
    {
        QColor color(0, 255,0);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    case ToolType::RootType_BrokenAnagen:
    {
        QColor color(0,189,247);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    case ToolType::RootType_AnagenWithPapilla:
    {
        QColor color(255,0,0);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    case ToolType::RootType_Telogen:
    {
        QColor color(0, 0, 255);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    case ToolType::RootType_Catagen:
    {
        QColor color(255, 255, 0);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    case ToolType::RootType_Dystrophic:
    {
        QColor color(99, 58, 16);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }

    default:
    {

    }
    }
    widget->update();
}

void IE_Tool_Marker::mouseFirstPress(QPointF point)
{
    setPos(point);
}

int IE_Tool_Marker::mouseMove(QPointF point)
{

}

void IE_Tool_Marker::read(const QJsonObject &json)
{
    _toolType = convertToolTitleToToolType(json["typeTitle"].toString());
}

void IE_Tool_Marker::write(QJsonObject &json) const
{
    json["typeTitle"] = getToolTitle(_toolType);
}

void IE_Tool_Marker::drawStar(QPainter *painter)
{
    int selectionOffset = 6;
    qreal radAngle = 60 * M_PI / 180;
    qreal dx = selectionOffset * sin(radAngle);
    qreal dy = selectionOffset * cos(radAngle);
    painter->drawLine(0,0, 0, -selectionOffset);
    painter->drawLine(0,0, dx, -dy);
    painter->drawLine(0,0, -dx, -dy);
    radAngle = 45 * M_PI / 180;
    dx = selectionOffset * sin(radAngle);
    dy = selectionOffset * cos(radAngle);
    painter->drawLine(0,0, -dx, dy);
    painter->drawLine(0,0, dx, dy);
}

void IE_Tool_Marker::drawArrow(QPainter *painter)
{
    int selectionOffset = 15;
    qreal radAngle = 45 * M_PI / 180;
    qreal dx1,dx = selectionOffset * sin(radAngle);
    qreal dy1,dy = selectionOffset * cos(radAngle);
    painter->drawLine(-dx, dy, dx, -dy);

    radAngle = 15 * M_PI / 180;
    selectionOffset = 7;
    dx1 = selectionOffset * sin(radAngle);
    dy1 = selectionOffset * cos(radAngle);

    painter->drawLine(dx, -dy, dx-dx1, -dy+dy1);

    radAngle = 75 * M_PI / 180;
    dx1 = selectionOffset * sin(radAngle);
    dy1 = selectionOffset * cos(radAngle);
    painter->drawLine(dx, -dy, dx-dx1, -dy+dy1);
}




