#ifndef IE_IMAGEBASECNT_H
#define IE_IMAGEBASECNT_H

#include <QDir>
#include <QDockWidget>
#include <QObject>
#include "ie_imageBase_treeModel.h"
#include "ie_imageBase_widget.h"


//! \brief Класс управления базой изображений для отельных разделов ImageEditor
//!
//! Все изображения уменьшаются до определенного размера и их идентификатором является тег соответсвующий родительскому тегу и собственному НА ЛАТИНИЦЕ
//! Взаимосвязи определяются в файле json. файл имеет иерархическую систему
//! Политика использования: выделяется в памяти под каждый раздел.
class IE_ImageBaseCnt : public QObject
{
    Q_OBJECT
public:
    enum Type
    {

    };

    static const char   * STD_DIR,
                        * FILE_NAME_RULE,
                        * FILE_NAME,
                        * FILE_EXTENSION,
                        * TSP_docTypeVALUE;
    const QString FULL_FILE_NAME;

    explicit IE_ImageBaseCnt(QDir workDir = QDir( STD_DIR ), QObject *parent = nullptr);
    //! Отобразить изображение в отдельном окне с информацией
    //! Редактор элемента базы
    //! виджет всех изображений в виде дерева
    QDockWidget * getDockWidgetWithAllImages();
    QStringList * getSelectedImagesTypeNameList() const;
    //! виджет выбранных изображений
    //! виджет управления?!
    //! доступные действия
    //! сопоставление слоев-инструментов и выбранных изображений (параметры: список слоев) = результат:
    //!     список пересечения этих двух множеств; если изображения с подобным связанным типом не
    //!     оказалось в базе, то приоритет за инструментом. Аналогично, если у изображения нет
    //!     связанного инструмента, то изображение остается.
    int makeCorellation(QStringList layerTitlesList);
    //! синхронизация со списоком собственных инструментов
    //! проверка изображений и соотсветвующее преобразование больших изображений в размер 512х512 с сохранением соотношения
    static bool containsImageBaseUserChoice(const QJsonObject &json);

signals:
    void needLayerTitlesList();
public slots:
    int readUserChoice(const QJsonObject &json, int index = -1);
    int setUserChoiceListSize(int size);
    int writeUserChoice(QJsonObject &json, int index) const;
    int setCurrentUserChoiceList(int num);
private:
    QDir m_workDir;
    IE_IB_treeModel * m_pTreeModel;
    IE_IB_widget *  m_pWidget;
    QDockWidget * m_allImagesDockWidget;

    int read(const QJsonObject &json);
    int write(QJsonObject &json)const;
    int open();
    int save();


};

#endif // IE_IMAGEBASECNT_H
