#include "ie_computeModule.h"

IE_ComputeModule::IE_ComputeModule(QObject *parent) : QObject(parent)
{
    //measureIndex = .0;
    pDockComputeTableView = nullptr;
    pDockComputeTableModel = new QStandardItemModel();
}

void IE_ComputeModule::addElementToCompute(IE_ModelLayer *layersData)
{
    QMap<ComputeType, StrCompTypes>::iterator res;
    switch (layersData->getToolType())
    {
    case ToolType::MainImage:
    {
        if( (res = ComputeTypeMap.find(ComputeType::ModelSquare) ) != ComputeTypeMap.end() )
        {

                    (*((*res).listIter))->append(layersData);
        }
        break;
    }
    case ToolType::DensityAndDiameter:
    {
        if( (res = ComputeTypeMap.find(ComputeType::DensityDiameterCount) ) != ComputeTypeMap.end() )
        {

                    (*((*res).listIter))->append(layersData);
        }
        break;
    }

    }
}

void IE_ComputeModule::removeElementToCompute(IE_ModelLayer *layersData)
{

}

void IE_ComputeModule::addSetting(ComputeType ct)
{
    //Q_ASSERT(IE_GLOBAL_DATA.getMeasureIndex()!=0.0);
//    if(measureIndex == 0.0)
//        emit(needMeasureIndex());

    QMap<ComputeType, StrCompTypes>::iterator res =
            ComputeTypeMap.find(ct);
    if(res != ComputeTypeMap.end())
        return;
    QModelIndex newItemIndex;
    QString labelText("tmp");
    QString dataText("0");
    switch (ct)
    {
    case ComputeType::ModelSquare:
    {
        labelText = "Площадь изображения";
        break;
    }
    case ComputeType::DensityDiameterCount:
    {
        labelText = "Колличество элементов плотности и диаметра";
        break;
    }
    }

    QList<QStandardItem*> listWithItems;
    listWithItems.append(new QStandardItem(labelText));
    listWithItems.append(new QStandardItem(dataText));
    pDockComputeTableModel->appendRow(listWithItems);
    QModelIndex idx = pDockComputeTableModel->index(pDockComputeTableModel->rowCount()-1, 1, QModelIndex());
    computeGroups.append(new QList<IE_ModelLayer*>());
    ComputeTypeMap.insert(ct, StrCompTypes(computeGroups.end()-1,idx));
    emit(needListOfElements());
}

void IE_ComputeModule::makeHairDensityComputeWithWidget()
{

}

qreal IE_ComputeModule::computeModelSquare()
{
    if(IE_GLOBAL_DATA.getMeasureIndex()==0.0)
    {
        qWarning()<<"Measure index equal 0";
        return 0;
    }
    //Q_ASSERT(IE_GLOBAL_DATA.getMeasureIndex()!=0.0);
    QMap<ComputeType, StrCompTypes>::iterator iter =
            ComputeTypeMap.find(ComputeType::ModelSquare);

    if(iter == ComputeTypeMap.end())
    {
        qDebug() << "Error. Can't compute model square, cos can't find main image";
        return 0;
    }
    if(((*(iter.value().listIter))->begin()) == (*(iter.value().listIter))->end())
        return 0;
    QGraphicsItem *pGraphItem = *((*(iter.value().listIter))->begin());
    qreal sq = (pGraphItem->boundingRect().width() *
                pGraphItem->boundingRect().height()
                ) / (
                    IE_GLOBAL_DATA.getMeasureIndex() *
                    IE_GLOBAL_DATA.getMeasureIndex()
                ) ;
    pDockComputeTableModel->item(iter.value().modelIndexComput.row(), iter.value().modelIndexComput.column())->setText(QString().number(sq));

    return sq;
}

QDockWidget *IE_ComputeModule::initViewTable()
{
    QDockWidget* pDockComputeInfo = new QDockWidget("Вычисления");
    pDockComputeTableView = new QTableView();
    pDockComputeTableView->setModel(pDockComputeTableModel);
    pDockComputeInfo->setWidget(pDockComputeTableView);
    return pDockComputeInfo;
}

void IE_ComputeModule::makeCompute()
{
    //Q_ASSERT(IE_GLOBAL_DATA.getMeasureIndex()!=0.0);
//    if(measureIndex == 0.0)
//        return;
    QMap<ComputeType, StrCompTypes>::iterator iter = ComputeTypeMap.begin();
    for (;iter!=ComputeTypeMap.end();iter++)
    {
        switch ( iter.key() )
        {
        case ComputeType::ModelSquare:
            {
                computeModelSquare();
                break;
            }
        case ComputeType::DensityDiameterCount:
            {

            break;
            }
        }
    }


}

void IE_ComputeModule::updateComputeRelatedWithToolType(ToolType tt)
{

}

void IE_ComputeModule::setMeasureIndex(qreal mIndex)
{
    //measureIndex = mIndex;
    makeCompute();
}
