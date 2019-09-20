#ifndef IE_TOOL_FOLLICULARUNIT_H
#define IE_TOOL_FOLLICULARUNIT_H

#include <QtWidgets>
#include "ie_tool.h"


class IE_Tool_FollicularUnit : public QObject, public QGraphicsSimpleTextItem, public ie_tool
{
public:
    explicit IE_Tool_FollicularUnit(QObject* parent=nullptr,
                            ToolType tt=ToolType::Marker);
    ~IE_Tool_FollicularUnit() override {}
    void numInc();
    void numDec();

    void mouseFirstPress(QPointF point) override;
    int mouseMove(QPointF point)        override {}
    void wheelMode(QWheelEvent *pe)     override;
    void release(QPointF point)         override {}

    bool activateEditMode()             override;
    bool deactivateEditMode()           override;
    void makeCompute()                  override {}
    QWidget* getWidgetPtr()             override {return nullptr;}
    void    read(const QJsonObject &json)           override;
    void    write(QJsonObject &json)const           override;

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
private:
    QColor backgroundColor, textColor;
    QString str;
    qint8 num;
};

#endif // IE_TOOL_FOLLICULARUNIT_H
