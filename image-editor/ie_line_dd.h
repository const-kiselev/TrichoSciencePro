#ifndef IE_LINE_DD_H
#define IE_LINE_DD_H

#include "ie_tool_line.h"

//! diameter and density — диаметр и плотность
//! \brief класс слоя "плотность, диаметр, длина"
//!
//! \todo стандартно диаметр волоса равен нулю. Но это не означает, что ширина линии будет нуливой. Необходимо ввести минимальную ширину для отображения.
class IE_Line_DD: public IE_Tool_Line
{

public:
    IE_Line_DD(_global_ie *gi, QObject* parent=nullptr);
    ~IE_Line_DD() override{}
    virtual void wheelMode(QWheelEvent *pe) override;

    int    read(const QJsonObject &json)           override;
    int    write(QJsonObject &json)const           override;
protected:
//    virtual void paint(QPainter *painter,
//                       const QStyleOptionGraphicsItem *option,
//                       QWidget *widget) override;
private:
    uint m_hairWidth; //! диаметр волоса хранится в оригинальных единицах измерения изображения, то есть в пикселях
};

#endif // IE_LINE_DD_H
