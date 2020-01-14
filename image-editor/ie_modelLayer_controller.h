#ifndef IE_MODELLAYER_CONTROLLER_H
#define IE_MODELLAYER_CONTROLLER_H

#include <QObject>
#include "ie_modelLayer.h"
#include <QGraphicsScene>

class IE_ModelLayer_Controller : public QObject
{
    Q_OBJECT
public:
    explicit IE_ModelLayer_Controller(QObject *parent = nullptr);
    IE_ConstMLayerListConstPtr
                            getLayerList();
public slots:
    void                    addLayer(IE_ModelLayer* pLayer);
    void                    addLayerViaToolCnt();

    void                    removeLayer(const IE_ModelLayer*pLayer);
    void                    hideLayer(const IE_ModelLayer*pLayer);
    void                    showLayer(const IE_ModelLayer*pLayer);


    void                    layerAction(IE_ModelLayer::Action action,
                                        const IE_ModelLayer* pLayer
                                        );

signals:
    void                    layerListWasChanged();
    void                    newLayerWasAdd(const IE_ModelLayer*pLayer);
    void                    layerWasDeleted(const IE_ModelLayer*pLayer);
private:
    QGraphicsScene * m_pGraphicsScene;
    QList<const IE_ModelLayer*> m_layerList;
};

#endif // IE_MODELLAYER_CONTROLLER_H
