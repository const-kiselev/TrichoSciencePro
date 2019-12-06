#ifndef IE_IB_TREEMODEL_H
#define IE_IB_TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

class IE_IB_treeItem;

class IE_IB_treeModel : public QAbstractItemModel
{
public:
    Q_OBJECT

    public:
        const int TYPE_FIELD = 0;
        static const int SMALL_IMAGE_ROLE = 100;
        explicit IE_IB_treeModel(const QDir workDir,
                                 QObject *parent = nullptr
                                );
        ~IE_IB_treeModel();

        int setImageBaseData(const QJsonObject &json);
        int setUserChoice(const QJsonObject &json);

        QVariant data(const QModelIndex &index, int role) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;
        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        bool setData(const QModelIndex & index, const QVariant & value, int role) override;

        int read(const QJsonObject &json);
        int write(QJsonObject &json)const;

        QPixmap *getImage(const QModelIndex & index);

signals:
public slots:
        void setCurrentUserChoiceList(int i);

    private:
        void setupModelData(const QJsonArray &jsonArray, IE_IB_treeItem *parent);
        int writeModelData(QJsonObject &json, IE_IB_treeItem *parent)const;
        QString findImage(QString typeName);
        int loadImage(QHash< QString, QPixmap*> &imageHash, QString path, QString key);
        IE_IB_treeItem *rootItem;
        QHash<QString, QPixmap*> m_smallImages;
        QDir m_workDir;
        int m_currentUserChoiceVector;

        QVector< QSet<QString>* > m_userChoice;
};

#endif // IE_IB_TREEMODEL_H
