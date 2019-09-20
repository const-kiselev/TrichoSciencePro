#ifndef IE_HEADER_H
#define IE_HEADER_H

#include <QString>
#include <QLabel>
#include <QObject>
#include "_global_ie.h"
#include "../_header_tsp.h"

enum class ToolType {
    NoneTool, Zoom,
    checkableTool = (1<< 6), // 0b01000000
        MainImage, // 0b01000001
        Image, // 0b01000010
        ComputingArea,
        Marker = checkableTool|(1<<5), // 0b01100000
            Marker_Colored,
            Marker_FollicularUnit,
            Marker_PeripilarSign,
            Marker_RootType,

            PeripilarSign_SpikyHair,
            PeripilarSign_ExclamationHair,
            PeripilarSign_BrokenHair,
            PeripilarSign_CadaverizedHair,
            PeripilarSign_YellowDot,
            PeripilarSign_RedDot,
            PeripilarSign_WhiteDot,
            PeripilarSign_Hyperpigmentation,

            RootType_Anagen,
            RootType_DysplasticAnagen,
            RootType_BrokenAnagen,
            RootType_AnagenWithPapilla,
            RootType_Telogen,
            RootType_Catagen,
            RootType_Dystrophic,
        SimpleLine/*=checkableTool|(1<<4)*/, // 0b01010000
            SmartLine,
            Ruler,
            DensityAndDiameter,
};


static QString getToolTitle(ToolType _toolType)
{
    switch(_toolType)
    {
    case ToolType::NoneTool: { return QString("NoneTool");}
    case ToolType::Zoom: { return QString("Zoom");}
    case ToolType::checkableTool : { return QString("checkableTool ");}
    case ToolType::MainImage: { return QString("MainImage");}
    case ToolType::Image: { return QString("Image");}
    case ToolType::ComputingArea: { return QString("ComputingArea");}
    case ToolType::Marker: { return QString("Marker");}
    case ToolType::Marker_Colored: { return QString("Marker_Colored");}
    case ToolType::Marker_FollicularUnit: { return QString("Marker_FollicularUnit");}
    case ToolType::Marker_PeripilarSign: { return QString("Marker_PeripilarSign");}
    case ToolType::Marker_RootType: { return QString("Marker_RootType");}

    case ToolType::PeripilarSign_SpikyHair: { return QString("PeripilarSign_SpikyHair");}
    case ToolType::PeripilarSign_ExclamationHair: { return QString("PeripilarSign_ExclamationHair");}
    case ToolType::PeripilarSign_BrokenHair: { return QString("PeripilarSign_BrokenHair");}
    case ToolType::PeripilarSign_CadaverizedHair: { return QString("PeripilarSign_CadaverizedHair");}
    case ToolType::PeripilarSign_YellowDot: { return QString("PeripilarSign_YellowDot");}
    case ToolType::PeripilarSign_RedDot: { return QString("PeripilarSign_RedDot");}
    case ToolType::PeripilarSign_WhiteDot: { return QString("PeripilarSign_WhiteDot");}
    case ToolType::PeripilarSign_Hyperpigmentation: { return QString("PeripilarSign_Hyperpigmentation");}

    case ToolType::RootType_Anagen: { return QString("RootType_Anagen");}
    case ToolType::RootType_DysplasticAnagen: { return QString("RootType_DysplasticAnagen");}
    case ToolType::RootType_BrokenAnagen: { return QString("RootType_BrokenAnagen");}
    case ToolType::RootType_AnagenWithPapilla: { return QString("RootType_AnagenWithPapilla");}
    case ToolType::RootType_Telogen: { return QString("RootType_Telogen");}
    case ToolType::RootType_Catagen: { return QString("RootType_Catagen");}
    case ToolType::RootType_Dystrophic: { return QString("RootType_Dystrophic");}
    case ToolType::SimpleLine: { return QString("SimpleLine");}
    case ToolType::SmartLine: { return QString("SmartLine");}
    case ToolType::Ruler: { return QString("Ruler");}
    case ToolType::DensityAndDiameter: { return QString("DensityAndDiameter");}
    }
}

enum class ToolSet {AllTools, CallibrationToolSet};

enum class ComputeType {ModelSquare,
                        SimpleLineCount,
                        DensityDiameterCount,
                        HairCount,
                        TerminalHairCount,
                        WellusCount};
enum class MarkerType{
    None,
    Colored,
    PeripilarSign_SpikyHair,
    PeripilarSign_ExclamationHair,
    PeripilarSign_BrokenHair,
    PeripilarSign_CadaverizedHair,
    PeripilarSign_YellowDot,
    PeripilarSign_RedDot,
    PeripilarSign_WhiteDot,
    RootType_Anagen,
    RootType_DysplasticAnagen,
    RootType_BrokenAnagen,
    RootType_AnagenWithPapilla,
    RootType_Telogen,
    RootType_Catagen,
    RootType_Dystrophic
};
enum class PeripilarSign{
    SpikyHair,
    ExclamationHair,
    BrokenHair,
    CadaverizedHair,
    YellowВot,
    RedDot,
    WhiteDot
};


enum class RootType {
    Anagen,
    DysplasticAnagen,
    BrokenAnagen,
    AnagenWithPapilla,
    Telogen,
    Catagen,
    Dystrophic
};
enum class SimpleLineType {Simple, Ruler};


// необходимо переименовать в layers!!!!!
struct ToolSettingDataStruct
{
        QString title;
        QString stringData;
        void * pData;
        enum class DataEditorType{None,
                                  ColorPalette,
                                  LineEditor,
                                  NumberEditor,
                                  NumberFloatEditor,
                                  FilePath
                           } editorType;
        enum class SettingName {
            PenColor,
            PenWidth,
            BrushColor,
            Length
        } nameOfSetting;
        ToolSettingDataStruct()
        {
            title = "";
            stringData = "";
            editorType = DataEditorType::None;
            pData = nullptr;
        }
        ToolSettingDataStruct(QString title_, QString stringData_,
                              DataEditorType inEditorType, void * pData_ = nullptr)
        {
            title = title_;
            stringData = stringData_;
            editorType = inEditorType;
            pData = pData_;
        }
};

struct ModelPatientData
{
    ModelPatientData() {}
    QString patientFullName;
    int patientID;

};


#endif // IE_HEADER_H
