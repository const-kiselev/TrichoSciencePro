#ifndef IELINE_H
#define IELINE_H

#include <QtWidgets>
#include "ie_tool.h"



class IE_Tool_LineInfoWidget;

class IELine : public QObject, public QGraphicsLineItem, public ie_tool
{
Q_OBJECT
public:
    enum LineSettings {
        AllSettings = -1,
        None = 0,
        PenColor = (1<<1),
        BrushColor = (1<<2),
        PenWidth = (1<<3),
        WidthTip = (1<<4),
        LengthTip = (1<<5)
    };



    explicit        IELine( _global_ie *gi,
                            QObject* parent=nullptr,
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


    int    read(const QJsonObject &json)           override;
    int    write(QJsonObject &json)const           override;
    void setP_ie_global_data(_global_ie *value);



protected:
    QRectF          boundingRect() const                override;

    virtual void    paint(  QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)        override;

    virtual void    paintLength(  QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget);
    virtual void    paintWidth(  QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget);
    _global_ie *p_ie_global_data() const;

protected slots:
    void            updateSettings();
private:
    LineSettings lineSettings;
    bool editMode;
    bool lengthTip, widthTip;
    QLineF lineData;



    bool ignoreMove;
    IE_Tool_LineInfoWidget *pInfoWidget;

    _global_ie *_p_ie_global_data;
private slots:

signals:
    void lineChanged();
};


class IE_Tool_LineInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IE_Tool_LineInfoWidget(_global_ie *gi, QWidget *parent=nullptr,
                                    IELine::LineSettings settings = IELine::LineSettings::AllSettings);
    ~IE_Tool_LineInfoWidget();
    void init();
private:
    _global_ie *p_ie_global_data;
    IELine::LineSettings lineSettings;
    QLabel  *length,
            *penWidth;
    QColor  *penColor,
            *brushColor;
    QCheckBox *lengthTip, *widthTip;
signals:
    void penWidthWasChanged(int width);
    void penColorWasChanged(QColor color);
    void brushColorWasChanged(QColor color);
    void penWidthChanged_INSIDE(int width);
    void penColorChanged_INSIDE(QColor color);
    void lengthTipChecnged(bool value);
    void widthTipChecnged(bool value);
public slots:
    void changeLenght(qreal length);
    void changePenWidth(int width);
    void changePenColor(QColor color);
    void changeBrushColor(QColor color);
    void changeLengthTip(bool value);
    void changeWidthTip(bool value);
};

#endif // IELINE_H
