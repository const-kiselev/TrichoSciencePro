#ifndef TSPIMAGEEDITORMODEL_H
#define TSPIMAGEEDITORMODEL_H

#include <QtWidgets>
#include "ie_report.h"
#include "ie_layersTableModel.h"
#include "ie_fieldOfView_controller.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

QT_CHARTS_USE_NAMESPACE




/// \todo переделать в класс
struct ModelData
{
    IE_ProfileType          profile;
    QString patientFullName; //     ФИО

    QDir    modelDir, // директория модели. Новая создается в папке IE_MODEL_RES_DIR_NAME
            resDir, // директория ресурсов модели. В ней хранятся различные изображения, файлы, которые используются.
            tmpDir; // директория временного размещения данных модели,
                    //  в ней хранятся данные до сохранения. Удаляется после закрытия модели.
    uint    patientID,
            patientUID, // присваивается ЕДИНОЖДЫ !!!
            model_ID;
    ModelData()
    {
        patientID = patientUID = model_ID = 0;
        patientFullName = "Empty";
        modelDir = resDir = tmpDir = QDir();
        profile = IE_ProfileType::None;
    }

    void initNew(_Model_patientData patientData)
    {
        model_ID = QDateTime::currentDateTime().toTime_t();
        init(patientData);
        modelDir = QString("%1/%2/%3") .arg(patientData.modelDir)
                                    .arg(IE_MODEL_DIR_NAME)
                                    .arg(model_ID);
        update();
    }

    void init(_Model_patientData patientData)
    {
        patientID = patientData.patient_ID;
        patientUID = patientData.patient_UID;
        patientFullName = patientData.patient_fullName;
        modelDir = patientData.modelDir;
        profile = patientData.ie_type;
        if(QFile::exists(patientData.modelPath))
        {
            modelDir = patientData.modelPath;
            modelDir.cdUp();
        }
    }

    void update()
    {
//        modelDir.setPath(QString("%1/%2").  arg(modelDir.path()).
//                                            arg(model_ID)
//                         );
        tmpDir.setPath(QString("%1/%2/%3"). arg(IE_TMP_DIR_NAME).
                                            arg(IE_MODEL_DIR_NAME).
                                            arg(model_ID)
                       );
        resDir.setPath(QString("%1").arg(IE_MODEL_RES_DIR_NAME));
    }

#ifdef QT_DEBUG
    void activateTestData()
    {
        modelDir = "data/patients/0";
        patientID = 0;
        patientFullName = "ФИО тестовые";
    }
#endif
    QString getPath()
    {

    }

    _Model_patientData to_Model_patientData()
    {
        _Model_patientData answer;
        answer.modelDir = modelDir.path();
        answer.model_ID = model_ID;
        answer.patient_ID = patientID;
        answer.patient_UID = patientUID;
        answer.patient_fullName = patientFullName;
        answer.ie_type = profile;
        return answer;
    }

    int    read(const QJsonObject &json)
    {

        // подразумивается, что patientUID может быть нуливым только тогда, когда мы читаем МОДЕЛЬ напрямую. А так, при поступлении данных о пациенте, эти данные перезаписываются и становятся актуальными (если изменился ID пациента и/или его имя).
        if(patientUID == 0)
        {
            patientID = json["patient_ID"].toString().toUInt();
            patientFullName = json["patient_fullName"].toString();
        }

        model_ID = json["model_ID"].toString().toUInt();
        patientUID = json["patient_UID"].toString().toUInt();
        profile = getIE_ProfileType(json["ie_type"].toString());

        update();
    }
    int    write(QJsonObject &json)const
    {
        json["model_ID"] = QString().number(model_ID);
        json["patient_ID"] = QString().number(patientID);
        json["patient_UID"] = QString().number(patientUID);
        json["patient_fullName"] = patientFullName;
        json["ie_type"] = getIE_ProfileType(profile);
    }
    void printAllData()
    {
        qDebug() << patientFullName;
        qDebug() << modelDir;
        qDebug() << resDir;
        qDebug() << tmpDir;
        qDebug() << patientID;
        qDebug() << patientUID;
        qDebug() << model_ID;
        qDebug() << getIE_ProfileType(profile);

    }
};
/*!
\brief Класс для работы с моделью изображения.

\todo список связей с другими документами (модели).

*/
class IE_Model : public QGraphicsScene
{
Q_OBJECT
public:

                            IE_Model();
                            ~IE_Model();
    int                     saveModel();
    int                     saveModelAsImage();

    int                     read(const QJsonObject &json);
    int                     write(QJsonObject &json)const;

    QRectF                  getImageRect() const;
    qreal                   getMeasureIndex() const;
    ToolsController*        getPToolCnt() const;

    void                    setMeasureIndex(const qreal &value);
    void                    setPToolCnt(ToolsController *value);

    void                    eraseLayer(int listIndex);
    void                    eraseLayer(QList<IE_ModelLayer*>::iterator iter);
    void                    addLayer(IE_ModelLayer* layerToAdd);
    void                    showLayer(int listIndex);
    void                    hideLayer(int listIndex);


    IE_ModelLayer*          getLayerByListIndex(int listIndex);
    QList<IE_ModelLayer*>::iterator
                            getLayerIteratorByListIndex(int listIndex);

    QDockWidget*            getPDockLayers() const;

    QList<IE_ModelLayer*>::const_iterator
                            getLayersListIter() const;

    QList<IE_ModelLayer *>  getLayersList() const;

    QDockWidget*            initInfoDock();
    QDockWidget*            initLayersDock();
    QDockWidget*            initToolInfoDock();

    void                    makeHairDensityComputeWithWidget();
    void                    makeHairDiameterComputeWithWidget();
    qreal                   computeSquare();
    void                    setInputArgs();

    _Model_patientData      get_Model_patientData();

    _global_ie *getPGlobal_data() const;

signals:
    void                    changedModelSize(qreal fx, qreal fy);
    void                    measureIndexChanged(qreal measure);

public slots:
//    void initAsNewModel(QString imageFilePath);
    int                     initAsNewModel(_Model_patientData patientData);
//    void initWithModel(QString modelFilePath);
    int                     initWithModel(_Model_patientData patientData);
    void                    save(QString modelFilePath);
    void                    close(QString modelFilePath);



    void                    addLayerViaToolCnt();

private:

    QFileInfo               imageFileInfo;
    QFileInfo               modelDataFileInfo;
    QRectF                  imageRect;
    QTableView              *pDockTableView,
                            *pDockLayersTableView;
    QDockWidget             *pDockDebugInfo,
                            *pDockLayers;
    IE_LayersTableModel     *pDockLayersTableModel;

    QList<IE_ModelLayer*>   layersList;
    ToolsController         *pToolCnt;
    IE_FieldOfView_Controller *
                            m_pFieldOfViewCnt;
    ModelData               _modelData;

    _global_ie              *__global_data;

    int globalDataKey; // ключ владения глобальным объектом

    QDialog::DialogCode makeDialogForSetupModelAsNew();

private slots:
    void                    layersController();
    void                    selectedLayer(int row);
};



#endif // TSPIMAGEEDITORMODEL_H
