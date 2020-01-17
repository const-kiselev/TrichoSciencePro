#ifndef IE_MODELLAYER_H
#define IE_MODELLAYER_H

#include <QtWidgets>
#include "ie_tool.h"
#include "ie_header.h"


//! \todo добавить статичную функцию, которая возвращает строковый список наименования слоев.
class IE_ModelLayer :public QGraphicsItem
{
public:
    typedef const IE_ModelLayer* PublicType;
    typedef QList<PublicType> PublicList;
    typedef const PublicList* PublicConstPtrToList;
    typedef QListIterator<PublicType> IteratorToPublicList; // итератор списка
    typedef PublicList::iterator PublicListIter; // итератор на константный элемент списка


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
    ToolType getToolType() const {return toolType;}
    bool isVisible() const {return visible;}

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) override;

    QPointF getPos() const;
    void setPos(QPointF pos);

    void hide();
    void show();
    void unhide();

    ie_tool* getToolPtr() const;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    void makeAction(Action act);

private:
    ie_tool* pTool;
    bool visible;
    ToolType toolType;
    QString layerTitle;
    //QGraphicsItem *pLayerGraphicsItem;
};
Q_DECLARE_METATYPE(IE_ModelLayer*)


//typedef const IE_ModelLayer* IE_ModelLayer::PublicType;
//typedef QList<IE_ModelLayer::PublicType> IE_ModelLayer::PublicList;
//typedef const IE_ModelLayer::PublicList* IE_ModelLayer::PublicListConstPtr;
//typedef QListIterator<IE_ModelLayer::PublicType> IE_MLayerListIter; // итератор списка
//typedef QList<const IE_ModelLayer*>::iterator PublicListIter; // итератор на константный элемент списка

Q_DECLARE_METATYPE(IE_ModelLayer::PublicType)

#endif // IE_MODELLAYER_H
