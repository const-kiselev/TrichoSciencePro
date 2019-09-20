#include "ie_tool_image.h"


IE_Tool_Image::IE_Tool_Image(QString resDir,
                             QObject *parent,
                             QGraphicsPixmapItem* pPixmapItem,
                             ToolType tt):  QObject(parent),
                                            QGraphicsPixmapItem(pPixmapItem),
                                            ie_tool (),
                                            _resDir(resDir)
{

}

IE_Tool_Image::IE_Tool_Image(QString resDir,
                             QObject *parent,
                             ToolType tt):  QObject(nullptr),
                                            QGraphicsPixmapItem(nullptr),
                                            ie_tool (),
                                            _resDir(resDir)
{

}

IE_Tool_Image::IE_Tool_Image(QString resDir,
                             QString tmpResDir,
                             QString relativeFilePath,
                             QString filePath,
                             ToolType tt):  QObject(nullptr),
                                            QGraphicsPixmapItem(nullptr),
                                            ie_tool (),
                                            _resDir(resDir),
                                            _tmpResDir(tmpResDir),
                                            _relativeFilePath(relativeFilePath)
{
    qDebug() << "IE_Tool_Image::IE_Tool_Image\t_resDir = " << _resDir
             << "\t_relativeFilePath = " << _relativeFilePath;
    loadImage(filePath);
}

IE_Tool_Image::~IE_Tool_Image()
{
    if(QFile::exists(_fileInfo.filePath()))
        QFile::remove(_fileInfo.filePath());
}

void IE_Tool_Image::read(const QJsonObject &json)
{

}

void IE_Tool_Image::write(QJsonObject &json) const
{
    QFile::copy(_fileInfo.filePath(), _resDir.path()+"/"+_fileInfo.fileName());
    json["typeTitle"] = getToolTitle(ToolType::Image);
    json["fileName"] = _fileInfo.fileName();
    json["filePath"] = _relativeFilePath;
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
                                                    QStandardPaths::HomeLocation ) );
    }
    QString fileName = filePath.section("/",-1,-1);

    if(QFile::copy(filePath, _tmpResDir.path()+"/"+fileName))
    {
        filePath = _tmpResDir.path()+"/"+fileName;
        _relativeFilePath +="/"+fileName;
    }
    _fileInfo.setFile(filePath);
    QGraphicsPixmapItem::setPixmap(QPixmap(filePath));
}
