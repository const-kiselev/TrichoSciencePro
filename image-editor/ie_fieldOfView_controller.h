#ifndef IE_FIELDOFVIEW_CONTROLLER_H
#define IE_FIELDOFVIEW_CONTROLLER_H

#include <QObject>

#include "ie_fieldOfView.h"
#include "ie_imageBase.h"


class IE_FieldOfView_ControllerInfoWidget;

/*!
  \brief Контроллер полей зрений.

  Сокращение FVC.
  FV — field of view.
  Политика класса: так как это контроллер, то логично, что он должен создаваться вместе с моделью класса и связывать сигналы и слоты сразу же.

  Количество FV задается при инициализации (как следствие, объекты FV и список с ними также инициализируется), а дальше меняется только через функцию changeQuantity().
*/

class IE_FieldOfView_Controller : public QObject
{
    Q_OBJECT
public:
    enum Quantity
    {
        One = 1,
        Two = 2,
        Three = 3,
        Four = 4,
        Six = 6
    } quantityMax = Quantity::Six;

    explicit IE_FieldOfView_Controller(IE_ConstMLayerListConstPtr modelLayerList,
                                       _global_ie * pieg,
                                       QObject *parent = nullptr
                                       );
    ~IE_FieldOfView_Controller();

    int                     read(const QJsonObject &json);
    int                     write(QJsonObject &json)const;


    void init(Quantity q = Quantity::One);
    int initImageBase();
    QDockWidget *getImageBaseDockWidget();

    QDialog::DialogCode makeInitDialog();
    QDialog::DialogCode makeDialogForSetupAsNew();

    /// \todo реализовать!!!
    void changeQuantity(Quantity q);
    uint getQuantity() const;

    void initDockWidget();
    QWidget * getInfoWidget()const;

    void setLayerListIterator(){}
    void setMainImageInFV(int fvCode){}
    //!     Запускает соотвествующий процесс релокации/перемещения полей зрения на основе их количества и в случае, если одно изображение заходит за другое.
    void relocateAllFieldOfView();
    //! Порядковый индекс MainImage из списка слоев типа MainImage соотвествует порядковому индексу поля зрения.
    static Quantity getStandartQuantity(IE_ProfileType profileType);
    int getActiveFVIndex() const;
    QWidget *getFastManagerWidget() const;

signals:
    /// \todo связать с моделью
    void addNewLayer(IE_ModelLayer* pLayer);
    /// \todo связать с моделью
    void removeLayer(QList<IE_ModelLayer*>::iterator iter);
    /// \todo связать с моделью
    /// \warning учесть вызов при различных изменениях связанных с FV
    void boundingRectWasChanged(QRectF);
    void hideLayer(QList<IE_ModelLayer*>::iterator iter);
    void layerAction(IE_ModelLayer::Action action, QList<IE_ModelLayer*>::iterator iter);
    void activeFVLayerListWasUpdated(QList<IE_ModelLayer*>);




public slots:
    /// \todo связать с моделью
    QRectF getBoundingRectOfAllFieldOfView();
    void changeActiveFieldOfView(int index);
    QList<IE_ModelLayer*> getActiveFieldOfViewLayerList();
    void checkLayerList();

private:
    uint m_quantityOfFields;
    bool m_inited;
    int m_activeFVIndex;
    QList<IE_FieldOfView*> m_fieldOfViewList;
//    QList<IE_ModelLayer*>   *layersList;
    IE_ConstMLayerListConstPtr m_modelLayerList;
    _global_ie *    m_p_ie_global_data;
    IE_FieldOfView_ControllerInfoWidget * m_pInfoWidget;
    IE_ImageBaseCnt         * m_pImageBaseCnt;


    void addFieldOfView(int index);
    void removeFieldOfView(int index);


};

//-------------------------     FVC WIDGET     -------------------------

class IE_FieldOfView_ControllerInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IE_FieldOfView_ControllerInfoWidget(QWidget *parent=nullptr);
    ~IE_FieldOfView_ControllerInfoWidget();
    void init(int currentFVquantity);

signals:
    void quantityWasChanged(int q);
    void activeFVWasChanged(int index);
    void changeActiveFVMainImage();
    void removeAllElementsOnActiveFV();
    void hideAllElementsOnActiveFV();
    void showAllElementsOnActiveFV();
    void activeFVNoteWasChanged(QString noteText);
    void getActiveFVNote();
    void deleteLayer(IE_ModelLayer* pModelLayer);
    void hideLayer(IE_ModelLayer* pModelLayer);
    void unhideLayer(IE_ModelLayer* pModelLayer);

    void makeInit();
public slots:
    void changeQuantity(int q);
    void changeActiveFV(int index, QString fvNote);
    void updateActiveFVLayerList(QList<IE_ModelLayer*> layerList);

    friend class IE_FieldOfView_Controller;
private:
    int oldQuantity;

    QComboBox    *pcboQuiantityFV, *pcboActiveFV;
    QPushButton * pInitBut;
    QString m_activeFVNote;
    QListWidget * m_pDockLayersListWidget;
    QWidget * m_pFastManagerWidget;

    void makeChangeActiveFVNoteDialog();



};



#endif // IE_FIELDOFVIEW_CONTROLLER_H
