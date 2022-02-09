#include "ifetch.h"

void ifetch::fetch_method() 
{
    IF_ADR.write(DEC_PC.read());
    IF_ADR_VALID.write(!DEC2IF_EMPTY.read());
    if (!IF2DEC_FLUSH.read()) {
        bool stall = IC_STALL.read() || IF2DEC_FULL.read() || DEC2IF_EMPTY.read();
        IF2DEC_PUSH.write(!stall);
        DEC2IF_POP.write(!stall);
    }
    else {
        IF2DEC_PUSH.write(false);
        DEC2IF_POP.write(true);
    }
}