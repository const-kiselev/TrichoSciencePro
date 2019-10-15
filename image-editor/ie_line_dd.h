#ifndef IE_LINE_DD_H
#define IE_LINE_DD_H

#include "ie_toolLine.h"

//diameter and density — диаметр и плотность
class IE_Line_DD: public IELine
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
};

#endif // IE_LINE_DD_H
