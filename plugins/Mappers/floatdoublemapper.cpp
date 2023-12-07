#include "floatdoublemapper.h"

FloatDoubleMapper::FloatDoubleMapper(QWidget *parent)
    : ConvertTypeMapper(parent)
{}

void FloatDoubleMapper::emitDoubleParam(float f) {
    emit convertedToDouble(static_cast<double>(f));
}
void FloatDoubleMapper::emitFloatParam(double d) {
    emit convertedToFloat(static_cast<float>(d));
}
