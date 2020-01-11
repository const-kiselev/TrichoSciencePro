#ifndef IE_FIELDOFVIEW_H
#define IE_FIELDOFVIEW_H

#include <QObject>
#include <QRect>
#include <QSharedPointer>
#include "ie_toolController.h"

/*!
    \brief Класс для работы с полем зрения.
    FV = Field of View
*/

class IE_FieldOfView : public QObject
{
    Q_OBJECT
public:
    explicit    IE_FieldOfView(uint index,
                            IE_MLayerListConstPtr pModelLayerList,
                            _global_ie * pieg,
                            QObject *parent = nullptr
                            );
                ~IE_FieldOfView();
    int                     read(const QJsonObject &json);
    int                     write(QJsonObject &json)const;
    //! функция удаления всех слоев, которые находятся в поле зрения
    //! \todo удалить!!!

    IE_MLayerListConstPtr
                getConstPtrOfLayerList() const;

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
    void        layerAction(IE_ModelLayer::Action action, const IE_ModelLayer* pLayer);
public slots:
    void        showIntersectedLayersWithFv();
    void        hideIntersectedLayersWithFv();
    void        removeIntersectedLayersWithFv();
    void        removeLayersAndMainImage();
    void        hideLayer(IE_ModelLayer* pLayer);
    void        unhideLayer(IE_ModelLayer* pLayer);
    void        deleteLayer(IE_ModelLayer* pLayer);

private:
    IE_MLayerList           m_layerList;
    QRectF                  rectData;
    uint                    num; //! ид. номер поля зрения
    IE_MLayerListConstPtr   m_pModelLayerList;
//    QList<IE_ModelLayer*> *layersList;
    _global_ie *            m_p_ie_global_data;


    const IE_ModelLayer *
                getMainImageLayerPtr();
    inline IE_Tool_Image *
                convertToImageTool(const IE_ModelLayer *pLayer);
    void        moveMainImage(QPointF pos);
    void        resetData();
    void        updateLayerList();
};

typedef const QList<IE_FieldOfView*>* IE_FVConstList;


#endif // IE_FIELDOFVIEW_H
