#include "ie_fieldOfView.h"

IE_FieldOfView::IE_FieldOfView(uint num,
                               IE_ConstMLayerListConstPtr modelLayerList,
                               _global_ie * pieg,
                               QObject *parent
                               ) :  QObject(parent),
                                    num(num),
                                    m_modelLayerList(modelLayerList),
                                    m_p_ie_global_data(pieg)
{

}

IE_FieldOfView::~IE_FieldOfView()
{
    removeIntersectedLayersWithFv();
}

int IE_FieldOfView::read(const QJsonObject &json)
{
    rectData.setX(json["posX"].toDouble());
    rectData.setY(json["posY"].toDouble());
    rectData.setWidth(json["width"].toDouble());
    rectData.setHeight(json["height"].toDouble());
    emit boundingRectWasCganged();
    return 0;
}

int IE_FieldOfView::write(QJsonObject &json) const
{
    json["posX"] = rectData.x();
    json["posY"] = rectData.y();
    json["width"] = rectData.width();
    json["height"] = rectData.height();
    return 0;
}

IE_ConstMLayerList IE_FieldOfView::getLayers()
{
    IE_ConstMLayerList list;
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
    {
        if(layerIter.i->t()->getToolType() == ToolType::MainImage)
            continue;
        QRectF layerRect = layerIter.i->t()->boundingRect();
        if(layerIter.i->t()->getPos().x()>layerRect.topLeft().x()
                && layerIter.i->t()->getPos().y()>layerRect.topLeft().y())
            layerRect.moveTopLeft(layerIter.i->t()->getPos());
        QRectF intersect = rectData.intersected(layerRect);
        if(intersect.isEmpty())
            continue;
        qreal   ih = intersect.height(),
                iw = intersect.width();
        qreal   sihp = ih / ( layerRect.height() / 100 ),
                siwp = iw / ( layerRect.width() / 100 );
         if(sihp >= 80 && siwp >= 80)
             list.append(layerIter.i->t());
    }
    return list;
}

QStringList IE_FieldOfView::getLayerTitleList()
{
    QStringList strList;
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
    {
        if(layerIter.i->t()->getToolType() == ToolType::MainImage)
            continue;
        strList << getToolTitle(layerIter.i->t()->getToolType());
    }
    return strList;
}

void IE_FieldOfView::removeIntersectedLayersWithFv()
{
    IE_ConstMLayerList layers = getLayers();
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
             emit layerAction(IE_ModelLayer::Action::Remove, layerIter.i->t());
}

void IE_FieldOfView::removeLayersAndMainImage()
{
    {
        IE_ConstMLayerList::const_iterator pMainImageLayerIter
                = findMainImageLayerIter();
        if(pMainImageLayerIter!=m_modelLayerList->end())
        {
            layerAction(IE_ModelLayer::Action::Remove, pMainImageLayerIter.i->t());
            pMainImageLayerIter = m_modelLayerList->end();
        }
    }
//    IE_Tool_Image * pToolImage = convertToImageTool(findMainImageLayer());
//    if(pToolImage)
//        pToolImage->deleteFile();
    removeIntersectedLayersWithFv();
    resetData();
}

void IE_FieldOfView::hideLayer(IE_ModelLayer *pLayer)
{
    IE_ConstMLayerList layers = getLayers();
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
        if(layerIter.i->t() == pLayer)
        {
            emit layerAction(IE_ModelLayer::Action::Hide, layerIter.i->t());
            return;
        }
}

void IE_FieldOfView::unhideLayer(IE_ModelLayer *pLayer)
{
    IE_ConstMLayerList layers = getLayers();
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
        if(layerIter.i->t() == pLayer)
        {
            emit layerAction(IE_ModelLayer::Action::Show, layerIter.i->t());
            return;
        }
}

void IE_FieldOfView::deleteLayer(IE_ModelLayer *pLayer)
{
    IE_ConstMLayerList layers = getLayers();
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
        if(layerIter.i->t() == pLayer)
        {
            emit layerAction(IE_ModelLayer::Action::Remove, layerIter.i->t());
            return;
        }
}

QFileInfo IE_FieldOfView::getMainImageFileInfo()
{
    IE_Tool_Image * pToolImage = convertToImageTool(findMainImageLayer());
    if(!pToolImage)
        return QFileInfo();
    return pToolImage->getFileInfo();
}

int IE_FieldOfView::setMainImage(QString filePath)
{
    IE_Tool_Image * pToolImage = convertToImageTool(findMainImageLayer());
    if(!pToolImage)
    {
        // Создаем новый слой
        pToolImage = new IE_Tool_Image();
        pToolImage->setDirs(m_p_ie_global_data->getModelResDirPath(),
                            m_p_ie_global_data->getModelDirPath(),
                            m_p_ie_global_data->getTmp_modelResDirPath());
        if(pToolImage->loadImage(filePath, m_p_ie_global_data->getLastSelectedDirByUser()))
        {
            resetData();
            return 1;
        }
        IE_ModelLayer* pLayer = new IE_ModelLayer(ToolType::MainImage,pToolImage);
        emit addNewLayer(pLayer);
    }
    else
    {
        pToolImage->setDirs(m_p_ie_global_data->getModelResDirPath(),
                            m_p_ie_global_data->getModelDirPath(),
                            m_p_ie_global_data->getTmp_modelResDirPath());
        if(pToolImage->loadImage(filePath))
        {
            resetData();
            return 1;
        }
    }

    m_p_ie_global_data->setLastSelectedDirByUser(pToolImage->getOriginalFileInfo().dir().path());
    emit boundingRectWasCganged();
    rectData.setWidth( pToolImage->boundingRect().width() );
    rectData.setHeight( pToolImage->boundingRect().height() );

    return 0;
}

QRectF IE_FieldOfView::getRect()
{
    return rectData;
}

void IE_FieldOfView::setPos(QPointF pos)
{
    moveMainImage(pos);
    rectData.moveTopLeft(pos);
    emit boundingRectWasCganged();
}

void IE_FieldOfView::showIntersectedLayersWithFv()
{
    IE_ConstMLayerList layers = getLayers();
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
        emit layerAction(IE_ModelLayer::Action::Show, layerIter.i->t());
}

void IE_FieldOfView::hideIntersectedLayersWithFv()
{
    IE_ConstMLayerList layers = getLayers();
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
        emit layerAction(IE_ModelLayer::Action::Hide, layerIter.i->t());
}

IE_ModelLayer_PublicType IE_FieldOfView::findMainImageLayer()
{
    IE_ConstMLayerList::const_iterator it = findMainImageLayerIter();
    if(it!=m_modelLayerList->end())
        return it.i->t();
    return nullptr;
}

IE_ConstMLayerList::const_iterator IE_FieldOfView::findMainImageLayerIter()
{
    uint i=0;
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
    {
        //! \warning для других типов слоев не будет работать.
        if( layerIter.i->t()->getToolType() == ToolType::MainImage)
        {
            i++;
            if(i == num)
                return layerIter;
        }
    }
    resetData();
    return m_modelLayerList->end();
}

IE_Tool_Image *IE_FieldOfView::convertToImageTool(IE_ModelLayer_PublicType pLayer)
{
    if(!pLayer)
    {
        resetData();
        return nullptr;
    }
    if(!pLayer->getToolPtr())
        return nullptr;
    return dynamic_cast<IE_Tool_Image*>(pLayer->getToolPtr());
}

void IE_FieldOfView::moveMainImage(QPointF pos)
{
    IE_ModelLayer_PublicType pMainImageLayer = findMainImageLayer();
    if(!pMainImageLayer)
    {
        resetData();
        return;
    }
    convertToImageTool(pMainImageLayer)->setPos(pos);
    IE_ConstMLayerList layers = getLayers();
    for (IE_ConstMLayerList::const_iterator layerIter = m_modelLayerList->begin();
         layerIter != m_modelLayerList->end();
         layerIter++)
    {
//        tmpIter.i->t()->moveBy(rectData.x()-pos.x(),
//                               rectData.y()-pos.y()
//                               );
        layerIter.i->t()->parentItem()->setPos(abs(layerIter.i->t()->parentItem()->pos().x() - rectData.x())   + pos.x(),
                                                abs(layerIter.i->t()->parentItem()->pos().y() - rectData.y()) + pos.y()
                                               );
    }
    //pMainImageLayer->setPos(pos);

}

void IE_FieldOfView::resetData()
{
//    rectData.setX(0);
//    rectData.setY(0);
    rectData.setWidth(0);
    rectData.setHeight(0);
}


