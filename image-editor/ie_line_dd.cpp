#include "ie_line_dd.h"

IE_Line_DD::IE_Line_DD(_global_ie *gi, QObject *parent):IELine(gi,parent,
                                               ToolType::DensityAndDiameter,
                                               LineSettings::PenWidth),
                                                        m_hairWidth( .0 )
{
    QPen locPen(pen());
    locPen.setColor(Qt::red);
    setPen(locPen);
}

void IE_Line_DD::wheelMode(QWheelEvent *pe)
{
    //! \todo если значение меньше минимального, обнулять диматер
    IELine::wheelMode(pe);
    QPen locPen(pen());
    IELine::wheelMode(pe);
    if(p_ie_global_data()->convertF(locPen.widthF())
            >= p_ie_global_data()->getThreshold_TW())
        locPen.setColor(Qt::green);
    else {
        locPen.setColor(Qt::red);
    }
    setPen(locPen);
    updateSettings();
}

int IE_Line_DD::read(const QJsonObject &json)
{
    return IELine::read(json);
}

int IE_Line_DD::write(QJsonObject &json) const
{
    int answ;
    if(answ = IELine::write(json))
        return answ;
    json["typeTitle"] = getToolTitle(ToolType::DensityAndDiameter);
    return 0;
}

//void IE_Line_DD::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    paint
//}


