#include "ie_modelLayer.h"

IE_ModelLayer::IE_ModelLayer(QGraphicsItem * parent):QGraphicsItem(parent),
                                                visible(true),
                                                toolType(ToolType::NoneTool),
                                                layerTitle("")

{
    pTool = dynamic_cast<ie_tool*>(parent);
}

IE_ModelLayer::IE_ModelLayer(ToolType tt,
                             QGraphicsItem *parent):QGraphicsItem(parent),
                                                visible(true),
                                                toolType(tt),
                                                layerTitle("")

{
    pTool = dynamic_cast<ie_tool*>(parent);
}

//IE_ModelLayer::IE_ModelLayer(ToolType tt,
//                             QGraphicsItem *pLayerGraphicsItem):QGraphicsItem(nullptr),
//                                                                visible(true),
//                                                                toolType(tt)
//{
//    this->pLayerGraphicsItem = pLayerGraphicsItem;
//    pTool = dynamic_cast<ie_tool*>(pLayerGraphicsItem);
//}

IE_ModelLayer::~IE_ModelLayer()
{
//    QGraphicsItem::~QGraphicsItem();
//    delete parentItem();
}

QString IE_ModelLayer::toStr(ToolType tt)
{
    switch(tt)
    {
    case ToolType::NoneTool:
        return QString("None");
    case ToolType::Zoom:
        return QString("Лупа");
    case ToolType::MainImage:
        return QString("Основное изображение");
    case ToolType::Image:
        return QString("Изображение");
    case ToolType::Marker:
        return QString("Маркер");
//    case ToolType::MarkerColored:
//        return QString("Цветной маркер");
    case ToolType::Marker_FollicularUnit:
        return QString("Подсчет фолликулярных юнитов");
    case ToolType::SimpleLine:
        return QString("Линия");
    case ToolType::DensityAndDiameter:
        return QString("Подсчет плотности и диаметра в ручном режиме");
    case ToolType::SmartLine:
        return QString("Умная линия");
    case ToolType::Ruler:
        return QString("Линейка");
    default:
        return QString("Неизвестный инструмент");
    }
}

QRectF IE_ModelLayer::boundingRect() const
{
    return parentItem()->boundingRect();
}

void IE_ModelLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //parentItem()->paint(painter, option, widget);
}

QPointF IE_ModelLayer::getPos()
{
    return parentItem()->pos();
}

void IE_ModelLayer::setPos(QPointF pos)
{
    parentItem()->setPos(pos);
}

void IE_ModelLayer::hide()
{
    visible = false;
    parentItem()->hide();
}

void IE_ModelLayer::show()
{
    visible = true;
    parentItem()->show();
}
void IE_ModelLayer::unhide()
{
    visible = true;
    parentItem()->show();
}

ie_tool *IE_ModelLayer::getToolPtr(){
    return pTool;
}



void IE_ModelLayer::read(const QJsonObject &json)
{
    parentItem()->setPos(json["posX"].toDouble(), json["posY"].toDouble());
    visible = json["visible"].toBool();
    if(visible)
        show();
    else
        hide();
    QJsonObject layerData = json["layerData"].toObject();
    pTool->read(layerData);
    toolType = convertToolTitleToToolType(json["typeTitle"].toString());
}

void IE_ModelLayer::write(QJsonObject &json) const
{

    json["posX"] = parentItem()->pos().x();
    json["posY"] = parentItem()->pos().y();
    json["visible"] = visible;
    QJsonObject layerData;
    pTool->write(layerData);
    json["layerData"] = layerData;
    json["typeTitle"] = getToolTitle(toolType);

}


