#include "ie_tool_image.h"

IE_Tool_Image::IE_Tool_Image(): QObject(nullptr),
                                QGraphicsPixmapItem(nullptr),
                                ie_tool (),
                                _destDir(""),
                                _tmpDir(""),
                                _destParentDir("")
{

}

IE_Tool_Image::IE_Tool_Image(QString destDir,
                             QString destParentDir,
                             QString tmpDir,
                             QString originalFilePath,
                             ToolType tt):  QObject(nullptr),
                                            QGraphicsPixmapItem(nullptr),
                                            ie_tool (),
                                            _destDir(destDir),
                                            _tmpDir(tmpDir),
                                            _destParentDir(destParentDir)
{
    loadImage(originalFilePath);
}



IE_Tool_Image::~IE_Tool_Image()
{
    // TODO: удаление временного изображения
//    if(QFile::exists(_fileInfo.filePath()))
    //        QFile::remove(_fileInfo.filePath());
}

void IE_Tool_Image::setDirs(QString destDir, QString destParentDir, QString tmpDir)
{
    _destDir = destDir;
    _tmpDir = tmpDir;
    _destParentDir = destParentDir;
}

void IE_Tool_Image::read(const QJsonObject &json)
{
    QString filePath = QString("%1/%2").arg(_destParentDir.path()).arg(json["filePath"].toString());
    if(QFile::exists(filePath))
        loadImage(filePath);
    else
    {
        qWarning() << filePath << " doesn't exist !";
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось найти изображение %1.").arg(filePath));
        //return 1;
    }
}

void IE_Tool_Image::write(QJsonObject &json) const
{
    json["typeTitle"] = getToolTitle(ToolType::Image);
    json["fileName"]  = _fileInfo.fileName();
//    QString destFilePath = _destDir.relativeFilePath(_fileInfo.fileName());
    QString destFilePath = QString("%1/%2").arg(_destDir.path()).arg(_fileInfo.fileName());

    if(!QFile::copy(_fileInfo.filePath(), destFilePath))
    {
        json["filePath"] = _fileInfo.absolutePath();
        return;
    }



    int ind = _destDir.absolutePath().indexOf(_destParentDir.path());
    int destParentDirPathSize = _destParentDir.path().size();
    if(ind!=-1)
    {
        int ind2 = _destDir.absolutePath().indexOf("/", ind + destParentDirPathSize);
        ind2++;
        json["filePath"] = _destDir.absolutePath().mid(ind + destParentDirPathSize,ind2 )+_fileInfo.fileName();
    }
    else
    {
        json["filePath"] = _fileInfo.absolutePath();
    }

}

QRectF IE_Tool_Image::boundingRect() const
{
    return QGraphicsPixmapItem::boundingRect();
}

void IE_Tool_Image::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
}

void IE_Tool_Image::loadImage(QString filePath)
{
    while(!QFile::exists(filePath))
    {
        filePath = QFileDialog::getOpenFileName(nullptr,
                                                "Выбор изображения",
                                                QStandardPaths::displayName(
                                                QStandardPaths::DocumentsLocation )
                                                );
    }
    _fileInfo.setFile(filePath);


    if(QFile::copy(filePath, QString("%1/%2").arg(_tmpDir.path()).arg(_fileInfo.fileName())))
        filePath = QString("%1/%2").arg(_tmpDir.path()).arg(_fileInfo.fileName());

    _fileInfo.setFile(filePath);
    QGraphicsPixmapItem::setPixmap(QPixmap(filePath));

}
