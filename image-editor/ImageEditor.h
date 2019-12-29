#ifndef TSPIMAGEEDITOR_H
#define TSPIMAGEEDITOR_H

#include <QObject>
#include <QtWidgets>
#include "ie_view.h"
#include "ie_calibration.h"


//! \brief класс редактора изображения
//!
//! Политика работы с объектом класса: создается на сессию работы с моделью изображения.
//! Инициализация объекта обязательна, ей необходимо передать параметр типа редактора.
//!

class ImageEditor: public QMainWindow
{
    Q_OBJECT
public:
    explicit ImageEditor();
    ~ImageEditor() override;

signals:
    void wasClosed();
    void wasSaved(TSP_PatientData patientData);

public slots:
    /**
    *   Открытие и чтение сохраненной модели.
    *   @see _Model_patientData
    *   в параметре @param patientData необходимы данные о пациенте: patient_fullName
    */
    int open(TSP_PatientData patientData);
    int makeNew(TSP_PatientData patientData, IE_ProfileType ie_type);
    //! \todo   реализовать
    //!         Выполняет смену активного виджета
    void changeTab(int viewIndex);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    IE_ProfileType m_ieType;
    QToolBar *m_pTopToolBar;
    QVector<IE_View*> m_ieViewVec;
    QStackedWidget  m_stackedWidget;
    int m_currentTab;
    QWidget * pCurrentFVFastManagment;

    int     init(IE_ProfileType ie_type);
    int     initModelsAsNew(TSP_PatientData patientData, IE_ProfileType ie_type);
    int     initModels(TSP_PatientData patientData);
    void    menuInit();
    void    clearIEViewVec();

private slots:
    void makeCalibration();
};

#endif // TSPIMAGEEDITOR_H
