#ifndef IE_LAYERSTABLEMODEL_H
#define IE_LAYERSTABLEMODEL_H

#include <QtWidgets>
#include "ie_header.h"
#include "ie_modelLayer.h"
#include <QAbstractTableModel>

class IE_LayersTableModel : public QAbstractTableModel
{

public:
    IE_LayersTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setPLayersList(QList<IE_ModelLayer *> *value);
public slots:
    void toChange();
    void onTableClicked(const QModelIndex &);
private:
    QList<IE_ModelLayer *>* pLayersList;


};

#endif // IE_LAYERSTABLEMODEL_H
