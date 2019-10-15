#include "ie_fieldOfView.h"

IE_FieldOfView::IE_FieldOfView(QObject *parent) : QObject(parent)
{

}

int IE_FieldOfView::read(const QJsonObject &json)
{
    rectData.setX(json["posX"].toDouble());
    rectData.setY(json["posY"].toDouble());
    rectData.setWidth(json["width"].toDouble());
    rectData.setHeight(json["height"].toDouble());
    return 0;
}

int IE_FieldOfView::write(QJsonObject &json) const
{
    json["posX"] = rectData.x();
    json["posY"] = rectData.y();
    json["width"] = rectData.width();
    json["height"] = rectData.height();
    return 0;
}

void IE_FieldOfView::setMainImage(QString filePath)
{
    //! происходит проверка среди существующих слоев, если такого нет, то создается


}
