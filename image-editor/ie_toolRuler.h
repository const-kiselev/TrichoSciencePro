#ifndef IE_TOOLRULER_H
#define IE_TOOLRULER_H

#include "ie_tool_line.h"

// стандартно рулетка работает в пикселях


class IERuler: public IE_Tool_Line
{
public:
    IERuler(_global_ie *gi, QObject* parent=nullptr);
    void wheelMode(QWheelEvent *pe) override;
protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual QRectF boundingRect() const override;

    int    read(const QJsonObject &json)           override;
    int    write(QJsonObject &json)const           override;
private:
    int selectionOffset;
};

#endif // IE_TOOLRULER_H
