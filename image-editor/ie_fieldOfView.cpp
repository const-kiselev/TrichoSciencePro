#include "ie_fieldOfView.h"

IE_FieldOfView::IE_FieldOfView(uint num,
                               QList<IE_ModelLayer*>*ll,
                               _global_ie * pieg,
                               QObject *parent
                               ) :  QObject(parent),
                                    num(num),
                                    layersList(ll),
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

QList<IE_ModelLayer *> IE_FieldOfView::getLayers()
{
    QList<IE_ModelLayer *> list;
    for (QList<IE_ModelLayer *>::iterator tmpIter = layersList->begin();
         tmpIter!=layersList->end();tmpIter++)
    {
        if(tmpIter.i->t()->getToolType() == ToolType::MainImage)
            continue;
        QRectF layerRect = tmpIter.i->t()->boundingRect();
        if(tmpIter.i->t()->getPos().x()>layerRect.topLeft().x()
                && tmpIter.i->t()->getPos().y()>layerRect.topLeft().y())
            layerRect.moveTopLeft(tmpIter.i->t()->getPos());
        QRectF intersect = rectData.intersected(layerRect);
        if(intersect.isEmpty())
            continue;
        qreal   ih = intersect.height(),
                iw = intersect.width();
        qreal   sihp = ih / ( layerRect.height() / 100 ),
                siwp = iw / ( layerRect.width() / 100 );
         if(sihp >= 80 && siwp >= 80)
             list.append(tmpIter.i->t());
    }
    return list;
}

QStringList IE_FieldOfView::getLayerTitleList()
{
    QStringList strList;
    for (QList<IE_ModelLayer *>::iterator tmpIter = layersList->begin();
         tmpIter!=layersList->end();tmpIter++)
    {
        if(tmpIter.i->t()->getToolType() == ToolType::MainImage)
            continue;
        strList << getToolTitle(tmpIter.i->t()->getToolType());
    }
    return strList;
}

void IE_FieldOfView::removeIntersectedLayersWithFv()
{
    QList<IE_ModelLayer *> layers = getLayers();
    for (QList<IE_ModelLayer *>::iterator tmpIter = layers.begin();
         tmpIter!=layers.end();tmpIter++)
             emit layerAction(IE_ModelLayer::Action::Remove, tmpIter);
}

void IE_FieldOfView::removeLayersAndMainImage()
{
    {
        QList<IE_ModelLayer*>::iterator pMainImageLayerIter
                = findMainImageLayerIter();
        if(pMainImageLayerIter!=layersList->end())
        {
            layerAction(IE_ModelLayer::Action::Remove, pMainImageLayerIter);
            pMainImageLayerIter = layersList->end();
        }
    }
//    IE_Tool_Image * pToolImage = convertToImageTool(findMainImageLayer());
//    if(pToolImage)
//        pToolImage->deleteFile();
    removeIntersectedLayersWithFv();
    resetData();
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
    QList<IE_ModelLayer *> layers = getLayers();
    for (QList<IE_ModelLayer *>::iterator tmpIter = layers.begin();
         tmpIter!=layers.end();tmpIter++)
        emit layerAction(IE_ModelLayer::Action::Show, tmpIter);
}

void IE_FieldOfView::hideIntersectedLayersWithFv()
{
    QList<IE_ModelLayer *> layers = getLayers();
    for (QList<IE_ModelLayer *>::iterator tmpIter = layers.begin();
         tmpIter!=layers.end();tmpIter++)
        emit layerAction(IE_ModelLayer::Action::Hide, tmpIter);
}

IE_ModelLayer *IE_FieldOfView::findMainImageLayer()
{
    QList<IE_ModelLayer*>::iterator it = findMainImageLayerIter();
    if(it!=layersList->end())
        return it.i->t();
    return nullptr;
}

QList<IE_ModelLayer*>::iterator IE_FieldOfView::findMainImageLayerIter()
{
    uint i=0;
    for (QList<IE_ModelLayer *>::iterator tmpIter = layersList->begin();
         tmpIter!=layersList->end();tmpIter++)
    {
        //! \warning для других типов слоев не будет работать.
        if( tmpIter.i->t()->getToolType() == ToolType::MainImage)
        {
            i++;
            if(i == num)
                return tmpIter;
        }
    }
    resetData();
    return layersList->end();
}

IE_Tool_Image *IE_FieldOfView::convertToImageTool(IE_ModelLayer *pLayer)
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
    IE_ModelLayer * pMainImageLayer = findMainImageLayer();
    if(!pMainImageLayer)
    {
        resetData();
        return;
    }
    convertToImageTool(pMainImageLayer)->setPos(pos);
    QList<IE_ModelLayer *> layers = getLayers();
    for (QList<IE_ModelLayer *>::iterator tmpIter = layers.begin();
         tmpIter!=layers.end();tmpIter++)
        tmpIter.i->t()->moveBy(rectData.x()-pos.x(), rectData.y()-pos.y());
    //pMainImageLayer->setPos(pos);

}

void IE_FieldOfView::resetData()
{
//    rectData.setX(0);
//    rectData.setY(0);
    rectData.setWidth(0);
    rectData.setHeight(0);
}


