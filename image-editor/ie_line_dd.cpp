#include "ie_line_dd.h"

IE_Line_DD::IE_Line_DD(QObject *parent):IELine(parent,
                                               ToolType::DensityAndDiameter,
                                               LineSettings::PenWidth)
{

}

void IE_Line_DD::wheelMode(QWheelEvent *pe)
{
    IELine::wheelMode(pe);
    if((int)IE_GLOBAL_DATA.convertWithForamtF(getPenWidth(),
                    (UnitType)IE_GLOBAL_DATA.getIndexThreshold_terminal_wellus_unitType())  >=
                                                                    IE_GLOBAL_DATA.getThreshold_TW())
        pen().setColor(Qt::green), updateSettings();
    else {
        pen().setColor(Qt::red), updateSettings();
    }
}

void IE_Line_DD::read(const QJsonObject &json)
{
    IELine::read(json);
}

void IE_Line_DD::write(QJsonObject &json) const
{
    IELine::write(json);
    json["typeTitle"] = getToolTitle(ToolType::DensityAndDiameter);
}

//void IE_Line_DD::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    paint
//}


