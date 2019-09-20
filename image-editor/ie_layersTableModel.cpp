#include "ie_layersTableModel.h"

IE_LayersTableModel::IE_LayersTableModel(QObject *parent) : QAbstractTableModel(parent)
{

}

int IE_LayersTableModel::rowCount(const QModelIndex &parent) const
{
    return pLayersList->count();
}

int IE_LayersTableModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant IE_LayersTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
          return QString("%1")
                      .arg( IE_ModelLayer::toStr(
                                pLayersList->at(index.row())->getToolType()
                                )
                            );
    }

    return QVariant();
}


void IE_LayersTableModel::onTableClicked(const QModelIndex &tmp)
{
//    qDebug()<< tmp.row() << " " << tmp.column();
}

void IE_LayersTableModel::setPLayersList(QList<IE_ModelLayer *> *value)
{
    pLayersList = value;
}

void IE_LayersTableModel::toChange()
{
//    emit dataChanged(createIndex(0,0), createIndex(rowCount(), 0), {Qt::DisplayRole});

}
