#pragma once

#include "converttypemapper.h"

class FloatDoubleMapper : public ConvertTypeMapper
{
    Q_OBJECT

public:
    FloatDoubleMapper(QWidget *parent = 0);
signals:
    void convertedToFloat(float);
    void convertedToDouble(double);
public slots:
    void emitDoubleParam(float f);
    void emitFloatParam(double d);
};
