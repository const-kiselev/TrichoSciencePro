#ifndef IE_FIELDOFVIEW_CONTROLLER_H
#define IE_FIELDOFVIEW_CONTROLLER_H

#include <QObject>
#include <ie_fieldOfView.h>

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




    explicit IE_FieldOfView_Controller(QObject *parent = nullptr);

    int                     read(const QJsonObject &json);
    int                     write(QJsonObject &json)const;
    void init(Quantity q = Quantity::One);
    void changeQuantity(Quantity){}
    uint getQuantity() const;
    void initDockWidget();
    QDockWidget * getDockWidget()const;
    void setLayerListIterator(){}
    void setMainImageInFV(int fvCode){}
    //!     Запускает соотвествующий процесс релокации/перемещения полей зрения на основе их количества и в случае, если одно изображение заходит за другое.
    void relocateAllFieldOfView(){}
    //! Порядковый индекс MainImage из списка слоев типа MainImage соотвествует порядковому индексу поля зрения.
    static Quantity getStandartQuantity(IE_ProfileType profileType);
signals:
    /// \todo связать с моделью
    void addLayer();
    /// \todo связать с моделью
    void removeLayer();
    /// \todo связать с моделью
    /// \warning учесть вызов при различных изменениях связанных с FV
    void boundingRectWasChanged(QRectF);

    QDialog::DialogCode makeDialogForSetupAsNew();

public slots:
    /// \todo связать с моделью
    QRectF getBoundingRectOfAllFieldOfView() {}
private:
    uint m_quantityOfFields;
    QList<IE_FieldOfView*> m_fieldOfViewList;
};

#endif // IE_FIELDOFVIEW_CONTROLLER_H
