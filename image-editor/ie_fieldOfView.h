#ifndef IE_FIELDOFVIEW_H
#define IE_FIELDOFVIEW_H

#include <QObject>
#include <QRect>
#include "ie_toolController.h"

class IE_FieldOfView : public QObject
{
    Q_OBJECT
public:
    explicit IE_FieldOfView(QObject *parent = nullptr);
    int                     read(const QJsonObject &json);
    int                     write(QJsonObject &json)const;

    void getLayers(){}
    void removeLayers(){}
    void getSquare() {}
    void move() {}
    QFileInfo getMainImageFileInfo(){}
    void setMainImage(QString filePath);

signals:

public slots:
private:
    QRectF rectData;

};

#endif // IE_FIELDOFVIEW_H
