#include "ie_toolLine.h"

IELine::IELine(QObject* parent, ToolType tt, LineSettings settings):
                                        QObject(parent),
                                        QGraphicsLineItem (nullptr),
                                        ie_tool(),
                                        lineSettings(settings)
{
    setPen(QPen(Qt::red, 2));
    pen().setCapStyle(Qt::FlatCap);
    pen().setJoinStyle(Qt::BevelJoin);

    editMode = false;
    lineData.setP1(QPointF(-1,-1));
    lineData.setP2(QPointF(-1,-1));
    ignoreMove = false;

    pInfoWidget = new IE_Tool_LineInfoWidget(nullptr, settings);
    pInfoWidget->changeLenght(0);
    pInfoWidget->changePenColor(pen().color());
    pInfoWidget->changePenWidth(1);
//    setPToolInfoWidget(pInfoWidget);

    connect(pInfoWidget, &IE_Tool_LineInfoWidget::penWidthWasChanged,
                          [this](int width)
    {
        pen().setWidthF(width);
        this->updateSettings();

    });

    connect(pInfoWidget, &IE_Tool_LineInfoWidget::penColorWasChanged,
                              [this](QColor color)
        {
            pen().setColor(color);
            this->updateSettings();
        });

    connect(this, &IELine::lineChanged, [this]()
    {
        pInfoWidget->changeLenght(getLineLength());
        pInfoWidget->changePenWidth(pen().width());
    });
    pInfoWidget->init();

}

// ------- EVENTS
void IELine::mouseFirstPress(QPointF point)
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
int IELine::mouseMove(QPointF point)
{
    emit lineChanged();
    if(ignoreMove)
        return 1;
    lineData.setP2(point);
    setLine(lineData);
    return 0;
}
void IELine::wheelMode(QWheelEvent* pe)
{
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
            pen().setWidth(iWidth);
        }

//     else if (!nPixels.isNull())
//        {
//        if(iWidth+abs(nPixels.ry())/nPixels.ry()>1)
//        {
//            qDebug()<< iWidth+abs(nPixels.ry())/nPixels.ry();
//            pen.setWidth(iWidth+abs(nPixels.ry())/nPixels.ry());
//        }
//        }



    setLine(lineData);
    updateSettings();
}

void IELine::release(QPointF point)
{
    ignoreMove = false;
}

QLineF IELine::getLine() const
{
    return line();
}
qreal IELine::getLineLength()
{
    return line().length();
}

void IELine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsLineItem::paint( painter, option, widget );


    if(editMode)
    {
        QPen localPen(painter->pen());
        localPen.setWidth(1);
        localPen.setColor(Qt::red);

        painter->setPen(localPen);
        painter->setBrush(QBrush(Qt::white));

        painter->drawEllipse(line().p1() , 5, 5);
        painter->drawEllipse(line().p2() , 5, 5);
    }



}

void IELine::updateSettings()
{
    //setPen(pen);
    setLine(lineData);

    pInfoWidget->changeLenght(getLineLength());
    pInfoWidget->changePenColor(pen().color());
    pInfoWidget->changePenWidth(pen().width());
}
void IELine::penWidthChanged(qreal width)
{

}
bool IELine::activateEditMode()
{
    editMode = true;
    update();
    return editMode;
}
bool IELine::deactivateEditMode()
{
    editMode = false;
    update();
    return editMode;
}

void IELine::makeCompute()
{

}


QWidget *IELine::getWidgetPtr()
{return pInfoWidget;}

void IELine::read(const QJsonObject &json)
{
    pen().color() = QColor(json["line_penColor"].toString());
    pen().setWidthF(json["line_penWidth"].toDouble());
    pen().setStyle(static_cast<Qt::PenStyle>(json["typeTitle"].toInt()));
    line().setP1(QPointF(   json["line_point1_posX"].toDouble(),
                            json["line_point1_posY"].toDouble()));
    line().setP2(QPointF(   json["line_point2_posX"].toDouble(),
                            json["line_point2_posY"].toDouble()));
}

void IELine::write(QJsonObject &json) const
{
    json["typeTitle"] = getToolTitle(ToolType::SimpleLine);
    json["line_penColor"] = pen().color().name();
    json["line_penWidth"] = pen().widthF();
    json["line_penStyle"] = static_cast<int>(pen().style()) ;
    json["line_point1_posX"] = line().p1().x();
    json["line_point1_posY"] = line().p1().y();
    json["line_point2_posX"] = line().p2().x();
    json["line_point2_posY"] = line().p2().y();
}

QRectF IELine::boundingRect() const
{
    QRectF rect(QGraphicsLineItem::boundingRect());

        rect.setLeft(rect.left()-7);
        rect.setRight(rect.right()+7);
        rect.setTop(rect.top()-7);
        rect.setBottom(rect.bottom()+7);


    return rect;
}


// ------- Widget

IE_Tool_LineInfoWidget::IE_Tool_LineInfoWidget(QWidget *parent,
                                               LineSettings settings):QWidget(parent),
                                                                        lineSettings(settings)
{
    length = new QLabel(this);
    length->hide();
    penWidth = new QLabel(this);
    penWidth->hide();
    penColor = new QColor();
    brushColor = new QColor();
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

    if(lineSettings & LineSettings::PenColor)
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

    if(lineSettings & LineSettings::PenWidth)
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


    this->setLayout(pVertBoxLayout);


}

void IE_Tool_LineInfoWidget::changeLenght(qreal length)
{
    this->length->setText(QString().number(IE_GLOBAL_DATA.convertF(length)) +
                          " "+
                          UnitTypeTitle[IE_GLOBAL_DATA.getUnitType()]);
}
void IE_Tool_LineInfoWidget::changePenWidth(int width)
{
    this->penWidth->setText(QString("%1 %2").
                            arg(width/IE_GLOBAL_DATA.getMeasureIndex()).
                            arg(UnitTypeTitle[IE_GLOBAL_DATA.getUnitType()]));
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
