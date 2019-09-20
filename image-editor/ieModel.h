#ifndef TSPIMAGEEDITORMODEL_H
#define TSPIMAGEEDITORMODEL_H

#include <QtWidgets>
#include "ie_header.h"
#include "toolController.h"
#include "ie_computeModule.h"
#include "ie_modelLayer.h"
#include "ie_layersTableModel.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

QT_CHARTS_USE_NAMESPACE

struct ModelData
{

    QString patientFullName, workDir;
    QDir    modelDir,
            resDir,
            tmpDir;
    uint    patientID,
            patientUID,
            model_ID;
    void initNew(QString workPath)
    {
        model_ID = QDateTime::currentDateTime().toTime_t();
        modelDir.setPath(workPath+"/"+QString().number(model_ID));
        tmpDir.setPath("tmp/IE_sessions/"+QString().number(model_ID));
    }
    void initNew(_Model_patientData patientData)
    {
        workDir = patientData.modelDir;
        modelDir = patientData.modelDir+"/IE_sessions";
        patientID = patientData.patient_ID;
        patientUID = patientData.patient_UID;
        patientFullName = patientData.patient_fullName;
        initNew(modelDir.path());

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
        return answer;
    }

};

class TSPImageEditorModel : public QGraphicsScene
{
Q_OBJECT
public:

                            TSPImageEditorModel();
                            ~TSPImageEditorModel();
    int                    saveModel();
    void                    saveModelAsImage();

    void    read(const QJsonObject &json);
    void    write(QJsonObject &json)const;

    QRectF                  getImageRect() const;
    QGraphicsPixmapItem*    getPMainImage() const;
    qreal                   getMeasureIndex() const;
    ToolsController*        getPToolCnt() const;
    int                     setMainImage(QString imageFilePath);
    void                    setMeasureIndex(const qreal &value);
    void                    setPToolCnt(ToolsController *value);

    void                    eraseLayer(int listIndex);
    void                    eraseLayer(QList<IE_ModelLayer*>::iterator iter);
    void                    addLayer(ToolType toolType, QGraphicsItem * item);
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
    QDockWidget*            initComputeDock();
    QDockWidget*            initToolInfoDock();

    void                    makeHairDensityComputeWithWidget();
    qreal                   computeSquare();
    void                    setInputArgs();

    IE_ComputeModule*       getPCompMod() const;

    _Model_patientData      get_Model_patientData() {return _modelData.to_Model_patientData();}

signals:
    void                    changedModelSize(qreal fx, qreal fy);
    void                    measureIndexChanged(qreal measure);

public slots:
    void initAsNewModel(QString imageFilePath);
    void initAsNewModel(_Model_patientData patientData);
    void initWithModel(QString modelFilePath);
    void save(QString modelFilePath);
    void close(QString modelFilePath);


    void addLayerViaToolCnt();

private:
    QGraphicsPixmapItem *   pMainImage;

    IE_ModelLayer           *pMainImageLayer;


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
    IE_ComputeModule        *pCompMod;
    ModelData               _modelData;


    int globalDataKey; // ключ владения глобальным объектом

private slots:
    void layersController();
    void selectedLayer(int row);
};

#endif // TSPIMAGEEDITORMODEL_H

/// TODO
/// указатель на изображение

/// QImage class ?????????
