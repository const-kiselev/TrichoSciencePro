#include "ie_tool_hairStandard.h"


IE_Tool_HairStandard::IE_Tool_HairStandard(_global_ie *gi,
                             QObject *parent,
                             LineSettings settings): IE_Line_DD(gi,
                                                            parent
                                                            )
{

}

//void IE_Tool_HairStandard::mouseFirstPress(QPointF point)
//{

//}

//int IE_Tool_HairStandard::mouseMove(QPointF point)
//{

//}

void IE_Tool_HairStandard::wheelMode(QWheelEvent *pe)
{
    //! \todo изменение угла наклона прямой
    //!
    //!
    static QTime time = QTime::currentTime();
    if(QTime::currentTime().msec() - time.msec()  < 10)
        return;
    time = QTime::currentTime();


    IE_Tool_Line::wheelMode(pe);
    QPen locPen(pen());
    IE_Tool_Line::wheelMode(pe);
    if(p_ie_global_data()->convertF(locPen.widthF())
            >= p_ie_global_data()->getThreshold_TW())
        locPen.setColor(Qt::green);
    else {
        locPen.setColor(Qt::red);
    }
    setPen(locPen);
    updateSettings();
}

//void IE_Tool_HairStandard::release(QPointF point)
//{

//}

void IE_Tool_HairStandard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
