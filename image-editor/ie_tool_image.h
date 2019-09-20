#ifndef IE_TOOL_IMAGE_H
#define IE_TOOL_IMAGE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "ie_tool.h"
#include <QFile>
#include <QJsonObject>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPixmap>


class IE_Tool_Image:public QObject, public QGraphicsPixmapItem, public ie_tool
{
    Q_OBJECT
public:
    explicit IE_Tool_Image( QString resDir,
                            QObject* parent=nullptr,
                            QGraphicsPixmapItem* pPixmapItem = nullptr,
                            ToolType tt=ToolType::Image);
    explicit IE_Tool_Image( QString resDir,
                            QObject* parent=nullptr,
                            ToolType tt=ToolType::Image);
    explicit IE_Tool_Image( QString resDir,
                            QString tmpResDir,
                            QString relativeFilePath,
                            QString filePath,
                            ToolType tt=ToolType::Image);
    ~IE_Tool_Image() override;
    void mouseFirstPress(QPointF point) override {}
    int mouseMove(QPointF point)        override {}
    void wheelMode(QWheelEvent *pe)     override {}
    void release(QPointF point)         override {}

    bool activateEditMode()             override {}
    bool deactivateEditMode()           override {}
    void makeCompute()                  override {}
    QWidget* getWidgetPtr()             override {return nullptr;}
    void    read(const QJsonObject &json)           override;
    void    write(QJsonObject &json)const           override;
    QRectF          boundingRect() const                override;
protected:


    virtual void    paint(  QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)        override;
private:
    QFile*      _pFile;
    QFileInfo   _fileInfo;
    QDir        _resDir;
    QDir        _tmpResDir;
    QString     _relativeFilePath;

    void copyFileToWorkDir();
    void loadImage(QString filePath);
};

#endif // IE_TOOL_IMAGE_H
