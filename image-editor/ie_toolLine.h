#ifndef IELINE_H
#define IELINE_H

#include <QtWidgets>
#include "ie_tool.h"

enum LineSettings {
    AllSettings = -1,
    None = 0,
    PenColor = (1<<1),
    BrushColor = (1<<2),
    PenWidth = (1<<3)
};

class IE_Tool_LineInfoWidget;

class IELine : public QObject, public QGraphicsLineItem, public ie_tool
{
Q_OBJECT
public:
/*
//    struct metaType
//    {
//        qreal width;
//        int color;
//        qreal x1,x2,y1,y2;
//        metaType(qreal inWidth, int inColor, qreal inX1, qreal inX2, qreal inY1, qreal inY2 )
//        {
//            width = inWidth;
//            color = inColor;
//            x1 = inX1;
//            x2 = inX2;
//            y1 = inY1;
//            y2 = inY2;
//        }
//       QByteArray getAllDataInJSONQByteArray()
//       {
//           QVariantMap data;

//           data.insert("width", width);
//           data.insert("color", color);
//           data.insert("x1", x1);
//           data.insert("x2", x2);
//           data.insert("y1", y1);
//           data.insert("y2", y2);

//           QJsonObject json;
//           json.fromVariantMap(data);
//           QJsonDocument doc(json);
//           QByteArray docByteArray = doc.toJson(QJsonDocument::Compact);
//           return docByteArray;
//       }
//    }metaData;
*/
    explicit        IELine( QObject* parent=nullptr,
                            ToolType tt = ToolType::SimpleLine,
                            LineSettings settings = LineSettings::AllSettings);

    void            mouseFirstPress(QPointF point)  override;
    int             mouseMove(QPointF point)        override;
    virtual void    wheelMode(QWheelEvent *pe)      override;
    void            release(QPointF point)          override;

    QLineF          getLine() const;
    qreal           getLineLength();
    qreal           getPenWidth()const
                        {return pen().widthF();}

    bool            activateEditMode()              override;
    bool            deactivateEditMode()            override;
    void            makeCompute()                   override;
    QWidget*        getWidgetPtr()                  override;


    void    read(const QJsonObject &json)           override;
    void    write(QJsonObject &json)const           override;
protected:
    QRectF          boundingRect() const                override;

    virtual void    paint(  QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)        override;
    QPen _pen;
protected slots:
    void            updateSettings();
private:
    LineSettings lineSettings;
    bool editMode;
    QLineF lineData;

    bool ignoreMove;
    IE_Tool_LineInfoWidget *pInfoWidget;
private slots:

    void penWidthChanged(qreal width);
signals:
    void lineChanged();
};


class IE_Tool_LineInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IE_Tool_LineInfoWidget(QWidget *parent=nullptr,
                                    LineSettings settings = LineSettings::AllSettings);
    ~IE_Tool_LineInfoWidget();
    void init();
private:
    LineSettings lineSettings;
    QLabel  *length,
            *penWidth;
    QColor  *penColor,
            *brushColor;
signals:
    void penWidthWasChanged(int width);
    void penColorWasChanged(QColor color);
    void brushColorWasChanged(QColor color);
    void penWidthChanged_INSIDE(int width);
    void penColorChanged_INSIDE(QColor color);
public slots:
    void changeLenght(qreal length);
    void changePenWidth(int width);
    void changePenColor(QColor color);
    void changeBrushColor(QColor color);
};

#endif // IELINE_H
