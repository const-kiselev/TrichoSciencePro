#include "ie_compute.h"

IE_Compute::IE_Compute(_global_ie *pieg,
                       QList<IE_ModelLayer *> *pll,
                       QObject *parent
                       ) :  QObject(parent),
                            _p_ie_global_data(pieg),
                            layersList(pll)
{

}

qreal IE_Compute::computeSquareInPixels() const
{
    qreal sq = 0;
    for (QList<IE_ModelLayer *>::iterator tmpIter = layersList->begin();
         tmpIter!=layersList->end();tmpIter++)
    {
        if( tmpIter.i->t()->getToolType() == ToolType::MainImage)
            sq += tmpIter.i->t()->boundingRect().width() * tmpIter.i->t()->boundingRect().height();
        else if( tmpIter.i->t()->getToolType() == ToolType::ComputingArea )
        {
            sq = tmpIter.i->t()->boundingRect().width() * tmpIter.i->t()->boundingRect().height();
            break;
        }
    }
    return sq;
}

IE_Compute::HairDensityReportData IE_Compute::computeHairDensity(InputData id)
{
    HairDensityReportData d;

    for (QList<LayerComputeData>::iterator tmpIter = id.layerList.begin();
         tmpIter!=id.layerList.end();
         tmpIter++
         )
    {
        if( tmpIter.i->t().toolType == ToolType::DensityAndDiameter)
        {
            d.hairQuantity++;

            if  (_p_ie_global_data->convertF(tmpIter.i->t().width)
                    >= _p_ie_global_data->getThreshold_TW()
                )
                d.terminalQuantity++;
            else
                d.vellusQuantity++;


        }
    }

    d.inputData.squareInPixels = computeSquareInPixels();

    qreal squareInUnitedUnits = (d.inputData.squareInPixels /
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

IE_Compute::HairDiameterReportData IE_Compute::computeHairDiameter(InputData id)
{
    HairDiameterReportData d;

    for (QList<LayerComputeData>::iterator tmpIter = id.layerList.begin();
         tmpIter!=id.layerList.end();
         tmpIter++
         )
    {
        if( tmpIter.i->t().toolType == ToolType::DensityAndDiameter)
        {
            d.hairQuantity++;

            qreal convertedPenWidth = _p_ie_global_data->convertF(tmpIter.i->t().width);
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

    d.inputData.squareInPixels = computeSquareInPixels();
    d.squareInUnitedUnits = d.inputData.squareInPixels
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
IE_Compute::OutputData IE_Compute::compute(IE_Compute::InputData id)
{
    OutputData outD;
    outD.computeType = id.computeType;
    if( (id.computeType&ComputeType::HairDensity) == ComputeType::HairDensity)
    {
        outD.density = computeHairDensity(id);
    }
    if( (id.computeType&ComputeType::HairDiameter) == ComputeType::HairDiameter)
    {
        outD.diameter = computeHairDiameter(id);
    }
    return outD;
}

IE_Compute::OutputData IE_Compute::compute(IE_Compute::ComputeType compType)
{
    InputData inputD = getInputData();
    inputD.computeType = compType;
    return compute(inputD);
}

IE_Compute::InputData IE_Compute::getInputData() const
{
    InputData inputD;



    for (QList<IE_ModelLayer *>::iterator tmpIter = layersList->begin();
         tmpIter!=layersList->end();tmpIter++)
    {
        //! \warning для других типов слоев не будет работать.
        if( tmpIter.i->t()->getToolType() == ToolType::DensityAndDiameter)
        {
            if(!tmpIter.i->t()->isVisible())
                continue;
            inputD.layerList.append(LayerComputeData(
                                        dynamic_cast<IELine*>(tmpIter.i->t()->getToolPtr())->getPenWidth(),
                                        dynamic_cast<IELine*>(tmpIter.i->t()->getToolPtr())->getLineLength(),
                                        tmpIter.i->t()->getToolType()
                                        ));
        }
    }


    inputD.squareInPixels = computeSquareInPixels();
    return inputD;
}


