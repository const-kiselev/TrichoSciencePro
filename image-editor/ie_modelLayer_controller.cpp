#include "ie_modelLayer_controller.h"

IE_ModelLayerCnt::IE_ModelLayerCnt(QGraphicsScene *pGScene): QObject(nullptr), m_pGScene(pGScene)
{

}

void IE_ModelLayerCnt::addLayer(IE_ModelLayer *layerToAdd)
{
    for (IE_MLIter iter = m_constLayerList.begin();iter!=m_constLayerList.end();iter++)
        if((*iter) == layerToAdd)
        {
            qWarning() << "Tring to add existed layer.";
            return;
        }
    m_pGScene->addItem(layerToAdd->parentItem());
    m_constLayerList.append(layerToAdd);
}

void IE_ModelLayerCnt::showLayer(const IE_ModelLayer *pLayer)
{

}

void IE_ModelLayerCnt::hideLayer(const IE_ModelLayer *pLayer)
{
    QGraphicsItem *pGraphicsItem;

    for (IE_MLIter iter = m_constLayerList.begin();iter!=m_constLayerList.end();iter++)
    {
        if((*iter) == pLayer)
        {
            const_cast<IE_ModelLayer*>(*iter)->hide();
            break;
        }
    }
}

void IE_ModelLayerCnt::removeLayer(const IE_ModelLayer *pLayer)
{
    QGraphicsItem *pGraphicsItem;

    for (IE_MLIter iter = m_constLayerList.begin();iter!=m_constLayerList.end();iter++)
    {
        if((*iter) == pLayer)
        {
            pGraphicsItem = (QGraphicsItem*) *iter;
            m_pGScene->removeItem(pGraphicsItem->parentItem());
            delete pGraphicsItem;
            pGraphicsItem = nullptr;
            m_constLayerList.erase(iter);
            emit layerListWasChanged();
//            pToolCnt->resetPActiveTool();
            break;
        }
    }
}

void IE_ModelLayerCnt::makeActionWithLayer(IE_ModelLayer::Action action, const IE_ModelLayer *pLayer)
{

}

IE_ConstMLayerListConstPtr IE_ModelLayerCnt::getConstMLayerConstPtr() const
{

}

IE_ConstMLayerList IE_ModelLayerCnt::getModelLayerList() const
{

}
