#ifndef IE_MODELLAYERCNT_H
#define IE_MODELLAYERCNT_H

#include <QObject>
#include "ie_modelLayer.h"
#include <QGraphicsScene>

class IE_ModelLayerCnt: public QObject
{
    Q_OBJECT
public:
                IE_ModelLayerCnt(QGraphicsScene * pGScene);
    void        addLayer                (IE_ModelLayer* layerToAdd);
    void        showLayer               (const IE_ModelLayer* pLayer);
    void        hideLayer               (const IE_ModelLayer* pLayer);
    void        removeLayer             (const IE_ModelLayer* pLayer);
    void        makeActionWithLayer     (IE_ModelLayer::Action action,
                                            const IE_ModelLayer* pLayer);
    IE_ConstMLayerListConstPtr
                getConstMLayerConstPtr() const;
    IE_ConstMLayerList
                getModelLayerList() const;
signals:
    void layerListWasChanged();
private:
    QGraphicsScene * m_pGScene;
//    QList<IE_ModelLayer*> m_layerList;
    QList<const IE_ModelLayer*> m_constLayerList;


};

#endif // IE_MODELLAYERCNT_H
