#include "_global_ie.h"

_global_ie::_global_ie(QObject *parent):QObject (parent)
{
    ownerSet = false;
    measureIndex = 1.0;
    measureIndexUnitType = UnitType::mm;
    unitType = UnitType::px;
    threshold_terminal_wellus = 40;
    threshold_terminal_wellus_unitType = UnitType::px;
}

qreal _global_ie::getMeasureIndex() const
{
    return measureIndex;
}
UnitType _global_ie::getUnitType() const
{
    return unitType;
}
qreal _global_ie::getThreshold_TW() const
{
    return threshold_terminal_wellus;
}

qreal _global_ie::convertF(qreal value) const
{
    return convertWithForamtF(value, unitType);
}

qreal _global_ie::convertWithForamtF(qreal value, UnitType uType) const
{
    qreal answer;

    switch (uType) {
    case UnitType::m:
        answer = value/(measureIndex/1000);
        break;
    case UnitType::cm:
        answer = value/(measureIndex/10);
        break;
    case UnitType::mm:
        answer = value/measureIndex;
        break;
    case UnitType::px:
        answer = value;
        break;
    case UnitType::um:
        answer = value/(measureIndex*1000);
        break;
    }
    return answer;
}


int _global_ie::getIndexThreshold_terminal_wellus_unitType() const
{
    return (int)threshold_terminal_wellus_unitType;
}

void _global_ie::setMeasureIndex(qreal mIndex, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        measureIndex = mIndex;
        emit measureIndexChanged(mIndex);
        emit changed();
    }
}
void _global_ie::setUnitType(UnitType uType, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        unitType = uType;
        emit unitTypeChanged(uType);
        emit changed();
    }
}

void _global_ie::setIndexUnitType(int uType, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        switch (uType) {
        case 0:
            unitType = UnitType::m;
            break;
        case 1:
            unitType = UnitType::cm;
            break;
        case 2:
            unitType = UnitType::mm;
            break;
        case 3:
            unitType = UnitType::um;
            break;
        case 4:
            unitType = UnitType::px;
            break;

        }
        emit unitTypeChanged(unitType);
        emit changed();
    }
}
void _global_ie::setThreshold_TW(qreal data, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        threshold_terminal_wellus = data;
        //emit unitTypeChanged(uType);
        emit changed();
    }
}
void _global_ie::setOwner(int randomKey)
{
    if(!ownerSet)
    {
        this->randomKey = randomKey;
        ownerSet = true;
        emit changed();
    }
}
void _global_ie::setThreshold_terminal_wellus_unitType(int value, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        switch (value) {
        case 0:
            threshold_terminal_wellus_unitType = UnitType::m;
            break;
        case 1:
            threshold_terminal_wellus_unitType = UnitType::cm;
            break;
        case 2:
            threshold_terminal_wellus_unitType = UnitType::mm;
            break;
        case 3:
            threshold_terminal_wellus_unitType = UnitType::um;
            break;
        case 4:
            threshold_terminal_wellus_unitType = UnitType::px;
            break;

        }
        //emit unitTypeChanged(uType);
        emit changed();
    }
}




