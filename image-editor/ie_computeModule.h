#ifndef IE_COMPUTEMODULE_H
#define IE_COMPUTEMODULE_H

#include <QWidget>
#include "toolController.h"

class IE_ComputeModule : public QObject
{
    Q_OBJECT
public:
    explicit IE_ComputeModule(QObject *parent = nullptr);

    void addElementToCompute(IE_ModelLayer *layersData);
    void removeElementToCompute(IE_ModelLayer *layersData);
    void addSetting(ComputeType ct);
    void makeHairDensityComputeWithWidget();

    qreal computeModelSquare();
    QDockWidget* initViewTable();
signals:
    void needListOfElements();
    void needMeasureIndex();

public slots:
    void makeCompute();
    void updateComputeRelatedWithToolType(ToolType tt);
    void setMeasureIndex(qreal mIndex);

private:
    struct StrCompTypes {
        QList<QList<IE_ModelLayer*>*>::iterator listIter;
        QModelIndex modelIndexComput;
        StrCompTypes(QList<QList<IE_ModelLayer*>*>::iterator inIter, QModelIndex inIndex)
        {
            listIter = inIter;
            modelIndexComput = inIndex;
        }
    };


    QMap<ComputeType, StrCompTypes> ComputeTypeMap;
    QList<QList<IE_ModelLayer*>*> computeGroups;
    QTableView *pDockComputeTableView;
    QStandardItemModel *pDockComputeTableModel;
    //qreal measureIndex;
};

#endif // IE_COMPUTEMODULE_H
