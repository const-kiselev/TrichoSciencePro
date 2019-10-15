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
    /// \todo  удаление временного изображения
//    if(QFile::exists(_fileInfo.filePath()))
    //        QFile::remove(_fileInfo.filePath());
}

void IE_Tool_Image::setDirs(QString destDir, QString destParentDir, QString tmpDir)
{
    _destDir = destDir;
    _tmpDir = tmpDir;
    _destParentDir = destParentDir;
}

int IE_Tool_Image::read(const QJsonObject &json)
{
    QString filePath = QString("%1/%2").arg(_destParentDir.path()).arg(json["filePath"].toString());
    filePath = QDir::cleanPath(filePath);
    if(QFile::exists(filePath))
    {
        if(loadImage(filePath))
            return 1;
    }
    else
    {
        qWarning() << filePath << " doesn't exist !";
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось найти изображение %1.").arg(filePath));
        return 1;
    }
    return 0;
}

int IE_Tool_Image::write(QJsonObject &json) const
{
    json["typeTitle"] = getToolTitle(ToolType::Image);
    json["fileName"]  = _fileInfo.fileName();
    QString destFilePath = QString("%1/%2").arg(_destDir.path()).arg(_fileInfo.fileName());

    // Проверка на совпадение пути файла и нового файла.
    if(_fileInfo.filePath() != destFilePath)
    {
        // Копирование файла
        if(!QFile::copy(_fileInfo.filePath(), destFilePath))
        {
            json["fileName"]  = _fileInfo.fileName();
            qWarning() << "Problem with coping:"<< _fileInfo.absoluteFilePath() << " "<< destFilePath;
            json["filePath"] = _fileInfo.absoluteFilePath();
            QMessageBox::warning(nullptr,
                                 "Application",
                                 QString("Не удалось скопировать изображение %1. Из-за этого при экспорте может произойти потеря данных. Пожалуйста, проверьте, что изображение доступно для копирования и не защищенно.").arg(_fileInfo.absoluteFilePath()));
            return 1;
        }
    }

    int ind = _destDir.absolutePath().indexOf(_destParentDir.path());
    int destParentDirPathSize = _destParentDir.path().size();
    if(ind!=-1)
    {
        json["filePath"] = QDir::cleanPath(QString("%1/%2") .arg(_destDir.absolutePath().mid(ind + destParentDirPathSize+1 ))
                                                            .arg(_fileInfo.fileName())
                                           );
    }
    else
    {
        json["filePath"] = _fileInfo.absoluteFilePath();
    }
    return 0;

}

QRectF IE_Tool_Image::boundingRect() const
{
    return QGraphicsPixmapItem::boundingRect();
}

void IE_Tool_Image::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
}

int IE_Tool_Image::loadImage(QString filePath)
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
    return 0;
}
