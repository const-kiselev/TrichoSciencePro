#include "_global_ie.h"

#include <QStandardPaths>

_global_ie::_global_ie(int rk, QObject *parent)
    :   QObject (parent),
        ownerSet(true),
        randomKey(rk)
{
    measureIndex = 1.0;
    unitType = UnitType::px;
    threshold_terminal_wellus = 0.04;
    threshold_thinHair_top = 0.06;
    threshold_mediumHair_top = 0.08;
    modelDirPath
            = modelResDirPath
            = tmp_modelDirPath
            = tmp_modelResDirPath = "";
    lastSelectedDirByUser = QStandardPaths::writableLocation(
                QStandardPaths::DocumentsLocation);
}

QString _global_ie::getModelDirPath() const
{
    return modelDirPath;
}

void _global_ie::setModelDirPath(const QString &value, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        modelDirPath = value;
        emit changed();
    }
}

QString _global_ie::getModelResDirPath() const
{
    return modelResDirPath;
}

void _global_ie::setModelResDirPath(const QString &value, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        modelResDirPath = value;
        emit changed();
    }
}

QString _global_ie::getTmp_modelDirPath() const
{
    return tmp_modelDirPath;
}

void _global_ie::setTmp_modelDirPath(const QString &value, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        tmp_modelDirPath = value;
        emit changed();
    }
}

QString _global_ie::getTmp_modelResDirPath() const
{
    return tmp_modelResDirPath;
}

void _global_ie::setTmp_modelResDirPath(const QString &value, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        tmp_modelResDirPath = value;
        emit changed();
    }
}

QString _global_ie::getLastSelectedDirByUser() const
{
    return lastSelectedDirByUser;
}

void _global_ie::setLastSelectedDirByUser(const QString &value)
{
    lastSelectedDirByUser = value;
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

qreal _global_ie::getThreshold_thinHair() const
{
    return threshold_thinHair_top;
}

qreal _global_ie::getThreshold_mediumHair() const
{
    return threshold_mediumHair_top;
}

qreal _global_ie::convertF(qreal value) const
{
    return convertPixelWithForamtF(value, unitType);
}

qreal _global_ie::convertPixelWithForamtF(qreal value, UnitType uType) const
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

qreal _global_ie::convertUnitedWithForamtF(qreal value, UnitType uType) const
{
    qreal answer;
    switch (unitType) {
    case UnitType::m:
        answer = value;
        break;
    case UnitType::cm:
        answer = value/100.0;
        break;
    case UnitType::mm:
        answer = value/1000.0;
        break;
    case UnitType::um:
        answer = value/1000000.0;
        break;
    }

    switch (uType) {
    case UnitType::cm:
        answer *= 100.0;
        break;
    case UnitType::mm:
        answer *= 1000.0;
        break;
    case UnitType::px:
        answer = value/(measureIndex*1000.0);
        break;
    case UnitType::um:
        answer *= 1000000.0;
        break;
    }
    return answer;
}




void _global_ie::setMeasureIndex(qreal mIndex, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        measureIndex = mIndex;
        unitType = UnitType::mm;
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

void _global_ie::setThreshold_thinHair(qreal data, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        threshold_thinHair_top = data;
        //emit unitTypeChanged(uType);
        emit changed();
    }
}

void _global_ie::setThreshold_mediumHair(qreal data, int key)
{
    if(!ownerSet)
        return;
    if(key == randomKey)
    {
        threshold_mediumHair_top = data;
        //emit unitTypeChanged(uType);
        emit changed();
    }
}






