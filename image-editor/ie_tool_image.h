#ifndef IE_TOOL_IMAGE_H
#define IE_TOOL_IMAGE_H

#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QObject>
#include "ie_tool.h"
#include <QFile>
#include <QJsonObject>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPixmap>



//! \brief Класс инструмена "Изображение"
//!
//! Стоит отбратить внимание, что использование данных о трех директориях не просто так.
//! desDir — директория назначения
//! destParentDir — директория родителя, который должен знать, где находится изображение.
//! То есть происходит вычитание из desDir destParentDir и получаем относительный путь или полный путь.
//! tmpDir — директория для временного хранения.
class IE_Tool_Image:public QObject, public QGraphicsPixmapItem, public ie_tool
{
    Q_OBJECT
public:
    explicit IE_Tool_Image();
    explicit IE_Tool_Image( QString destDir,
                            QString destParentDir,
                            QString tmpDir,
                            QString originalFilePath = "",
                            ToolType tt=ToolType::Image,
                            QString dirToOpenInDialog = QStandardPaths::writableLocation(
                                                            QStandardPaths::DocumentsLocation)
                            );

    ~IE_Tool_Image() override;

    void setDirs(QString destDir,
                 QString destParentDir,
                 QString tmpDir
                );

    int loadImage(QString filePath, QString dirToOpenInDialog = QStandardPaths::writableLocation(
                QStandardPaths::DocumentsLocation));

    void mouseFirstPress(QPointF point) override {}
    int  mouseMove(QPointF point)        override {}
    void wheelMode(QWheelEvent *pe)     override {}
    void release(QPointF point)         override {}

    bool activateEditMode()             override {}
    bool deactivateEditMode()           override {}
    void makeCompute()                  override {}
    QWidget* getWidgetPtr()             override {return nullptr;}
    int    read(const QJsonObject &json)           override;
    int    write(QJsonObject &json)const           override;
    QRectF          boundingRect() const                override;
    QFileInfo getFileInfo() const;

    QFileInfo getOriginalFileInfo() const;
    void deleteFile();

protected:


    virtual void    paint(  QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)        override;
private:
    QFile*      _pFile;
    QFileInfo   _fileInfo, m_originalFileInfo;
    QDir        _destDir, _tmpDir, _destParentDir;

//    void copyFileToWorkDir();

};

#endif // IE_TOOL_IMAGE_H
