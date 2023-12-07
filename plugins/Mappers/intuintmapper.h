#pragma once

#include "converttypemapper.h"

class IntUIntMapper : public ConvertTypeMapper
{
    Q_OBJECT

public:
    IntUIntMapper(QWidget *parent = 0);

signals:
    void convertedToUInt(uint);
    void convertedToUIntNegative(uint);
    void convertedToInt(int);
public slots:
    void emitUIntParam(int _int);
    void emitIntParam(uint _uint);
};
