#ifndef IE_MODELLAYER_CONTROLLER_H
#define IE_MODELLAYER_CONTROLLER_H

#include <QObject>
#include "ie_modelLayer.h"
#include <QGraphicsScene>

class IE_Model;

//!     \brief Класс-контроллер списка слоев.
//!
//! Необходим для работы со списком слоев, обеспечивает доступ
//! к актуальному списку слоев и возможность отправлять запросы на действия со слоями.
//! \todo реализация виджета для работы со списком слоев, через который происходит доступ к самому слою, его редактированию
//! \todo добавить опциональный параметр при добавлении нового слоя, который сообщает, после какого слоя должен располагаться новый слой

class IE_ModelLayer_Controller : public QObject
{
    Q_OBJECT
public:
    explicit                IE_ModelLayer_Controller(QObject *parent = nullptr);
    bool                    isExist(IE_ModelLayer::PublicType pLayer);
    IE_ModelLayer::PublicListIter
                            getPublicIter(IE_ModelLayer::PublicType pLayer);

public slots:
    void                    addLayer(IE_ModelLayer* pLayer);
    void                    addLayerViaToolCnt();

    void                    removeLayer(IE_ModelLayer::PublicType pLayer);
    void                    hideLayer(IE_ModelLayer::PublicType pLayer);
    void                    showLayer(IE_ModelLayer::PublicType pLayer);


    void                    layerAction(IE_ModelLayer::Action action,
                                        IE_ModelLayer::PublicType pLayer
                                        );
    IE_ModelLayer::PublicConstPtrToList
                            getPublicConstPtrToList() const;
    IE_ModelLayer::PublicList      getModelLayerList() const;

signals:
    void                    layerListWasChanged();
private:
    friend class IE_Model;
    QGraphicsScene * m_pGScene;
//    QList<IE_ModelLayer*> m_layerList;
    QList<const IE_ModelLayer*> m_constLayerList;
};

#endif // IE_MODELLAYER_CONTROLLER_H
