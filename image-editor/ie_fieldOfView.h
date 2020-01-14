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
                            QList<IE_ModelLayer*>*ll,
                            _global_ie * pieg,
                            QObject *parent = nullptr
                            );
            ~IE_FieldOfView();
    int                     read(const QJsonObject &json);
    int                     write(QJsonObject &json)const;
    //! функция удаления всех слоев, которые находятся в поле зрения
    QList<IE_ModelLayer*> getLayers();
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
    void layerAction(IE_ModelLayer::Action action, QList<IE_ModelLayer*>::iterator iter);
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
    QList<IE_ModelLayer*>   *layersList;
    _global_ie *    m_p_ie_global_data;


    IE_ModelLayer * findMainImageLayer();
    QList<IE_ModelLayer*>::iterator findMainImageLayerIter();
    inline IE_Tool_Image * convertToImageTool(IE_ModelLayer*pLayer);
    void moveMainImage(QPointF pos);
    void resetData();
};

#endif // IE_FIELDOFVIEW_H
