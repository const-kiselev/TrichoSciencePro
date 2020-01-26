#ifndef IE_Tool_HairStandard_H
#define IE_Tool_HairStandard_H

#include <QtWidgets>
#include "ie_line_dd.h"

//!*
//! \brief Класс интсрумент "эталон"
//!
//! Основная логика: взаимодействует с объектом-интсрументом "линия".
//!
//!
//!
//! \todo 1. Отрисовка
//! \todo 2. Управление, поворот
//! \todo 3. Управление, ЛКМ и ПКЛ
//!
//!

class IE_Tool_HairStandard: public IE_Line_DD
{
public:
    IE_Tool_HairStandard( _global_ie *gi,
                   QObject* parent=nullptr,
                   LineSettings settings = LineSettings::AllSettings );

//    void            mouseFirstPress(QPointF point)  override;
//    int             mouseMove(QPointF point)        override;
    virtual void    wheelMode(QWheelEvent *pe)      override;
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
private:
//    float angle;
    static float angle;

};

#endif // IE_Tool_HairStandard_H
