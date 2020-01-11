#include "ie_fieldOfView.h"

IE_FieldOfView::IE_FieldOfView(uint num,
                               IE_MLayerListConstPtr pModelLayerList,
                               _global_ie * pieg,
                               QObject *parent
                               ) :  QObject(parent),
                                    num(num),
                                    m_pModelLayerList(pModelLayerList),
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

void IE_FieldOfView::updateLayerList()
{
    m_layerList.clear();
    foreach(const IE_ModelLayer* pLayer, m_pModelLayerList.get())
    {
        if(pLayer->getToolType() == ToolType::MainImage)
            continue;
        QRectF layerRect = pLayer->boundingRect();
        if(pLayer->getPos().x()>layerRect.topLeft().x()
                && pLayer->getPos().y()>layerRect.topLeft().y())
            layerRect.moveTopLeft(pLayer->getPos());
        QRectF intersect = rectData.intersected(layerRect);
        if(intersect.isEmpty())
            continue;
        qreal   ih = intersect.height(),
                iw = intersect.width();
        qreal   sihp = ih / ( layerRect.height() / 100 ),
                siwp = iw / ( layerRect.width() / 100 );
         if(sihp >= 80 && siwp >= 80)
             m_layerList.append(pLayer);
    }
}

IE_MLayerListConstPtr IE_FieldOfView::getConstPtrOfLayerList() const
{
    return &m_layerList;
}

QStringList IE_FieldOfView::getLayerTitleList()
{
    QStringList strList;
    updateLayerList();
    foreach(const IE_ModelLayer* pLayer, m_layerList)
    {
        if(pLayer->getToolType() == ToolType::MainImage)
            continue;
        strList << getToolTitle(pLayer->getToolType());
    }
    return strList;
}

qreal IE_FieldOfView::getAreaValue() const
{
    // вычисление площади поля зрения
    // если есть инструмент-область, то площадь берется по этому интсрументу
    // в противном случае — по главному изображению
    //! \todo   реализовать вычисление площади для интсрумента-области
    IE_ModelLayer* pMainImg = findMainImageLayer();
    if(!pMainImg)
        return 0;
    return pMainImg->boundingRect().width() *
            pMainImg->boundingRect().height();

}

void IE_FieldOfView::removeIntersectedLayersWithFv()
{
    updateLayerList();
    foreach(const IE_ModelLayer* pLayer, m_layerList)
    {
        emit layerAction(IE_ModelLayer::Action::Remove, pLayer);
    }
}

void IE_FieldOfView::removeLayersAndMainImage()
{

    const IE_ModelLayer* pMainImageLayerPtr
            = getMainImageLayerPtr();
    // проверка на существоание оснвного изобаржения
    if(pMainImageLayerPtr != nullptr)
    {
        layerAction(IE_ModelLayer::Action::Remove, pMainImageLayerPtr);
        pMainImageLayerPtr = nullptr;
    }

    //! \bug реализовать удаление изображения правда, это должен делать класс, который совершает удаление непосредственно
            //    IE_Tool_Image * pToolImage = convertToImageTool(findMainImageLayer());
            //    if(pToolImage)
            //        pToolImage->deleteFile();
    removeIntersectedLayersWithFv();
    resetData();
}

void IE_FieldOfView::hideLayer(IE_ModelLayer *pLayerToHide)
{
    updateLayerList();
    foreach(const IE_ModelLayer* pLayer, m_layerList)
        if(pLayer == pLayerToHide)
        {
            emit layerAction(IE_ModelLayer::Action::Hide, pLayer);
            return;
        }
}

void IE_FieldOfView::unhideLayer(IE_ModelLayer *pLayerToUnhide)
{
    updateLayerList();
    foreach(const IE_ModelLayer* pLayer, m_layerList)
        if(pLayer == pLayerToUnhide)
        {
            emit layerAction(IE_ModelLayer::Action::Show, pLayer);
            return;
        }
}

void IE_FieldOfView::deleteLayer(IE_ModelLayer *pLayerToDelete)
{
    updateLayerList();
    foreach(const IE_ModelLayer* pLayer, m_layerList)
        if(pLayer == pLayerToDelete)
        {
            emit layerAction(IE_ModelLayer::Action::Remove, pLayer);
            return;
        }
}

QFileInfo IE_FieldOfView::getMainImageFileInfo()
{
    IE_Tool_Image * pToolImage = convertToImageTool(getMainImageLayerPtr());
    if(!pToolImage)
        return QFileInfo();
    return pToolImage->getFileInfo();
}

int IE_FieldOfView::setMainImage(QString filePath)
{
    IE_Tool_Image * pToolImage = convertToImageTool(getMainImageLayerPtr());
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

QRectF IE_FieldOfView::getRect() const
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
    updateLayerList();
    foreach(const IE_ModelLayer* pLayer, m_layerList)
        emit layerAction(IE_ModelLayer::Action::Show, pLayer);
}

void IE_FieldOfView::hideIntersectedLayersWithFv()
{
    updateLayerList();
    foreach(const IE_ModelLayer* pLayer, m_layerList)
        emit layerAction(IE_ModelLayer::Action::Hide, pLayer);
}

const IE_ModelLayer * IE_FieldOfView::getMainImageLayerPtr()
{
    uint i=0;
    foreach(const IE_ModelLayer* pLayer, m_pModelLayerList)
    {
        //! \warning для других типов слоев не будет работать.
        if( pLayer->getToolType() == ToolType::MainImage)
        {
            i++;
            if(i == num)
                return pLayer;
        }
    }
    resetData();
    return nullptr;
}

IE_Tool_Image *IE_FieldOfView::convertToImageTool(const IE_ModelLayer *pLayer)
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
     updateLayerList();
    const IE_ModelLayer * pMainImageLayer = getMainImageLayerPtr();
    if(!pMainImageLayer)
    {
        resetData();
        return;
    }
    convertToImageTool(pMainImageLayer)->setPos(pos);

    foreach(const IE_ModelLayer* pLayer, m_layerList)
    {
//        tmpIter.i->t()->moveBy(rectData.x()-pos.x(),
//                               rectData.y()-pos.y()
//                               );
        //! все операции должны проходить через передачу типа действия в функцию манипцляций со слоем IE_ModelLayer
        pLayer->parentItem()->setPos(abs(pLayer->parentItem()->pos().x() - rectData.x())   + pos.x(),
                                        abs(pLayer->parentItem()->pos().y() - rectData.y()) + pos.y()
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


