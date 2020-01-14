#include "ie_modelLayer_controller.h"

IE_ModelLayer_Controller::IE_ModelLayer_Controller(QObject *parent) : QObject(parent)
{
//    connect(&m_constLayerList, &QList::)
    m_pGScene = nullptr;
}

bool IE_ModelLayer_Controller::isExist(IE_ModelLayer_PublicType pLayer)
{
    if(getPublicIter(pLayer) == m_constLayerList.end())
        return false;
    else
        return true;
}

IE_MLIter IE_ModelLayer_Controller::getPublicIter(IE_ModelLayer_PublicType pLayer)
{
    for(IE_MLIter iter = m_constLayerList.begin();
        iter!=m_constLayerList.end();
        iter++
        )
        if(iter.i->t() == pLayer)
            return iter;
    return m_constLayerList.end();
}

void IE_ModelLayer_Controller::addLayer(IE_ModelLayer *pLayer)
{
    if(isExist(pLayer))
        return;
    if(m_pGScene)
        m_pGScene->addItem(pLayer->parentItem());
    m_constLayerList.append(pLayer);
    emit layerListWasChanged();
    qDebug() << "void IE_ModelLayer_Controller::addLayer(IE_ModelLayer *pLayer)";
}

void IE_ModelLayer_Controller::addLayerViaToolCnt()
{

}

void IE_ModelLayer_Controller::removeLayer(IE_ModelLayer_PublicType pLayer)
{
    if(!isExist(pLayer))
        return;
    if(m_pGScene)
        m_pGScene->removeItem(pLayer->parentItem());
    m_constLayerList.removeAll(pLayer);
    emit layerListWasChanged();
}

void IE_ModelLayer_Controller::hideLayer(IE_ModelLayer_PublicType pLayer)
{
    if(!isExist(pLayer))
        return;
    IE_ModelLayer * loc_pLayer = const_cast<IE_ModelLayer*>(pLayer);
    loc_pLayer->makeAction(IE_ModelLayer::Action::Hide);
}

void IE_ModelLayer_Controller::showLayer(IE_ModelLayer_PublicType pLayer)
{
    if(!isExist(pLayer))
        return;
    IE_ModelLayer * loc_pLayer = const_cast<IE_ModelLayer*>(pLayer);
    loc_pLayer->makeAction(IE_ModelLayer::Action::Show);
}

void IE_ModelLayer_Controller::layerAction(IE_ModelLayer::Action action, IE_ModelLayer_PublicType pLayer)
{
    if(!isExist(pLayer))
        return;
    switch (action)
    {
        case IE_ModelLayer::Action::Hide:
            hideLayer(pLayer);
        break;
        case IE_ModelLayer::Action::Show:
            showLayer(pLayer);
        break;
        case IE_ModelLayer::Action::Remove:
            removeLayer(pLayer);
        break;
    }
}

IE_ConstMLayerListConstPtr IE_ModelLayer_Controller::getConstMLayerConstPtr() const
{
    return &m_constLayerList;
}

IE_ConstMLayerList IE_ModelLayer_Controller::getModelLayerList() const
{
    return m_constLayerList;
}
