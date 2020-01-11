#include "ie_compute.h"


IE_Compute::IE_Compute(_global_ie *pieg,
                       IE_FVConstList fieldOfViewListConstPtr,
                       QObject *parent
                       ) :  QObject(parent),
                            _p_ie_global_data(pieg),
                            m_fieldOfViewListConstPtr(fieldOfViewListConstPtr)
{

}

qreal IE_Compute::computeSquareInPixels() const
{

    qreal sq = 0;
    foreach(const IE_FieldOfView* pFV, m_fieldOfViewListConstPtr)
        sq += pFV->getAreaValue();
    return sq;
}

IE_Compute::HairDensityReportData IE_Compute::computeHairDensity()
{
    HairDensityReportData d;

    foreach(const IE_FieldOfView* pFV, m_fieldOfViewListConstPtr)
    {
        foreach(const IE_ModelLayer * pLayer, pFV->getConstPtrOfLayerList())
        {
            if(pLayer->getToolType() == ToolType::DensityAndDiameter)
            {
                d.hairQuantity++;
                qreal penWidth = dynamic_cast<IELine*>(pLayer->getToolPtr())->getPenWidth();
                if  (_p_ie_global_data->convertF(penWidth)
                        >= _p_ie_global_data->getThreshold_TW()
                    )
                    d.terminalQuantity++;
                else
                    d.vellusQuantity++;
            }
        }
    }



    d.areaValue = computeSquareInPixels();

    qreal squareInUnitedUnits = (d.areaValue /
                                       (_p_ie_global_data->getMeasureIndex()*
                                        _p_ie_global_data->getMeasureIndex()
                                        )
                                       );
    d.squareInCm2 = _p_ie_global_data->convertUnitedWithForamtF(
                squareInUnitedUnits,
                UnitType::cm
                );
    d.squareInCm2 = _p_ie_global_data->convertUnitedWithForamtF(
                d.squareInCm2,
                UnitType::cm
                );  // тк кв см


    d.totalHairQuantityInCm2 = round(d.hairQuantity/d.squareInCm2);
    d.percentTermin = d.terminalQuantity*1.0/d.hairQuantity*100.0;
    d.terminHairInCm2 = round(d.terminalQuantity/d.squareInCm2);
    d.percentVellus = d.vellusQuantity*1.0/d.hairQuantity*100.0;
    d.vellusHairInCm2 = round(d.vellusQuantity/d.squareInCm2);

    return d;
}

IE_Compute::HairDiameterReportData IE_Compute::computeHairDiameter()
{
    HairDiameterReportData d;

    foreach(const IE_FieldOfView* pFV, m_fieldOfViewListConstPtr)
    {
        foreach(const IE_ModelLayer * pLayer, pFV->getConstPtrOfLayerList())
        {
            if(pLayer->getToolType() == ToolType::DensityAndDiameter)
            {
                d.hairQuantity++;

                qreal penWidth = dynamic_cast<IELine*>(pLayer->getToolPtr())->getPenWidth();
                qreal convertedPenWidth = _p_ie_global_data->convertF(penWidth);
                d.diametersListInUm << _p_ie_global_data->convertUnitedWithForamtF(convertedPenWidth, UnitType::um);
                d.sumOfDiametersOfAllHair += convertedPenWidth;
                if ( convertedPenWidth
                        >= _p_ie_global_data->getThreshold_TW()
                    )
                {
                    d.terminalQuantity++;
                    d.sumOfDiametersOfTerminHair += convertedPenWidth;
                    if ( convertedPenWidth >= _p_ie_global_data->getThreshold_thinHair()
                            && convertedPenWidth < _p_ie_global_data->getThreshold_mediumHair()
                        )
                        d.mediumHairQ++;
                    else if(convertedPenWidth < _p_ie_global_data->getThreshold_thinHair())
                        d.thinHairQ++;
                }
                else
                    d.vellusQuantity++;

            }
        }
    }

    d.areaValue = computeSquareInPixels();
    d.squareInUnitedUnits = d.areaValue
            /   ( _p_ie_global_data->getMeasureIndex()
                    * _p_ie_global_data->getMeasureIndex()
                );

    d.midDiameterAll = d.sumOfDiametersOfAllHair/d.hairQuantity;
    d.midDiameterTermin = d.sumOfDiametersOfTerminHair/d.terminalQuantity;
    d.percentTermin = d.terminalQuantity*1./d.hairQuantity*100.;
    d.percentVellus = d.vellusQuantity*1./d.hairQuantity*100.;
    d.percentThinHair = d.thinHairQ*1./d.terminalQuantity*100.;
    d.percentMediumHair = d.mediumHairQ*1./d.terminalQuantity*100.;

    return d;
}


/// \todo доделать
IE_Compute::OutputData IE_Compute::compute(IE_Compute::ComputeType compType)
{
    OutputData outD;
    outD.computeType = compType;
    if( (compType&ComputeType::HairDensity) == ComputeType::HairDensity)
    {
        outD.density = computeHairDensity();
    }
    if( (compType&ComputeType::HairDiameter) == ComputeType::HairDiameter)
    {
        outD.diameter = computeHairDiameter();
    }
    return outD;
}

IE_Compute::OutputData IE_Compute::makeCompute(_global_ie * pieg,
                                               InputData id,
                                               QObject *parent
                                             )
{
    IE_Compute comp(pieg, id.fieldOfViewListConstIter, parent);
    return comp.compute(id.computeType);
}




