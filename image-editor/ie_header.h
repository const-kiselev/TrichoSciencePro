#ifndef IE_HEADER_H
#define IE_HEADER_H

#include <QString>
#include <QLabel>
#include <QObject>
#include <QSet>
#include <QAction>
#include "_global_ie.h"
#include "../_header_tsp.h"

enum class ToolType {
    NoneTool, Zoom,
    checkableTool = (1<< 6),            // 0b01000000
        MainImage,                      // 0b01000001
        Image,                          // 0b01000010
        ComputingArea,
        Marker = checkableTool|(1<<5),  // 0b01100000
            Marker_Colored,
            Marker_FollicularUnit,
            Marker_PeripilarSign,
            Marker_RootType,
    SimpleLine/*=checkableTool|(1<<4)*/, // 0b01010000
        SmartLine,
        Ruler,
        DensityAndDiameter,

    InterfollicularRegion,
    InterfollicularRegion_FineScaling,
    InterfollicularRegion_YellowishScaling,
    InterfollicularRegion_WhiteScaling,
    InterfollicularRegion_LamellarScaling,
    InterfollicularRegion_PerifollicularScaling,
    InterfollicularRegion_WhiteArea,
    InterfollicularRegion_PinkArea,
    InterfollicularRegion_RedArea,
    InterfollicularRegion_MilkyRedArea,
    InterfollicularRegion_Pustule,
    InterfollicularRegion_Exudate,


    HairFollicleOpening,
    HairFollicleOpening_PeripilarSigns,
    HairFollicleOpening_YellowDot,
    HairFollicleOpening_BlackDot,
    HairFollicleOpening_WhiteDot,
    HairFollicleOpening_RedDot,
    HairFollicleOpening_PeripilarCast,

    BloodVessel,
    BloodVessel_ThickArborizingVessel,
    BloodVessel_ThinArborizingVessel,
    BloodVessel_GlomerularAndTwisted,
    BloodVessel_CommaHair,


    HairShaft,
    HairShaft_Healthy,
    HairShaft_Intermediate,
    HairShaft_Vellus,
    HairShaft_UprightRegrowingHair,
    HairShaft_ExclamationMark,
    HairShaft_Cone,
    HairShaft_Trichoptilosis,
    HairShaft_Trichoclasia,
    HairShaft_Trichoshisis,
    HairShaft_Trichorexis,
    HairShaft_BrokenHairsAtDifferentLength,
    HairShaft_TuftedHairs,
    HairShaft_FlameAndBroom,
    HairShaft_ZigzagAndIntermittent,
    HairShaft_Twisted,
    HairShaft_PiliAnnulati,
    HairShaft_Moniletrix,
    HairShaft_Trichonodosis,
    HairShaft_BambooHair,


    HairRoot,
    HairRoot_Anagen,
    HairRoot_Catagen,
    HairRoot_Telogen,
    HairRoot_Dystrophic,
    HairRoot_Broken,
    HairRoot_Seborrhea
};


static QString getToolTitle(ToolType _toolType)
{
    switch(_toolType)
    {
    case ToolType::NoneTool: { return QString("NoneTool");}
    case ToolType::Zoom: { return QString("Zoom");}
    case ToolType::checkableTool: { return QString("checkableTool");}
    case ToolType::MainImage: { return QString("MainImage");}
    case ToolType::Image: { return QString("Image");}
    case ToolType::ComputingArea: { return QString("ComputingArea");}
    case ToolType::Marker: { return QString("Marker");}
    case ToolType::Marker_Colored: { return QString("Marker_Colored");}
    case ToolType::Marker_FollicularUnit: { return QString("Marker_FollicularUnit");}
    case ToolType::Marker_PeripilarSign: { return QString("Marker_PeripilarSign");}
    case ToolType::Marker_RootType: { return QString("Marker_RootType");}

    case ToolType::InterfollicularRegion: { return QString("InterfollicularRegion");}
    case ToolType::InterfollicularRegion_FineScaling: { return QString("InterfollicularRegion_FineScaling");}
    case ToolType::InterfollicularRegion_YellowishScaling: { return QString("InterfollicularRegion_YellowishScaling");}
    case ToolType::InterfollicularRegion_WhiteScaling: { return QString("InterfollicularRegion_WhiteScaling");}
    case ToolType::InterfollicularRegion_LamellarScaling: { return QString("InterfollicularRegion_LamellarScaling");}
    case ToolType::InterfollicularRegion_PerifollicularScaling: { return QString("InterfollicularRegion_PerifollicularScaling");}
    case ToolType::InterfollicularRegion_WhiteArea: { return QString("InterfollicularRegion_WhiteArea");}
    case ToolType::InterfollicularRegion_PinkArea: { return QString("InterfollicularRegion_PinkArea");}
    case ToolType::InterfollicularRegion_RedArea: { return QString("InterfollicularRegion_RedArea");}
    case ToolType::InterfollicularRegion_MilkyRedArea: { return QString("InterfollicularRegion_MilkyRedArea");}
    case ToolType::InterfollicularRegion_Pustule: { return QString("InterfollicularRegion_Pustule");}
    case ToolType::InterfollicularRegion_Exudate: { return QString("InterfollicularRegion_Exudate");}


    case ToolType::HairFollicleOpening: { return QString("HairFollicleOpening");}
    case ToolType::HairFollicleOpening_PeripilarSigns: { return QString("HairFollicleOpening_PeripilarSigns");}
    case ToolType::HairFollicleOpening_YellowDot: { return QString("HairFollicleOpening_YellowDot");}
    case ToolType::HairFollicleOpening_BlackDot: { return QString("HairFollicleOpening_BlackDot");}
    case ToolType::HairFollicleOpening_WhiteDot: { return QString("HairFollicleOpening_WhiteDot");}
    case ToolType::HairFollicleOpening_RedDot: { return QString("HairFollicleOpening_RedDot");}
    case ToolType::HairFollicleOpening_PeripilarCast: { return QString("HairFollicleOpening_PeripilarCast");}

    case ToolType::BloodVessel: { return QString("BloodVessel");}
    case ToolType::BloodVessel_ThickArborizingVessel: { return QString("BloodVessel_ThickArborizingVessel");}
    case ToolType::BloodVessel_ThinArborizingVessel: { return QString("BloodVessel_ThinArborizingVessel");}
    case ToolType::BloodVessel_GlomerularAndTwisted: { return QString("BloodVessel_GlomerularAndTwisted");}
    case ToolType::BloodVessel_CommaHair: { return QString("BloodVessel_CommaHair");}


    case ToolType::HairShaft: { return QString("HairShaft");}
    case ToolType::HairShaft_Healthy: { return QString("HairShaft_Healthy");}
    case ToolType::HairShaft_Intermediate: { return QString("HairShaft_Intermediate");}
    case ToolType::HairShaft_Vellus: { return QString("HairShaft_Vellus");}
    case ToolType::HairShaft_UprightRegrowingHair: { return QString("HairShaft_UprightRegrowingHair");}
    case ToolType::HairShaft_ExclamationMark: { return QString("HairShaft_ExclamationMark");}
    case ToolType::HairShaft_Cone: { return QString("HairShaft_Cone");}
    case ToolType::HairShaft_Trichoptilosis: { return QString("HairShaft_Trichoptilosis");}
    case ToolType::HairShaft_Trichoclasia: { return QString("HairShaft_Trichoclasia");}
    case ToolType::HairShaft_Trichoshisis: { return QString("HairShaft_Trichoshisis");}
    case ToolType::HairShaft_Trichorexis: { return QString("HairShaft_Trichorexis");}
    case ToolType::HairShaft_BrokenHairsAtDifferentLength: { return QString("HairShaft_BrokenHairsAtDifferentLength");}
    case ToolType::HairShaft_TuftedHairs: { return QString("HairShaft_TuftedHairs");}
    case ToolType::HairShaft_FlameAndBroom: { return QString("HairShaft_FlameAndBroom");}
    case ToolType::HairShaft_ZigzagAndIntermittent: { return QString("HairShaft_ZigzagAndIntermittent");}
    case ToolType::HairShaft_Twisted: { return QString("HairShaft_Twisted");}
    case ToolType::HairShaft_PiliAnnulati: { return QString("HairShaft_PiliAnnulati");}
    case ToolType::HairShaft_Moniletrix: { return QString("HairShaft_Moniletrix");}
    case ToolType::HairShaft_Trichonodosis: { return QString("HairShaft_Trichonodosis");}
    case ToolType::HairShaft_BambooHair: { return QString("HairShaft_BambooHair");}


    case ToolType::HairRoot: { return QString("HairRoot");}
    case ToolType::HairRoot_Anagen: { return QString("HairRoot_Anagen");}
    case ToolType::HairRoot_Catagen: { return QString("HairRoot_Catagen");}
    case ToolType::HairRoot_Telogen: { return QString("HairRoot_Telogen");}
    case ToolType::HairRoot_Dystrophic: { return QString("HairRoot_Dystrophic");}
    case ToolType::HairRoot_Broken: { return QString("HairRoot_Broken");}
    case ToolType::HairRoot_Seborrhea: { return QString("HairRoot_Seborrhea");}

    case ToolType::SimpleLine: { return QString("SimpleLine");}
    case ToolType::SmartLine: { return QString("SmartLine");}
    case ToolType::Ruler: { return QString("Ruler");}
    case ToolType::DensityAndDiameter: { return QString("DensityAndDiameter");}
    }
}

static ToolType convertToolTitleToToolType(QString toolTitle)
{
    if(toolTitle == "NoneTool") return ToolType::NoneTool;
    if(toolTitle == "Zoom") return ToolType::Zoom;
    if(toolTitle == "checkableTool") return ToolType::checkableTool;
    if(toolTitle == "MainImage") return ToolType::MainImage;
    if(toolTitle == "Image") return ToolType::Image;
    if(toolTitle == "ComputingArea") return ToolType::ComputingArea;
    if(toolTitle == "Marker") return ToolType::Marker;
    if(toolTitle == "Marker_Colored") return ToolType::Marker_Colored;
    if(toolTitle == "Marker_FollicularUnit") return ToolType::Marker_FollicularUnit;
    if(toolTitle == "Marker_PeripilarSign") return ToolType::Marker_PeripilarSign;
    if(toolTitle == "Marker_RootType") return ToolType::Marker_RootType;

    if(toolTitle == "InterfollicularRegion") return ToolType::InterfollicularRegion;
    if(toolTitle == "InterfollicularRegion_FineScaling") return ToolType::InterfollicularRegion_FineScaling;
    if(toolTitle == "InterfollicularRegion_YellowishScaling") return ToolType::InterfollicularRegion_YellowishScaling;
    if(toolTitle == "InterfollicularRegion_WhiteScaling") return ToolType::InterfollicularRegion_WhiteScaling;
    if(toolTitle == "InterfollicularRegion_LamellarScaling") return ToolType::InterfollicularRegion_LamellarScaling;
    if(toolTitle == "InterfollicularRegion_PerifollicularScaling") return ToolType::InterfollicularRegion_PerifollicularScaling;
    if(toolTitle == "InterfollicularRegion_WhiteArea") return ToolType::InterfollicularRegion_WhiteArea;
    if(toolTitle == "InterfollicularRegion_PinkArea") return ToolType::InterfollicularRegion_PinkArea;
    if(toolTitle == "InterfollicularRegion_RedArea") return ToolType::InterfollicularRegion_RedArea;
    if(toolTitle == "InterfollicularRegion_MilkyRedArea") return ToolType::InterfollicularRegion_MilkyRedArea;
    if(toolTitle == "InterfollicularRegion_Pustule") return ToolType::InterfollicularRegion_Pustule;
    if(toolTitle == "InterfollicularRegion_Exudate") return ToolType::InterfollicularRegion_Exudate;


    if(toolTitle == "HairFollicleOpening") return ToolType::HairFollicleOpening;
    if(toolTitle == "HairFollicleOpening_PeripilarSigns") return ToolType::HairFollicleOpening_PeripilarSigns;
    if(toolTitle == "HairFollicleOpening_YellowDot") return ToolType::HairFollicleOpening_YellowDot;
    if(toolTitle == "HairFollicleOpening_BlackDot") return ToolType::HairFollicleOpening_BlackDot;
    if(toolTitle == "HairFollicleOpening_WhiteDot") return ToolType::HairFollicleOpening_WhiteDot;
    if(toolTitle == "HairFollicleOpening_RedDot") return ToolType::HairFollicleOpening_RedDot;
    if(toolTitle == "HairFollicleOpening_PeripilarCast") return ToolType::HairFollicleOpening_PeripilarCast;

    if(toolTitle == "BloodVessel") return ToolType::BloodVessel;
    if(toolTitle == "BloodVessel_ThickArborizingVessel") return ToolType::BloodVessel_ThickArborizingVessel;
    if(toolTitle == "BloodVessel_ThinArborizingVessel") return ToolType::BloodVessel_ThinArborizingVessel;
    if(toolTitle == "BloodVessel_GlomerularAndTwisted") return ToolType::BloodVessel_GlomerularAndTwisted;
    if(toolTitle == "BloodVessel_CommaHair") return ToolType::BloodVessel_CommaHair;


    if(toolTitle == "HairShaft") return ToolType::HairShaft;
    if(toolTitle == "HairShaft_Healthy") return ToolType::HairShaft_Healthy;
    if(toolTitle == "HairShaft_Intermediate") return ToolType::HairShaft_Intermediate;
    if(toolTitle == "HairShaft_Vellus") return ToolType::HairShaft_Vellus;
    if(toolTitle == "HairShaft_UprightRegrowingHair") return ToolType::HairShaft_UprightRegrowingHair;
    if(toolTitle == "HairShaft_ExclamationMark") return ToolType::HairShaft_ExclamationMark;
    if(toolTitle == "HairShaft_Cone") return ToolType::HairShaft_Cone;
    if(toolTitle == "HairShaft_Trichoptilosis") return ToolType::HairShaft_Trichoptilosis;
    if(toolTitle == "HairShaft_Trichoclasia") return ToolType::HairShaft_Trichoclasia;
    if(toolTitle == "HairShaft_Trichoshisis") return ToolType::HairShaft_Trichoshisis;
    if(toolTitle == "HairShaft_Trichorexis") return ToolType::HairShaft_Trichorexis;
    if(toolTitle == "HairShaft_BrokenHairsAtDifferentLength") return ToolType::HairShaft_BrokenHairsAtDifferentLength;
    if(toolTitle == "HairShaft_TuftedHairs") return ToolType::HairShaft_TuftedHairs;
    if(toolTitle == "HairShaft_FlameAndBroom") return ToolType::HairShaft_FlameAndBroom;
    if(toolTitle == "HairShaft_ZigzagAndIntermittent") return ToolType::HairShaft_ZigzagAndIntermittent;
    if(toolTitle == "HairShaft_Twisted") return ToolType::HairShaft_Twisted;
    if(toolTitle == "HairShaft_PiliAnnulati") return ToolType::HairShaft_PiliAnnulati;
    if(toolTitle == "HairShaft_Moniletrix") return ToolType::HairShaft_Moniletrix;
    if(toolTitle == "HairShaft_Trichonodosis") return ToolType::HairShaft_Trichonodosis;
    if(toolTitle == "HairShaft_BambooHair") return ToolType::HairShaft_BambooHair;


    if(toolTitle == "HairRoot") return ToolType::HairRoot;
    if(toolTitle == "HairRoot_Anagen") return ToolType::HairRoot_Anagen;
    if(toolTitle == "HairRoot_Catagen") return ToolType::HairRoot_Catagen;
    if(toolTitle == "HairRoot_Telogen") return ToolType::HairRoot_Telogen;
    if(toolTitle == "HairRoot_Dystrophic") return ToolType::HairRoot_Dystrophic;
    if(toolTitle == "HairRoot_Broken") return ToolType::HairRoot_Broken;
    if(toolTitle == "HairRoot_Seborrhea") return ToolType::HairRoot_Seborrhea;

    if(toolTitle == "SimpleLine") return ToolType::SimpleLine;
    if(toolTitle == "SmartLine") return ToolType::SmartLine;
    if(toolTitle == "Ruler") return ToolType::Ruler;
    if(toolTitle == "DensityAndDiameter") return ToolType::DensityAndDiameter;
}

enum class ToolSet {
    AllTools,
    MeasureIndex,
    HairDencity,
    Simple,
    TrichoscopyPatterns
};



enum class SimpleLineType {Simple, Ruler};

struct IE_ToolStruct
{
    ToolType type;
    QAction * pAction;
};

struct ModelPatientData
{
    ModelPatientData() {}
    QString patientFullName;
    int patientID;

};

enum class IEM_type
{
    None,
    HairDencity,
    TrichoscopyPatterns,
    AssessmentOfScalp,
    AssessmentOfHairRoots,
    AssessmentOfHairRods,
    DermatoscopyOfNeoplasms,
    MeasureIndex

};

static QString getTSP_JSON_IEM_type(IEM_type ieType)
{
    switch(ieType)
    {
    case IEM_type::HairDencity:
        return QString("HairDencity");
    case IEM_type::TrichoscopyPatterns:
        return QString("TrichoscopyPatterns");
    case IEM_type::AssessmentOfScalp:
        return QString("AssessmentOfScalp");
    case IEM_type::AssessmentOfHairRoots:
        return QString("AssessmentOfHairRoots");
    case IEM_type::AssessmentOfHairRods:
        return QString("AssessmentOfHairRods");
    case IEM_type::DermatoscopyOfNeoplasms:
        return QString("DermatoscopyOfNeoplasms");
    }
}

static IEM_type getIEM_type(QString str)
{
    if(str == "HairDencity")
        return IEM_type::HairDencity;
    if(str == "TrichoscopyPatterns")
        return IEM_type::TrichoscopyPatterns;
    if(str == "AssessmentOfScalp")
        return IEM_type::AssessmentOfScalp;
    if(str == "AssessmentOfHairRoots")
        return IEM_type::AssessmentOfHairRoots;
    if(str == "AssessmentOfHairRods")
        return IEM_type::AssessmentOfHairRods;
    if(str == "DermatoscopyOfNeoplasms")
        return IEM_type::DermatoscopyOfNeoplasms;
}

static QString UI_getIEM_type_title(IEM_type ieType)
{
    switch(ieType)
    {
    case IEM_type::HairDencity:
        return QString("Плотность волос");
    case IEM_type::TrichoscopyPatterns:
        return QString("Трихоскопические паттерны");
    case IEM_type::AssessmentOfScalp:
        return QString("Оценка кожи головы");
    case IEM_type::AssessmentOfHairRoots:
        return QString("Оценка корней");
    case IEM_type::AssessmentOfHairRods:
        return QString("Оценка стержней волос");
    case IEM_type::DermatoscopyOfNeoplasms:
        return QString("Дерматоскопия новообразований");
    }
}


//static QSet<ToolType> getToolTypeGroupSet()
//{
//    QSet<ToolType> set;
//    set << ToolType::InterfollicularRegion
//        << ToolType::HairFollicleOpening
//        << ToolType::BloodVessel
//        << ToolType::HairShaft
//        << ToolType::HairRoot;
//    return set;
//}

struct ToolTypeStruct
{
    ToolType type;
    QAction * pAction;
};

enum class IE_ReportType
{
    Full,
    HairDensity,
    Trichoscopy
};


#endif // IE_HEADER_H
