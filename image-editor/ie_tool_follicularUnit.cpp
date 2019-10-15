#include "ie_tool_follicularUnit.h"

IE_Tool_FollicularUnit::IE_Tool_FollicularUnit(QObject* parent,
                             ToolType tt):  QObject(parent),
                                            QGraphicsSimpleTextItem (nullptr),
                                            ie_tool()
{
    num = 1;
    backgroundColor = QColor(0, 184, 222, 255);
    textColor = Qt::black;
    str = QString().number(num);
    setText(str);
}

void IE_Tool_FollicularUnit::numInc()
{
    static const qint8 max = 30;
    if(num==max)
        return;
    num++;
    str = QString().number(num);
    setText(str);
}

void IE_Tool_FollicularUnit::numDec()
{
    if(num==1)
        return;
    num--;
    str = QString().number(num);
    setText(str);
}

void IE_Tool_FollicularUnit::mouseFirstPress(QPointF point)
{
    setPos(point);
}



void IE_Tool_FollicularUnit::wheelMode(QWheelEvent *pe)
{
    QPoint delta = pe->pixelDelta();
    if(delta.ry()>0)
        numInc();
    else
        numDec();
}

bool IE_Tool_FollicularUnit::activateEditMode()
{

}

bool IE_Tool_FollicularUnit::deactivateEditMode()
{

}

int IE_Tool_FollicularUnit::read(const QJsonObject &json)
{
    setText(json["text"].toString());
    num = json["text"].toInt();
    return 0;
}

int IE_Tool_FollicularUnit::write(QJsonObject &json) const
{
    json["typeTitle"] = getToolTitle(ToolType::Marker_FollicularUnit);
    json["text"] = text();
    return 0;
}

void IE_Tool_FollicularUnit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush( backgroundColor );
    painter->drawRect( boundingRect() );
    QGraphicsSimpleTextItem::paint( painter, option, widget );
}
