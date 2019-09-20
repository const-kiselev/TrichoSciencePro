#ifndef IE_MODELLAYER_H
#define IE_MODELLAYER_H

#include <QtWidgets>
#include "ie_tool.h"
#include "ie_header.h"

class IE_ModelLayer :public QGraphicsItem
{

public:
    IE_ModelLayer(QGraphicsItem *parent = nullptr);
    IE_ModelLayer(ToolType tt, QGraphicsItem *parent = nullptr);
//    explicit IE_ModelLayer(ToolType tt, QGraphicsItem *pLayerGraphicsItem);
    ~IE_ModelLayer() override;
    static QString toStr(ToolType tt);
    ToolType getToolType(){return toolType;}
    bool isVisible(){return visible;}

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) override;

    void hide();
    void unhide();

    ie_tool* getToolPtr();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    ie_tool* pTool;
    bool visible;
    ToolType toolType;
    //QGraphicsItem *pLayerGraphicsItem;
};

#endif // IE_MODELLAYER_H
