#include "ie_compute.h"

IE_Compute::IE_Compute(_global_ie *pieg,
                       IE_FieldOfView::PublicList fvList,
                       QObject *parent
                       ) :  QObject(parent),
                            m_fvList(fvList),
                            _p_ie_global_data(pieg)
{

}


IE_Compute::HairDensityReportData IE_Compute::computeHairDensity() const
{
    HairDensityReportData d;

    for (IE_FieldOfView::PublicList::const_iterator fvIter = m_fvList.begin();
         fvIter!=m_fvList.end();
         fvIter++
         )
    {

        IE_ModelLayer::PublicList layerList = fvIter.i->t()->getLayers();
        for(IE_ModelLayer::PublicList::const_iterator layerIter = layerList.begin();
            layerIter != layerList.end();
            layerIter++
            ){
            if( layerIter.i->t()->getToolType() == ToolType::DensityAndDiameter)
            {
                d.hairQuantity++;
                qreal penWidth = dynamic_cast<IE_Tool_Line*>(layerIter.i->t()->getToolPtr())->getPenWidth();

                if  (_p_ie_global_data->convertF(penWidth)
                        >= _p_ie_global_data->getThreshold_TW()
                    )
                    d.terminalQuantity++;
                else
                    d.vellusQuantity++;


            }
        }

       d.areaValue += fvIter.i->t()->getAreaValue();
    }



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

IE_Compute::HairDiameterReportData IE_Compute::computeHairDiameter() const
{
    HairDiameterReportData d;

    for (IE_FieldOfView::PublicList::const_iterator fvIter = m_fvList.begin();
         fvIter!=m_fvList.end();
         fvIter++
         )
    {

        IE_ModelLayer::PublicList layerList = fvIter.i->t()->getLayers();
        for(IE_ModelLayer::PublicList::const_iterator layerIter = layerList.begin();
            layerIter != layerList.end();
            layerIter++
            ){
            if( layerIter.i->t()->getToolType() == ToolType::DensityAndDiameter)
            {
                d.hairQuantity++;
                qreal penWidth = dynamic_cast<IE_Tool_Line*>(layerIter.i->t()->getToolPtr())->getPenWidth();
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

       d.areaValue += fvIter.i->t()->getAreaValue();
    }

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

// static
IE_Compute::OutputData IE_Compute::makeCompute(_global_ie *pieg, IE_Compute::InputData id, QObject *parent)
{
    IE_Compute comp(pieg, id.fvList, parent);
    return comp.compute(id.computeType);
}


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


