#ifndef IE_TOOLRULER_H
#define IE_TOOLRULER_H

#include "ie_toolLine.h"

// стандартно рулетка работает в пикселях


class IERuler: public IELine
{
public:
    IERuler(QObject* parent=nullptr);
    void wheelMode(QWheelEvent *pe) override;
protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual QRectF boundingRect() const override;

    void    read(const QJsonObject &json)           override;
    void    write(QJsonObject &json)const           override;
private:
    int selectionOffset;
};

#endif // IE_TOOLRULER_H
