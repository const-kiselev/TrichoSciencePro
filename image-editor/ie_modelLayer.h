#ifndef IE_MODELLAYER_H
#define IE_MODELLAYER_H

#include <QtWidgets>
#include "ie_tool.h"
#include "ie_header.h"


//! \todo добавить статичную функцию, которая возвращает строковый список наименования слоев.
class IE_ModelLayer :public QGraphicsItem
{
public:
    enum Action{
        Show,
        Hide,
        Remove
    };
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

    QPointF getPos();
    void setPos(QPointF pos);

    void hide();
    void show();
    void unhide();

    ie_tool* getToolPtr();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    ie_tool* pTool;
    bool visible;
    ToolType toolType;
    QString layerTitle;
    //QGraphicsItem *pLayerGraphicsItem;
};
Q_DECLARE_METATYPE(IE_ModelLayer*)

typedef QList<const IE_ModelLayer*> IE_ConstMLayerList;
typedef const IE_ConstMLayerList* IE_ConstMLayerListConstPtr;
typedef QListIterator<const IE_ModelLayer*> IE_MLayerListIter; // итератор списка
typedef QList<const IE_ModelLayer*>::iterator IE_MLIter; // итератор на константный элемент списка

#endif // IE_MODELLAYER_H
