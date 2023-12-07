#include "intuintmapper.h"

IntUIntMapper::IntUIntMapper(QWidget *parent) :
    ConvertTypeMapper(parent)
{
}

void IntUIntMapper::emitUIntParam(int _int) {
    if (_int >= 0)
        emit convertedToUInt(static_cast<uint>(_int));
    else
        emit convertedToUIntNegative(static_cast<uint>(-_int));
}
void IntUIntMapper::emitIntParam(uint _uint) {
    emit convertedToInt(static_cast<int>(_uint));
}

