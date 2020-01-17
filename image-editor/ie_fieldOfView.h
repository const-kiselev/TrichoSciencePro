#ifndef IE_FIELDOFVIEW_H
#define IE_FIELDOFVIEW_H

#include <QObject>
#include <QRect>
#include "ie_toolController.h"

class IE_FieldOfView : public QObject
{
    Q_OBJECT
public:
    typedef QList<const IE_FieldOfView*> PublicList;


    explicit IE_FieldOfView(uint index,
                            IE_ModelLayer::PublicConstPtrToList modelLayerList,
                            _global_ie * pieg,
                            QObject *parent = nullptr
                            );
                ~IE_FieldOfView();
    int                     read(const QJsonObject &json);
    int                     write(QJsonObject &json)const;
    IE_ModelLayer::PublicList
                getLayers() const;
    QStringList getLayerTitleList();

    qreal       getAreaValue() const;
    QString     getNote() const {return QString("");}
    void        move() {}
    QFileInfo   getMainImageFileInfo();
    int         setMainImage(QString filePath="");
    QRectF      getRect() const;
    void        setPos(QPointF pos);

signals:
    void        addNewLayer(IE_ModelLayer* pLayer);
    void        boundingRectWasCganged();
    void        layerAction(IE_ModelLayer::Action action, IE_ModelLayer::PublicType layer);
public slots:
    void        showIntersectedLayersWithFv();
    void        hideIntersectedLayersWithFv();
    //! функция удаления всех слоев, которые находятся в поле зрения
    void        removeIntersectedLayersWithFv();
    void        removeLayersAndMainImage();
    void        hideLayer(IE_ModelLayer* pLayer);
    void        unhideLayer(IE_ModelLayer* pLayer);
    void        deleteLayer(IE_ModelLayer* pLayer);

private:
    IE_ModelLayer::PublicConstPtrToList
                m_modelLayerList;
    QRectF      rectData;
    uint        m_num;
    _global_ie* m_p_ie_global_data;
    QString     m_note;


    IE_ModelLayer::PublicType
                findMainImageLayer();
    IE_ModelLayer::PublicList::const_iterator
                findMainImageLayerIter() const;
    inline IE_Tool_Image *
                convertToImageTool(IE_ModelLayer::PublicType pLayer);
    void        moveMainImage(QPointF pos);
    void        resetData();
};

#endif // IE_FIELDOFVIEW_H
