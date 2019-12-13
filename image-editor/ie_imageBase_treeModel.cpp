#include "ie_imageBase_treeModel.h"
#include "ie_imageBase_treeItem.h"
#include <QDebug>

#include <QPixmap>
#include <QStringList>

IE_IB_treeModel::IE_IB_treeModel(const QDir workDir, QObject *parent)
    : QAbstractItemModel(parent), m_workDir(workDir)
{
    rootItem = new IE_IB_treeItem({"","","", "","",""});
    m_currentUserChoiceVector = -1;
}


IE_IB_treeModel::~IE_IB_treeModel()
{
    delete rootItem;
}

int IE_IB_treeModel::setImageBaseData(const QJsonObject &json)
{
    if(!json["data"].isArray())
        return 1;
    setupModelData(json["data"].toArray(), rootItem);
    return 0;
}

int IE_IB_treeModel::readUserChoice(const QJsonObject &json, int index)
{
    if(index >= m_userChoice.size())
        return 1;
    if(!json["imageBaseUserChoiceArray"].isArray())
        return 0;

    QSet<QString> * typeSet = new QSet<QString>;


    QJsonArray ucArray = json["imageBaseUserChoiceArray"].toArray();
    for(int i=0, ucArraySize = ucArray.size(); i<ucArraySize; i++)
    {
        QString ucType = ucArray.at(i).toString();
        typeSet->insert(ucType);
//        IE_IB_treeItem * pCurrentRoot = rootItem;
//        int lastFound = ucType.indexOf("_");
//        while( lastFound != -1 )
//        {

//            QString preType = ucType.left( lastFound - 1 );
//            for( int childIndex = 0, childCount = pCurrentRoot->childCount();
//                 childIndex < childCount; childIndex++)
//            {
//                if( !pCurrentRoot->child(childIndex)->data(0).toString()
//                        .compare( preType, Qt::CaseInsensitive )
//                        )
//                {
//                    pCurrentRoot = pCurrentRoot->child(childIndex);
//                    if( !pCurrentRoot->childCount() ) // искомый тип
//                    {
//                        lastFound = -1;
//                        *typeSet << ucType;
//                    }
//                    else
//                        lastFound = ucType.indexOf("_", lastFound);
//                    break;
//                }
//            }
//        }
    }
    if(index == -1)
        m_userChoice << typeSet;
    else
        m_userChoice.replace(index, typeSet);

    return 0;
}

int IE_IB_treeModel::writeUserChoice(QJsonObject &json, int index) const
{
    if(!m_userChoice.size() || index >= m_userChoice.size())
        return 1;


    QSet<QString>::const_iterator setIter = m_userChoice.at(index)->constBegin();
    QJsonArray ucArray;
    while (setIter != m_userChoice.at(index)->constEnd())
    {
           ucArray.append(*setIter);
           qDebug() << *setIter;
           ++setIter;
    }

    json["imageBaseUserChoiceArray"] = ucArray;
    return 0;
}

int IE_IB_treeModel::setUserChoiseListSize(int i)
{
    if( i ==  m_userChoice.size())
        return true;
    if(i < m_userChoice.size())
        while(i!= m_userChoice.size())
            m_userChoice.removeLast();
    else
        while(i!= m_userChoice.size())
            m_userChoice << new QSet<QString>;
    return true;
}


int IE_IB_treeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<IE_IB_treeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

bool IE_IB_treeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (role) {
    case Qt::CheckStateRole:
    {
        IE_IB_treeItem *item = static_cast<IE_IB_treeItem*>(index.internalPointer());

        if(m_currentUserChoiceVector == -1)
            return -1;

        if( value.toBool() )
        {
            m_userChoice.at(m_currentUserChoiceVector)->insert( item->data(0).toString() );
        }
        else
        {
            m_userChoice.at(m_currentUserChoiceVector)->remove( item->data(0).toString() );
        }

        return true;


//        return item->setData(5, value);
    }
    default:
        return false;

    }
}

int IE_IB_treeModel::write(QJsonObject &json) const
{
    writeModelData(json, rootItem);
    return 0;
}

QPixmap* IE_IB_treeModel::getImage(const QModelIndex &index)
{
    QPixmap * pPix = m_smallImages.find(  data(index, SMALL_IMAGE_ROLE).toString() ).value();
    return pPix;
    return new QPixmap();
}

bool IE_IB_treeModel::containsImageBaseUserChoice(const QJsonObject &json)
{
    return json["imageBaseUserChoiceArray"].isArray();
}

void IE_IB_treeModel::setCurrentUserChoiceList(int i)
{
    m_currentUserChoiceVector = i;
    emit dataChanged( index(0,0), index(rowCount(), columnCount()) );
}


QVariant IE_IB_treeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        IE_IB_treeItem *item = static_cast<IE_IB_treeItem*>(index.internalPointer());

        return item->data(1);
    }
    case Qt::CheckStateRole:
    {
        IE_IB_treeItem *item = static_cast<IE_IB_treeItem*>(index.internalPointer());

        if(m_currentUserChoiceVector == -1)
            return false;

        if( item->childCount() )
            return QVariant();

        if( m_userChoice.at(m_currentUserChoiceVector)->contains(item->data(0).toString()) )
            return true;
        else
            return false;
    }
    case SMALL_IMAGE_ROLE:
    {
        IE_IB_treeItem *item = static_cast<IE_IB_treeItem*>(index.internalPointer());

        return item->data(4).toString();
//        return QVariant();
    }
    default:
        return QVariant();

    }
}


Qt::ItemFlags IE_IB_treeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
//    return QAbstractItemModel::flags(index);
}


QVariant IE_IB_treeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}


QModelIndex IE_IB_treeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    IE_IB_treeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<IE_IB_treeItem*>(parent.internalPointer());

    IE_IB_treeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}


QModelIndex IE_IB_treeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    IE_IB_treeItem *childItem = static_cast<IE_IB_treeItem*>(index.internalPointer());
    IE_IB_treeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


int IE_IB_treeModel::rowCount(const QModelIndex &parent) const
{
    IE_IB_treeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<IE_IB_treeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void IE_IB_treeModel::setupModelData(const QJsonArray &jsonArray, IE_IB_treeItem *parent)
{
    QVector<IE_IB_treeItem*> parents;
    parents << parent;


    for(int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject arrElemObj = jsonArray.at(i).toObject();

        QVector<QVariant> data;
        QString typeStr;
        // случай, когда родитель пустой, то есть корень!
        if(parents.last()->data(0).toString().isEmpty())
            typeStr = QString("%1").arg(arrElemObj["type"].toString());
        else
            typeStr = QString("%1_%2")  .arg( parents.last()->data(0).toString() )
                                        .arg(arrElemObj["type"].toString());
        if(arrElemObj.contains("data"))
        {
            data = {typeStr,
                    arrElemObj["name"].toString(),
                    arrElemObj["note"].toString(), "", "", false
                   };
        }
        else
            data = {typeStr,
                    arrElemObj["name"].toString(),
                    arrElemObj["note"].toString(),
                    arrElemObj["relatedToolsArray"].toArray().toVariantList(),
                    findImage(typeStr),
                    false
                   };


        if( !data[4].toString().isEmpty() )
            loadImage( m_smallImages, data[4].toString() , data[0].toString());


        IE_IB_treeItem * child = new IE_IB_treeItem(data, parents.last());
        parents.last()->appendChild( child );
        if(arrElemObj["data"].isArray())
        {
            setupModelData(arrElemObj["data"].toArray(), child);
        }
    }
}

int IE_IB_treeModel::writeModelData(QJsonObject &json, IE_IB_treeItem *parent) const
{

    QJsonArray array;
    for(int i=0; parent->columnCount(); i++)
    {
        IE_IB_treeItem * current = parent->child(i);
        QJsonObject arrElem;
        {
            if(parent->parentItem())
            {
                QString parentType = parent->parentItem()->data(0).toString();
                int m = parentType.length()+1; // с учетом '_'
                arrElem["type"] = parent->data(0).toString().mid(m);
            }
            else
                arrElem["type"] = parent->data(0).toString();
        }
        arrElem["name"] = current->data(1).toString();
        arrElem["note"] = current->data(2).toString();
        if(current->childCount())
        {
            QJsonObject childObj;
            writeModelData(childObj, current);
            arrElem["data"] = childObj;
        }
        else
        {
            QJsonArray relatedToolsArray;
            QStringList relatedToolsList = current->data(3).toStringList();
            foreach (QString relatedTool, relatedToolsList)
                relatedToolsArray.append(relatedTool);
            arrElem["relatedToolsArray"] = relatedToolsArray;
        }
        array.append(arrElem);
    }
    QJsonObject elemObj;
    if(parent->parentItem())
    {
        QString parentType = parent->parentItem()->data(0).toString();
        int m = parentType.length()+1; // с учетом '_'
        elemObj["type"] = parent->data(0).toString().mid(m);
    }
    else
        elemObj["type"] = parent->data(0).toString();
    json["data"] = array;

    return 0;
}

QString IE_IB_treeModel::findImage(QString typeName)
{
    QDir current = m_workDir;
    QString path = typeName.replace("_", "/");
    QString fileName = path.mid(path.lastIndexOf("/")+1);
    path = path.left( path.lastIndexOf("/") );
    if( !current.cd(path) )
        return QString();
    QStringList result = current.entryList( QStringList() << QString("%1.png").arg(fileName), QDir::Files );
    if(result.isEmpty())
        return QString();
    return current.filePath(result[0]);
}

int IE_IB_treeModel::loadImage(QHash<QString, QPixmap *> &imageHash, QString path, QString key)
{
//    QPixmap * pimg = new QPixmap(path);
//    imageHash.insert(key, pimg);
    return 0;
}
