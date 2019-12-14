#ifndef IE_IB_TREEMODEL_H
#define IE_IB_TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QAbstractProxyModel>

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
        //! методо задания данных из файла с данными об иерархии изображений и дополнительной информации к элементам иерархии
        int setImageBaseData(const QJsonObject &json);
        //! методо считывает из JSONobject массив элементов, которые выбрал пользователь.
        int readUserChoice(const QJsonObject &json, int index = -1);
        //! методо записывает в JSONobject массив элементов, которые выбрал пользователь.
        int writeUserChoice(QJsonObject &json, int index) const;
        //! методо задания количества списков выбора пользователя. Каждый список закрпелен за определенным FV.
        int setUserChoiseListSize(int i);

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
        //! метод проверяет, существует ли база изображений.
        //! \todo если нет, то должно происходить копирование из папки ПРИЛОЖЕНИЯ со статичными файлами-примерами.
        static bool containsImageBaseUserChoice(const QJsonObject &json);

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
