#include "pm_documentIndex.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QAction>
#include <QMenu>
PM_DocumentIndexElement::PM_DocumentIndexElement()
{

}

QString PM_DocumentIndexElement::convertType(PM_DocumentIndexElement::Type type)
{
    switch (type)
    {
    case Type::ie_model:
        return QString("ie_model");
    case Type::medicalExamRecord:
        return QString("medicalExamRecord");
    }
}
PM_DocumentIndexElement::Type PM_DocumentIndexElement::convertType(QString stringType)
{
    if(stringType == "ie_model")
        return Type::ie_model;
    if(stringType == "medicalExamRecord")
        return Type::medicalExamRecord;
}

PM_DocumentIndexElement::IE_ModelInfoStruct PM_DocumentIndexElement::getIE_ModelInfo()
{
    return m_ieModelInfo;
}
void PM_DocumentIndexElement::setIE_ModelInfo(PM_DocumentIndexElement::IE_ModelInfoStruct &iemis)
{
    m_ieModelInfo = iemis;
}

int PM_DocumentIndexElement::read(const QJsonObject &json)
{
    m_type = convertType( json["type"].toString() );
    m_alias = json["alias"].toString();
    m_path = json["path"].toString();
    m_saveDateTime =  QDateTime().fromString(json["saveDateTime"].toString(), Qt::DateFormat::ISODate);
    m_createDateTime = QDateTime().fromString(json["createDateTime"].toString(), Qt::DateFormat::ISODate);
    if(m_type == Type::ie_model)
    {
        QJsonObject infoObj = json["info"].toObject();
        m_ieModelInfo.ie_type = getIE_ProfileType(infoObj["ie_type"].toString());
    }
    return 0;
}

int PM_DocumentIndexElement::write(QJsonObject &json) const
{
    json["type"] = convertType(m_type);
    json["alias"] = m_alias;
    json["path"] = m_path;
    json["saveDateTime"] = m_saveDateTime.toString(Qt::DateFormat::ISODate);
    json["createDateTime"] = m_createDateTime.toString(Qt::DateFormat::ISODate);
    QJsonObject infoObj;
    if(m_type == Type::ie_model)
    {
        infoObj["ie_type"] = getIE_ProfileType(m_ieModelInfo.ie_type);
    }
    json["info"] = infoObj;
    return 0;
}

int PM_DocumentIndexElement::openIE_Model(QDir dir)
{
    QFile ieModelFile ;
    ieModelFile.setFileName( dir.filePath( QString("%1_IE_model.json").arg( dir.dirName().toUInt() ) ) );
    if(!ieModelFile.exists())
    {
        qWarning() << ieModelFile.fileName() << " doesn't exist.";
        return 1;
    }

    if(!ieModelFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось открыть файл %1 с моделью изображения.")
                                                                .arg(ieModelFile.fileName()));
        qWarning() << "Couldn't open file " << ieModelFile.fileName();
        return 1;
    }

    QByteArray saveData = ieModelFile.readAll();
    ieModelFile.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject jsonObj = loadDoc.object();

    if(jsonObj["TSP_docType"] != "TSP_JSON_IE_model")
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Файл %1 не является моделью изображения.").arg(ieModelFile.fileName()));
        qWarning() << ieModelFile.fileName() << "isn't TSP_JSON_IE_model.";
//        patientBaseFile.rename("data/patients/!!!patientBase_ERROR.json");
        return 1;
    }

    m_type = Type::ie_model;
    m_alias = jsonObj["alias"].toString();
    m_path = ieModelFile.fileName();
    m_createDateTime = QDateTime().fromString(jsonObj["createDateTime"].toString(), Qt::DateFormat::ISODate);
    m_saveDateTime = QDateTime().fromString(jsonObj["saveDateTime"].toString(), Qt::DateFormat::ISODate);
    m_ieModelInfo.ie_type = getIE_ProfileType(jsonObj["ie_type"].toString());

    return 0;
}

PM_DocumentIndexElement::Type PM_DocumentIndexElement::getType() const
{
    return m_type;
}
void PM_DocumentIndexElement::setType(const Type &type)
{
    m_type = type;
}
QString PM_DocumentIndexElement::getAlias() const
{
    return m_alias;
}
void PM_DocumentIndexElement::setAlias(const QString &alias)
{
    m_alias = alias;
}
QString PM_DocumentIndexElement::getPath() const
{
    return m_path;
}
void PM_DocumentIndexElement::setPath(const QString &path)
{
    m_path = path;
}
QDateTime PM_DocumentIndexElement::getCreateDateTime() const
{
    return m_createDateTime;
}
void PM_DocumentIndexElement::setCreateDateTime(const QDateTime &createDateTime)
{
    m_createDateTime = createDateTime;
}
QDateTime PM_DocumentIndexElement::getSaveDateTime() const
{
    return m_saveDateTime;
}
void PM_DocumentIndexElement::setSaveDateTime(const QDateTime &saveDateTime)
{
    m_saveDateTime = saveDateTime;
}

PM_DocumentIndexElement::IE_ModelInfoStruct PM_DocumentIndexElement::getIeModelInfo() const
{
    return m_ieModelInfo;
}

void PM_DocumentIndexElement::setIeModelInfo(const IE_ModelInfoStruct &ieModelInfo)
{
    m_ieModelInfo = ieModelInfo;
}

bool operator ==(const PM_DocumentIndexElement &left, const PM_DocumentIndexElement &right)
{
    if(left.m_path != right.m_path)
        return false;

    return true;
}

PM_DocumentIndexCnt::PM_DocumentIndexCnt(QDir workDir, QObject *parent):
                                                                QObject(parent),
                                                                m_workDir(workDir)
{
    m_widget = nullptr;
    m_tableView = nullptr;
    m_documentIndexModel = new QStandardItemModel(0, 5);
    connect(this, &PM_DocumentIndexCnt::documentIndexWasChanged, &PM_DocumentIndexCnt::updateTable);
}

void PM_DocumentIndexCnt::findAllDocuments()
{
    //! Получаем список директорий. Если эта директория проименована типом документа, то хаходим в эту директорию
    //! и обрабатываем документы, добавляем их в список.
    m_documentList.clear();
    QStringList folderList = m_workDir.entryList(QDir::Dirs);
    foreach(QString subdir, folderList)
    {
        if(subdir == "." || subdir == "..")
            continue;
        if(subdir == "IE_models")
        {
            QDir ieModelsDir = m_workDir;
            ieModelsDir.cd(subdir);
            QStringList ieModelFolderList = ieModelsDir.entryList(QDir::Dirs);
            foreach(QString ieModelFoldersubdir, ieModelFolderList)
            {
                if(ieModelFoldersubdir == "." || ieModelFoldersubdir == "..")
                    continue;
                PM_DocumentIndexElement docElem;
                QDir tmpDir = QDir(ieModelsDir);
                tmpDir.cd(ieModelFoldersubdir);
                docElem.openIE_Model( tmpDir );
                m_documentList.append(docElem);
            }
        }
    }
    save();
    emit documentIndexWasChanged();
}

int PM_DocumentIndexCnt::save()
{
    QFile documentIndexFile;
    documentIndexFile.setFileName( m_workDir.filePath("documentIndex.json") );
    if(!documentIndexFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open documentIndex file "<<documentIndexFile.fileName();
        return 1;
    }
    QJsonObject documentIndexObj;
    documentIndexObj["TSP_docType"] = "TSP_JSON_documentIndex";
    write(documentIndexObj);
    QJsonDocument saveDoc(documentIndexObj);
    documentIndexFile.write(saveDoc.toJson());
    documentIndexFile.close();
    return 0;
}

int PM_DocumentIndexCnt::open()
{
    QFile documentIndexFile;
    documentIndexFile.setFileName( m_workDir.filePath("documentIndex.json") );

    if(!documentIndexFile.exists())
    {
        return 1;
    }
    if(!documentIndexFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось открыть файл %1 с документами.")
                                                                .arg(documentIndexFile.fileName()));
        qWarning() << "Couldn't open documentIndex file "<<documentIndexFile.fileName();
        return 1;
    }

    QByteArray saveData = documentIndexFile.readAll();
    documentIndexFile.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));


    QJsonObject jsonDoc = loadDoc.object();

    if(jsonDoc["TSP_docType"] != "TSP_JSON_documentIndex")
    {
        qDebug() << documentIndexFile.fileName() << "isn't TSP_JSON_documentIndex.";
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось прочитать файл %1 с документами.")
                                                                .arg(documentIndexFile.fileName()));
        return 1;
    }

     return read(jsonDoc);
}

void PM_DocumentIndexCnt::addDocument(PM_DocumentIndexElement &iemis)
{
    m_documentList.append(iemis);
    save();
    emit documentIndexWasChanged();
}

void PM_DocumentIndexCnt::deleteDocument(PM_DocumentIndexElement &iemis)
{
    int i = 0;
    foreach(PM_DocumentIndexElement die, m_documentList)
    {

        if(die == iemis)
        {
            m_documentList.removeAt(i);
            QDir dirToRemove = QDir(iemis.getPath());
            if(QFileInfo(iemis.getPath()).isFile())
                dirToRemove.cdUp();
            dirToRemove.removeRecursively();
            break;
        }
        i++;

    }
    save();
    emit documentIndexWasChanged();
}

int PM_DocumentIndexCnt::read(const QJsonObject &json)
{
    m_documentList.clear();

    QJsonArray documentArray = json["documentArray"].toArray();
    for (int docIndex = 0; docIndex < documentArray.size(); ++docIndex)
    {
        QJsonObject docIndexElemObject = documentArray[docIndex].toObject();
        PM_DocumentIndexElement die;
        die.read( docIndexElemObject );
        m_documentList.append( die );
    }
    emit documentIndexWasChanged();
    return 0;
}

int PM_DocumentIndexCnt::write(QJsonObject &json) const
{
    QJsonArray documentArray;
    foreach(PM_DocumentIndexElement die, m_documentList)
    {
        QJsonObject dieObj;
        die.write(dieObj);
        documentArray.append(dieObj);
    }
    json["documentArray"] = documentArray;

    return 0;
}

void PM_DocumentIndexCnt::updateTable()
{
    m_documentIndexModel->clear();
    QStringList str;
    str << "Type" << "Alias" << "Path" << "Create" << "Save";
    m_documentIndexModel->setHorizontalHeaderLabels( str );
    foreach(PM_DocumentIndexElement die, m_documentList)
    {
        QList<QStandardItem*> row;
        if( die.getType() == PM_DocumentIndexElement::Type::ie_model )
            row.append(new QStandardItem( QString( getIE_ProfileType(die.getIE_ModelInfo().ie_type ) ) ) );
        else
            row.append(new QStandardItem( QString(  PM_DocumentIndexElement::convertType(die.getType()) ) ) );
        row.append(new QStandardItem( QString(  die.getAlias() ) ) );
        row.append(new QStandardItem( QString(  die.getPath() ) ) );
        row.append(new QStandardItem( QString(  die.getCreateDateTime().toString(Qt::DateFormat::ISODate) ) ) );
        row.append(new QStandardItem( QString(  die.getSaveDateTime().toString(Qt::DateFormat::ISODate) ) ) );
        m_documentIndexModel->appendRow(row);
    }
}

void PM_DocumentIndexCnt::changeFilter(PM_DocumentIndexCnt::FilterType ft)
{

}

QWidget * PM_DocumentIndexCnt::getWidget()
{
    if(!m_widget)
        initWidget();
    return m_widget;
}

void PM_DocumentIndexCnt::initWidget()
{
    m_widget = new QWidget();
    QVBoxLayout * pvbLayout = new QVBoxLayout(m_widget);
    m_tableView = new QTableView(m_widget);
    m_tableView->setModel(m_documentIndexModel);
    m_tableView->hideColumn(1);
//    m_tableView->hideColumn(2);
    m_tableView->setSortingEnabled(true);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    pvbLayout->addWidget(m_tableView);

//    m_tableView->do



//    connect(m_tableView->selectionModel(), &QItemSelectionModel::selectionChanged,)

    m_widget->setLayout(pvbLayout);



    connect(m_tableView, &QTableView::doubleClicked, [this](const QModelIndex &index)
    {
        emit openImageEditor( m_tableView->model()->index(index.row(), 2).data().toString() );
    });

    connect(m_tableView, &QTableView::customContextMenuRequested,
                [this](const QPoint &pos)
    {
        QMenu contextMenu("Context menu");
        QModelIndex index = m_tableView->indexAt(pos);
        if(index.row()==-1)
            return;
        QAction action1("Удалить документ");
        connect(&action1, &QAction::triggered, [this, index](){
            PM_DocumentIndexElement elem;
    //        elem.setType( PM_DocumentIndexElement::convertType(m_tableView->model()->index(index.row(), 0).data().toString()) );
    //        elem.setAlias( m_tableView->model()->index(index.row(), 1).data().toString() );
            elem.setPath( m_tableView->model()->index(index.row(), 2).data().toString() );
    //        elem.setCreateDateTime( QDateTime::fromString(m_tableView->model()->index(index.row(), 3).data().toString(), Qt::DateFormat::ISODate) );
    //        elem.setSaveDateTime( QDateTime::fromString(m_tableView->model()->index(index.row(), 4).data().toString(), Qt::DateFormat::ISODate) );
            deleteDocument(elem);
        });
        contextMenu.addAction(&action1);
        contextMenu.exec(m_tableView->mapToGlobal(pos));

    });

}
