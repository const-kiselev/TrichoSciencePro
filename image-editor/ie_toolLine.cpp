#include "ie_toolLine.h"

            IELine::IELine              (_global_ie *gi,
                                         QObject* parent,
                                         ToolType tt,
                                         LineSettings settings):
                                                QObject(parent),
                                                QGraphicsLineItem (nullptr),
                                                ie_tool(),
                                                lineSettings(settings),
                                                _p_ie_global_data(gi)
{
    QPen locPen(QPen(Qt::black, 2));

    locPen.setCapStyle(Qt::FlatCap);
    locPen.setJoinStyle(Qt::BevelJoin);
    setPen(locPen);

    editMode = false;
    lengthTip = false;
    widthTip = false;
    lineData.setP1(QPointF(-1,-1));
    lineData.setP2(QPointF(-1,-1));
    ignoreMove = false;

    pInfoWidget = new IE_Tool_LineInfoWidget(p_ie_global_data() ,nullptr, settings);
    pInfoWidget->changeLenght(0);
    pInfoWidget->changePenColor(pen().color());
    pInfoWidget->changePenWidth(1);
//    setPToolInfoWidget(pInfoWidget);

    connect(pInfoWidget, &IE_Tool_LineInfoWidget::penWidthWasChanged,
                          [this](int width)
    {
        QPen locPen(this->pen());
        locPen.setWidthF(width);
        this->setPen(locPen);
        this->updateSettings();
    });

    connect(pInfoWidget, &IE_Tool_LineInfoWidget::penColorWasChanged,
                              [this](QColor color)
        {
            QPen locPen(this->pen());
            locPen.setColor(color);
            this->setPen(locPen);
            this->updateSettings();
        });

    connect(this, &IELine::lineChanged, [this]()
    {
        pInfoWidget->changeLenght(getLineLength());
        pInfoWidget->changePenWidth(pen().width());
        pInfoWidget->changePenColor(pen().color());
    });

    connect(pInfoWidget, &IE_Tool_LineInfoWidget::lengthTipChecnged,
                              [this](bool value)
        {
            lengthTip = value;
            this->updateSettings();
        }
    );

    connect(pInfoWidget, &IE_Tool_LineInfoWidget::widthTipChecnged,
                              [this](bool value)
        {
            widthTip = value;
            this->updateSettings();
        }
    );


    pInfoWidget->init();

}

// ------- EVENTS
void        IELine::mouseFirstPress     (QPointF point)
{
    if(lineData.p1() == QPointF(-1,-1))
    {
        lineData.setP1(point);
        ignoreMove = false;
    }
    // режим редактирования линии
    else {
        if(QLineF(point,lineData.p1()).length() <= 2*pen().widthF())
        {
            QPointF tmp(lineData.p2());
            lineData.setP2(lineData.p1());
            lineData.setP1(tmp);
        }
        else if (QLineF(point,lineData.p2()).length() > 2*pen().widthF()) {
            ignoreMove = true;
        }

    }
}
int         IELine::mouseMove           (QPointF point)
{
    emit lineChanged();
    if(ignoreMove)
        return 1;
    lineData.setP2(point);
    setLine(lineData);
    return 0;
}
void        IELine::wheelMode           (QWheelEvent* pe)
{
    QPen locPen(pen());
    QPoint nAngle = pe->angleDelta();
    if (nAngle.isNull())
        return;

    int iWidth = pen().width();



    if(nAngle.ry()>0)
        iWidth++;
    else if(nAngle.ry()<0)
        iWidth--;
    else {
        return;
    }
    if(iWidth>1)
        {
            locPen.setWidth(iWidth);
        }

//     else if (!nPixels.isNull())
//        {
//        if(iWidth+abs(nPixels.ry())/nPixels.ry()>1)
//        {
//            qDebug()<< iWidth+abs(nPixels.ry())/nPixels.ry();
//            pen.setWidth(iWidth+abs(nPixels.ry())/nPixels.ry());
//        }
//        }


    setPen(locPen);
    setLine(lineData);
    updateSettings();
}

void        IELine::release             (QPointF point)
{
    ignoreMove = false;
}

QLineF      IELine::getLine             () const
{
    return line();
}
qreal       IELine::getLineLength       ()
{
    return line().length();
}

void        IELine::paint               (QPainter *painter,
                                         const QStyleOptionGraphicsItem *option,
                                         QWidget *widget
                                         )
{
    QGraphicsLineItem::paint( painter, option, widget );



    if(editMode)
    {
        QPen localPen(pen());
        localPen.setWidth(1);
        localPen.setColor(Qt::red);

        painter->setPen(localPen);
        painter->setBrush(QBrush(Qt::white));

        painter->drawEllipse(line().p1() , 5, 5);
        painter->drawEllipse(line().p2() , 5, 5);
    }
    if(lengthTip)
        paintLength(painter, option, widget);
    if(widthTip)
        paintWidth(painter, option, widget);





}

void IELine::paintLength(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    qreal angle;
    int sign = -1;
    QPointF p1 = line().p1(), p2 = line().p2();

    QString text = QString("%1 %2")  .arg( p_ie_global_data()->convertF( line().length() ) )
            .arg(UnitTypeTitle[p_ie_global_data()->getUnitType()]);

    QPainterPath path;
    QFont f("times new roman,utopia");
    f.setStyleStrategy(QFont::ForceOutline);
    f.setPointSize(15);
    f.setStyleHint(QFont::Helvetica);
    path.addText(0, 0, f, text);

    angle = line().angle();
//    if(angle > 90 || angle > 180)
//    {
//        QPointF tmpPoint = p2;
//        p1 = p2;
//        p2 = tmpPoint;
//        sign = 1;
//    }

    QFontMetrics fm(f);
    QRectF br(fm.boundingRect(text));
    QPointF center(br.center());
    center.setY( (pen().widthF()/qreal(2) + 15)*sign);
    QPointF centerF ( (p1.x() + p2.x())/qreal(2),
                      (p1.y() + p2.y())/qreal(2)
                      );


    painter->translate(centerF - center);
    painter->translate(center);
    painter->rotate(-line().angle());
    painter->translate(-center);
    painter->fillPath(path, Qt::black);

    painter->setPen(QPen(QColor(255, 0, 0, 0), 0.25, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(br.adjusted(-1, -1, 1, 1));

//    qreal selectionOffset = pen().widthF()/2. + 5;
//    QPen locPen = pen();
//    locPen.setWidth(1);
//    painter->setPen(locPen);
//    qreal radAngle = line().angle()* M_PI / 180;


//    qreal dx = selectionOffset * sin(radAngle);
//    qreal dy = selectionOffset * cos(radAngle);

//    QPointF offset1 = QPointF(dx, dy);





    painter->restore();
}

void IELine::paintWidth(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    qreal angle;
    int sign = -1;
    QPointF p1 = line().p1(), p2 = line().p2();

    QString text = QString("%1 %2")  .arg( p_ie_global_data()->convertF( pen().widthF() ) )
            .arg(UnitTypeTitle[p_ie_global_data()->getUnitType()]);

    QPainterPath path;
    QFont f("times new roman,utopia");
    f.setStyleStrategy(QFont::ForceOutline);
    f.setPointSize(15);
    f.setStyleHint(QFont::Helvetica);
    path.addText(0, 0, f, text);

    angle = line().angle();
//    if(angle > 90 || angle > 180)
//    {
//        QPointF tmpPoint = p2;
//        p1 = p2;
//        p2 = tmpPoint;
//        sign = 1;
//    }

    QFontMetrics fm(f);
    QRectF br(fm.boundingRect(text));
    QPointF center(br.center());
    center.setY(15);
    QPointF centerF (p1);


    painter->translate(centerF - center);
    painter->translate(center);
    painter->rotate(90-line().angle());
    painter->translate(-center);
    painter->fillPath(path, Qt::black);

    painter->setPen(QPen(QColor(255, 0, 0, 0), 0.25, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(br.adjusted(-1, -1, 1, 1));

//    qreal selectionOffset = pen().widthF()/2. + 5;
//    QPen locPen = pen();
//    locPen.setWidth(1);
//    painter->setPen(locPen);
//    qreal radAngle = line().angle()* M_PI / 180;


//    qreal dx = selectionOffset * sin(radAngle);
//    qreal dy = selectionOffset * cos(radAngle);

//    QPointF offset1 = QPointF(dx, dy);





    painter->restore();
}

void        IELine::updateSettings      ()
{
    //setPen(pen);
    setLine(lineData);

    pInfoWidget->changeLenght(getLineLength());
    pInfoWidget->changePenColor(pen().color());
    pInfoWidget->changePenWidth(pen().width());

    update();
}

_global_ie *IELine::p_ie_global_data() const
{
    return _p_ie_global_data;
}

void IELine::setP_ie_global_data(_global_ie *value)
{
    _p_ie_global_data = value;
}

bool        IELine::activateEditMode    ()
{
    editMode = true;
    update();
    return editMode;
}
bool        IELine::deactivateEditMode  ()
{
    editMode = false;
    update();
    return editMode;
}

void        IELine::makeCompute         ()
{

}


QWidget*    IELine::getWidgetPtr        ()
{return pInfoWidget;}

int         IELine::read                (const QJsonObject &json)
{
    QColor locColor;
    locColor.setNamedColor(json["line_penColor"].toString());
    QPen locPen;
    locPen.setColor(locColor);
    locPen.setWidthF(json["line_penWidth"].toDouble());
    locPen.setStyle(static_cast<Qt::PenStyle>(json["line_penStyle"].toInt()));

    setPen(locPen);

    lineData.setLine(   json["line_point1_posX"].toDouble(),
                        json["line_point1_posY"].toDouble(),
                        json["line_point2_posX"].toDouble(),
                        json["line_point2_posY"].toDouble()
                    );
    updateSettings();
    return 0;
}

int         IELine::write               (QJsonObject &json) const
{
    json["typeTitle"] = getToolTitle(ToolType::SimpleLine);

    json["line_penColor"] = pen().color().name();
    json["line_penWidth"] = pen().widthF();
    json["line_penStyle"] = static_cast<int>(pen().style()) ;

    json["line_point1_posX"] = line().p1().x();
    json["line_point1_posY"] = line().p1().y();
    json["line_point2_posX"] = line().p2().x();
    json["line_point2_posY"] = line().p2().y();

    return 0;
}

QRectF      IELine::boundingRect        () const
{
    QRectF rect(QGraphicsLineItem::boundingRect());

        rect.setLeft(rect.left()-7);
        rect.setRight(rect.right()+7);
        rect.setTop(rect.top()-7);
        rect.setBottom(rect.bottom()+7);


    return rect;
}


// ------- Widget

IE_Tool_LineInfoWidget::IE_Tool_LineInfoWidget(_global_ie *gi, QWidget *parent,
                                               IELine::LineSettings settings):  QWidget(parent),
                                                                        p_ie_global_data(gi),
                                                                        lineSettings(settings)


{
    length = new QLabel(this);
    length->hide();
    penWidth = new QLabel(this);
    penWidth->hide();
    penColor = new QColor();
    brushColor = new QColor();
    lengthTip = new QCheckBox("Длина линии");
    lengthTip->setChecked(false);
    lengthTip->hide();
    widthTip = new QCheckBox("Ширина линии");
    widthTip->setChecked(false);
    widthTip->hide();
}

     IE_Tool_LineInfoWidget::~IE_Tool_LineInfoWidget()
{
    delete length;
    delete penWidth;
    delete penColor;
    delete brushColor;
}

void IE_Tool_LineInfoWidget::init()
{


    QVBoxLayout *pVertBoxLayout = new QVBoxLayout(this);
    length->setNum(0);
    length->show();
    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(this);

    pHorBoxLayout->addWidget(new QLabel("Длина линии: ", this));
    pHorBoxLayout->addWidget(length);
    pVertBoxLayout->addItem(pHorBoxLayout);

    if(lineSettings & IELine::LineSettings::PenColor)
    {
        pHorBoxLayout = new QHBoxLayout(this);
        pHorBoxLayout->addWidget(new QLabel("Цвет обводки:", this));
        pHorBoxLayout->addStretch(1);


        QPushButton *pPButton = new QPushButton(this);


        QPalette palette = pPButton->palette();
        pPButton->setAutoFillBackground(true);
        pPButton->setFlat(true);
        pPButton->setMaximumWidth(25);
        pPButton->setMaximumHeight(25);
        palette.setColor(pPButton->backgroundRole(), *penColor);
        palette.setColor(pPButton->foregroundRole(), *penColor);
        pPButton->setPalette(palette);
        pHorBoxLayout->addWidget(pPButton);
        pHorBoxLayout->addStretch(2);
        pVertBoxLayout->addItem(pHorBoxLayout);


        connect(pPButton, &QPushButton::released, [this,pPButton](){
            QColor color = QColorDialog::getColor(*penColor);
            if(color.isValid())
            {
                penColor = &color;
                QPalette palette = pPButton->palette();
                emit this->penColorWasChanged(color);
                palette.setColor(pPButton->backgroundRole(), *penColor);
                palette.setColor(pPButton->foregroundRole(), *penColor);
                pPButton->setPalette(palette);
                pPButton->update();
            }
        });
    }

    if(lineSettings & IELine::LineSettings::PenWidth)
    {
        penWidth->show();
        QSlider *pWidthSlider = new QSlider(Qt::Horizontal, this);
        pWidthSlider->setRange(1,100);
        pWidthSlider->setValue(1);

        connect(pWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(changePenWidth(int)));
        connect(pWidthSlider, &QSlider::valueChanged,[this](int width){emit this->penWidthWasChanged(width);});
        connect(this, SIGNAL(penWidthChanged_INSIDE(int)), pWidthSlider, SLOT(setValue(int)));

        pHorBoxLayout = new QHBoxLayout(this);
        pHorBoxLayout->addWidget(pWidthSlider);
        pHorBoxLayout->addWidget(penWidth);

        pVertBoxLayout->addItem(pHorBoxLayout);
    }

    if(lineSettings & IELine::LineSettings::WidthTip)
    {
        widthTip->show();
        connect(widthTip, &QCheckBox::stateChanged, [this](int value){emit this->widthTipChecnged(value);});
        pVertBoxLayout->addWidget(widthTip);
    }

    if(lineSettings & IELine::LineSettings::LengthTip)
    {
        lengthTip->show();
        connect(lengthTip, &QCheckBox::stateChanged, [this](int value){emit this->lengthTipChecnged(value);});
        pVertBoxLayout->addWidget(lengthTip);
    }

    pVertBoxLayout->addStretch(1);
    this->setLayout(pVertBoxLayout);


}

void IE_Tool_LineInfoWidget::changeLenght(qreal length)
{
    this->length->setText(QString().number(p_ie_global_data->convertF(length)) +
                          " "+
                          UnitTypeTitle[p_ie_global_data->getUnitType()]);
}
void IE_Tool_LineInfoWidget::changePenWidth(int width)
{
    this->penWidth->setText(QString("%1 %2").
                            arg(width/p_ie_global_data->getMeasureIndex()).
                            arg(UnitTypeTitle[p_ie_global_data->getUnitType()]));
    emit penWidthChanged_INSIDE(width);
}
void IE_Tool_LineInfoWidget::changePenColor(QColor color)
{
    penColor->setRgb(color.red(), color.green(), color.blue());
}
void IE_Tool_LineInfoWidget::changeBrushColor(QColor color)
{
    brushColor->setRgb(color.red(), color.green(), color.blue());


}
void IE_Tool_LineInfoWidget::changeLengthTip(bool value)
{
    lengthTip->setChecked(value);
}
void IE_Tool_LineInfoWidget::changeWidthTip(bool value)
{
    widthTip->setChecked(value);
}
