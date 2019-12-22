#include "ie_toolMarker.h"

IE_Tool_Marker::IE_Tool_Marker( QObject* parent,
                                ToolType tt):QObject(parent),
                                                            QGraphicsItem (nullptr),
                                                            ie_tool(),
                                                            m_toolType(tt)
{

}

QRectF IE_Tool_Marker::boundingRect() const
{
    return QRectF(0, 0, 10, 10);
}

void IE_Tool_Marker::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{

    QPen locPen = painter->pen();
    switch (m_toolType)
    {
    case ToolType::HairShaft_UprightRegrowingHair:
    {
        locPen.setColor(Qt::blue);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawArrow(painter);
        break;
    }
    case ToolType::HairShaft_ExclamationMark:
    {
        locPen.setColor(Qt::black);
        locPen.setWidth(4);
        painter->setPen(locPen);
        painter->drawLine(0,0, 0, -4);
        painter->drawPoint(0,7);
        break;
    }
//    case ToolType::PeripilarSign_BrokenHair:
//    {
//        QColor color(0, 0, 0);
//        locPen.setColor(color);
//        locPen.setWidth(2);
//        painter->setPen(locPen);
//        painter->drawLine(0,0, -8, -8);
//        painter->drawLine(0,0, -8, 8);
//        locPen.setWidth(6);
//        painter->drawPoint(-8,8);
//        break;
//    }
//    case ToolType::PeripilarSign_CadaverizedHair:
//    {
//        QColor color(224, 128, 64);
//        locPen.setColor(color);
//        locPen.setWidth(11);
//        painter->setPen(locPen);
//        painter->drawEllipse(-6, -4, 12, 8);
//        color.setRgb(0,0,0);
//        locPen.setColor(color);
//        locPen.setWidth(2);
//        painter->setPen(locPen);
//        painter->drawPoint(0,0);
//        break;
//    }
    case ToolType::HairFollicleOpening_YellowDot:
    {
        locPen.setColor(Qt::yellow);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawStar(painter);
        break;
    }
    case ToolType::HairFollicleOpening_RedDot:
    {
        locPen.setColor(Qt::red);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawStar(painter);
        break;
    }
    case ToolType::HairFollicleOpening_WhiteDot:
    {
        locPen.setColor(Qt::white);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawStar(painter);
        break;
    }
    case ToolType::HairFollicleOpening_BlackDot:
    {
        locPen.setColor(Qt::black);
        locPen.setWidth(2);
        painter->setPen(locPen);
        drawStar(painter);
        break;
    }
    case ToolType::HairRoot_Anagen:
    {
        QColor color(0, 115,74);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
//    case ToolType::RootType_DysplasticAnagen:
//    {
//        QColor color(0, 255,0);
//        locPen.setColor(color);
//        locPen.setWidth(7);
//        painter->setPen(locPen);
//        painter->drawPoint(0,0);
//        break;
//    }
    case ToolType::HairRoot_Broken:
    {
        QColor color(0,189,247);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
//    case ToolType::RootType_AnagenWithPapilla:
//    {
//        QColor color(255,0,0);
//        locPen.setColor(color);
//        locPen.setWidth(7);
//        painter->setPen(locPen);
//        painter->drawPoint(0,0);
//        break;
//    }
    case ToolType::HairRoot_Telogen:
    {
        QColor color(0, 0, 255);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    case ToolType::HairRoot_Catagen:
    {
        QColor color(255, 255, 0);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    case ToolType::HairRoot_Dystrophic:
    {
        QColor color(99, 58, 16);
        locPen.setColor(color);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }

    default:
    {
        QColor color(99, 58, 16);
        locPen.setColor(Qt::magenta);
        locPen.setWidth(7);
        painter->setPen(locPen);
        painter->drawPoint(0,0);
        break;
    }
    }
    widget->update();
}

void IE_Tool_Marker::mouseFirstPress(QPointF point)
{
    setPos(point);
}

int IE_Tool_Marker::mouseMove(QPointF point)
{

}

int IE_Tool_Marker::read(const QJsonObject &json)
{
    m_toolType = convertToolTitleToToolType(json["typeTitle"].toString());
    return 0;
}

int IE_Tool_Marker::write(QJsonObject &json) const
{
    json["typeTitle"] = getToolTitle(m_toolType);
    return 0;
}

QList<QAction *> IE_Tool_Marker::getListOfAction(ToolType tt)
{
    QList<QAction *> actionList;
    QAction * pAct;
    switch (tt)
    {

    case ToolType::InterfollicularRegion:
    {
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_FineScaling"), "Незначительное шелушение");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_FineScaling ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_YellowishScaling"), "Желтоватое шелушение");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_YellowishScaling ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_WhiteScaling"), "Белое шелушение");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_WhiteScaling ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_LamellarScaling"), "Пластинчатое шелушение");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_LamellarScaling ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_PerifollicularScaling"), "Перифолликулярное шелушение");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_PerifollicularScaling ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_WhiteArea"), "Белая зона");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_WhiteArea ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_PinkArea"), "Розовая зона");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_PinkArea ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_RedArea"), "Красная зона");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_RedArea ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_MilkyRedArea"), "Молочно-красная зона");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_MilkyRedArea ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_Pustule"), "Пустула");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_Pustule ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/InterfollicularRegion_Exudate"), "Экссудат");
                pAct->setData( getToolTitle( ToolType::InterfollicularRegion_Exudate ) );
                actionList  << pAct;
                break;
    }
    case  ToolType::HairFollicleOpening:
    {
        pAct = new QAction(QPixmap("://toolIcons/HairFollicleOpening_PeripilarSigns"), "Перипилярная пигментация");
                pAct->setData( getToolTitle( ToolType::HairFollicleOpening_PeripilarSigns ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairFollicleOpening_YellowDot"), "Желтая точка");
                pAct->setData( getToolTitle( ToolType::HairFollicleOpening_YellowDot ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairFollicleOpening_BlackDot"), "Черная точка");
                pAct->setData( getToolTitle( ToolType::HairFollicleOpening_BlackDot ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairFollicleOpening_WhiteDot"), "Белая точка");
                pAct->setData( getToolTitle( ToolType::HairFollicleOpening_WhiteDot ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairFollicleOpening_RedDot"), "Красная точка");
                pAct->setData( getToolTitle( ToolType::HairFollicleOpening_RedDot ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairFollicleOpening_PeripilarCast"), "Перипилярная муфта");
                pAct->setData( getToolTitle( ToolType::HairFollicleOpening_PeripilarCast ) );
                actionList  << pAct;
                break;
    }
    case  ToolType::BloodVessel:
    {
        pAct = new QAction(QPixmap("://toolIcons/BloodVessel_ThickArborizingVessel"), "Толстый древовидный");
               pAct->setData( getToolTitle( ToolType::BloodVessel_ThickArborizingVessel ) );
               actionList  << pAct;
       pAct = new QAction(QPixmap("://toolIcons/BloodVessel_ThinArborizingVessel"), "Тонкий древовидный");
               pAct->setData( getToolTitle( ToolType::BloodVessel_ThinArborizingVessel ) );
               actionList  << pAct;
       pAct = new QAction(QPixmap("://toolIcons/BloodVessel_GlomerularAndTwisted"), "Гломерулярный и скрученный");
               pAct->setData( getToolTitle( ToolType::BloodVessel_GlomerularAndTwisted ) );
               actionList  << pAct;
       pAct = new QAction(QPixmap("://toolIcons/BloodVessel_CommaHair"), "Волос в виде запятой");
               pAct->setData( getToolTitle( ToolType::BloodVessel_CommaHair ) );
               actionList  << pAct;
               break;
    }
    case  ToolType::HairShaft:
    {
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Healthy"), "Здоровый");
                pAct->setData( getToolTitle( ToolType::HairShaft_Healthy ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Intermediate"), "Промежуточный");
                pAct->setData( getToolTitle( ToolType::HairShaft_Intermediate ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Vellus"), "Веллус");
                pAct->setData( getToolTitle( ToolType::HairShaft_Vellus ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_UprightRegrowingHair"), "Юный (остроконечный)");
                pAct->setData( getToolTitle( ToolType::HairShaft_UprightRegrowingHair ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_ExclamationMark"), "Восклицательный знак");
                pAct->setData( getToolTitle( ToolType::HairShaft_ExclamationMark ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Cone"), "Конические");
                pAct->setData( getToolTitle( ToolType::HairShaft_Cone ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Trichoptilosis"), "Трихоптилоз");
                pAct->setData( getToolTitle( ToolType::HairShaft_Trichoptilosis ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Trichoclasia"), "Трихоклазия");
                pAct->setData( getToolTitle( ToolType::HairShaft_Trichoclasia ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Trichoshisis"), "Трихишизис");
                pAct->setData( getToolTitle( ToolType::HairShaft_Trichoshisis ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Trichorexis"), "Трихорексис");
                pAct->setData( getToolTitle( ToolType::HairShaft_Trichorexis ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_BrokenHairsAtDifferentLength"), "Обломанные на разной длине");
                pAct->setData( getToolTitle( ToolType::HairShaft_BrokenHairsAtDifferentLength ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_TuftedHairs"), "Пучкообразный рост волос");
                pAct->setData( getToolTitle( ToolType::HairShaft_TuftedHairs ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_FlameAndBroom"), "Пламевидные и метельчатые");
                pAct->setData( getToolTitle( ToolType::HairShaft_FlameAndBroom ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_ZigzagAndIntermittent"), "Зигзагообразные и прерывистые");
                pAct->setData( getToolTitle( ToolType::HairShaft_ZigzagAndIntermittent ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Twisted"), "Перекрученный");
                pAct->setData( getToolTitle( ToolType::HairShaft_Twisted ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_PiliAnnulati"), "Кольчатый");
                pAct->setData( getToolTitle( ToolType::HairShaft_PiliAnnulati ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Moniletrix"), "Монилетрикс");
                pAct->setData( getToolTitle( ToolType::HairShaft_Moniletrix ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_Trichonodosis"), "Трихонодоз");
                pAct->setData( getToolTitle( ToolType::HairShaft_Trichonodosis ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairShaft_BambooHair"), "Бамбуковидный");
                pAct->setData( getToolTitle( ToolType::HairShaft_BambooHair ) );
                actionList  << pAct;
                break;
    }
    case  ToolType::HairRoot:
    {
        pAct = new QAction(QPixmap("://toolIcons/HairRoot"), "Корень волоса");
                pAct->setData( getToolTitle( ToolType::HairRoot ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairRoot_Anagen"), "Анагеновый");
                pAct->setData( getToolTitle( ToolType::HairRoot_Anagen ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairRoot_Catagen"), "Катагеновый");
                pAct->setData( getToolTitle( ToolType::HairRoot_Catagen ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairRoot_Telogen"), "Телогеновый");
                pAct->setData( getToolTitle( ToolType::HairRoot_Telogen ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairRoot_Dystrophic"), "Дистрофичный");
                pAct->setData( getToolTitle( ToolType::HairRoot_Dystrophic ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairRoot_Broken"), "Обломанный");
                pAct->setData( getToolTitle( ToolType::HairRoot_Broken ) );
                actionList  << pAct;
        pAct = new QAction(QPixmap("://toolIcons/HairRoot_Seborrhea"), "При себорее");
                pAct->setData( getToolTitle( ToolType::HairRoot_Seborrhea ) );
                actionList  << pAct;
                break;
    }

    }
    return actionList;
}

void IE_Tool_Marker::drawStar(QPainter *painter)
{
    int selectionOffset = 6;
    qreal radAngle = 60 * M_PI / 180;
    qreal dx = selectionOffset * sin(radAngle);
    qreal dy = selectionOffset * cos(radAngle);
    painter->drawLine(0,0, 0, -selectionOffset);
    painter->drawLine(0,0, dx, -dy);
    painter->drawLine(0,0, -dx, -dy);
    radAngle = 45 * M_PI / 180;
    dx = selectionOffset * sin(radAngle);
    dy = selectionOffset * cos(radAngle);
    painter->drawLine(0,0, -dx, dy);
    painter->drawLine(0,0, dx, dy);
}

void IE_Tool_Marker::drawArrow(QPainter *painter)
{
    int selectionOffset = 15;
    qreal radAngle = 45 * M_PI / 180;
    qreal dx1,dx = selectionOffset * sin(radAngle);
    qreal dy1,dy = selectionOffset * cos(radAngle);
    painter->drawLine(-dx, dy, dx, -dy);

    radAngle = 15 * M_PI / 180;
    selectionOffset = 7;
    dx1 = selectionOffset * sin(radAngle);
    dy1 = selectionOffset * cos(radAngle);

    painter->drawLine(dx, -dy, dx-dx1, -dy+dy1);

    radAngle = 75 * M_PI / 180;
    dx1 = selectionOffset * sin(radAngle);
    dy1 = selectionOffset * cos(radAngle);
    painter->drawLine(dx, -dy, dx-dx1, -dy+dy1);
}




