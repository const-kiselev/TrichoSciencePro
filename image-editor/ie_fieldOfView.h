#ifndef IE_FIELDOFVIEW_H
#define IE_FIELDOFVIEW_H

#include <QObject>
#include <QRect>
#include "ie_toolController.h"

class IE_FieldOfView : public QObject
{
    Q_OBJECT
public:
    explicit IE_FieldOfView(uint index,
                            IE_ConstMLayerListConstPtr modelLayerList,
                            _global_ie * pieg,
                            QObject *parent = nullptr
                            );
            ~IE_FieldOfView();
    int                     read(const QJsonObject &json);
    int                     write(QJsonObject &json)const;
    //! функция удаления всех слоев, которые находятся в поле зрения
    IE_ConstMLayerList getLayers();
    QStringList getLayerTitleList();

    void getSquare() {}
    QString getNote() {return QString("");}
    void move() {}
    QFileInfo getMainImageFileInfo();
    int setMainImage(QString filePath="");
    QRectF getRect();
    void setPos(QPointF pos);

signals:
    void addNewLayer(IE_ModelLayer* pLayer);
    void boundingRectWasCganged();
    void layerAction(IE_ModelLayer::Action action, IE_ModelLayer_PublicType layer);
public slots:
    void showIntersectedLayersWithFv();
    void hideIntersectedLayersWithFv();
    void removeIntersectedLayersWithFv();
    void removeLayersAndMainImage();
    void hideLayer(IE_ModelLayer* pLayer);
    void unhideLayer(IE_ModelLayer* pLayer);
    void deleteLayer(IE_ModelLayer* pLayer);

private:
    QRectF rectData;
    uint num;
//    QList<IE_ModelLayer*>   *layersList;
    IE_ConstMLayerListConstPtr m_modelLayerList;
    _global_ie *    m_p_ie_global_data;
    QString m_note;


    IE_ModelLayer_PublicType findMainImageLayer();
    IE_ConstMLayerList::const_iterator findMainImageLayerIter();
    inline IE_Tool_Image * convertToImageTool(IE_ModelLayer_PublicType pLayer);
    void moveMainImage(QPointF pos);
    void resetData();
};

#endif // IE_FIELDOFVIEW_H
