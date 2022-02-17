#include "core.h"

void core::trace(sc_trace_file* tf) {
    ifetch_inst.trace(tf);
    dec_inst.trace(tf);
    exec_inst.trace(tf);
    mem_inst.trace(tf);
    wbk_inst.trace(tf);
    reg_inst.trace(tf);
}

