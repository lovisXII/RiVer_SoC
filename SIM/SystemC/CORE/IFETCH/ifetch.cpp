#include "ifetch.h"

void ifetch::fetch_method() {
    sc_bv<if2dec_size> if2dec_in_var;
    sc_bv<if2dec_size> instr_ri_var = instr_ri.read();
    if (EXCEPTION_SM.read() == 0) {
        if (PRED_TAKEN_SD.read() && !PRED_FAILED_RD.read()) {
            ADR_SI.write(PRED_ADR_SD.read());
        } else {
            ADR_SI.write(PC_RD.read());
        }

        // data sent in if2dec
        if2dec_in_var[96]           = PRED_ADR_TAKEN_SI.read();
        if2dec_in_var.range(95, 64) = PRED_NEXT_ADR_SI.read();
        if2dec_in_var.range(63, 32) = (sc_bv_base)INST_SIC.read();
        if2dec_in_var.range(31, 0) =
            (PRED_TAKEN_SD.read() && !PRED_FAILED_RD.read()) ? PRED_ADR_SD.read() : PC_RD.read();
        if2dec_in_si.write(if2dec_in_var);

        // data coming out from if2dec :
        PRED_TAKEN_RI.write((bool)instr_ri_var[96]);
        PRED_ADR_RI.write((sc_bv_base)instr_ri_var.range(95, 64));
        INSTR_RI.write((sc_bv_base)instr_ri_var.range(63, 32));
        PC_IF2DEC_RI.write((sc_bv_base)instr_ri_var.range(31, 0));
        if (IF2DEC_FLUSH_SD.read()) {
            IF2DEC_PUSH_SI.write(false);
            DEC2IF_POP_SI.write(true);
            ADR_VALID_SI.write(false);
        } else {
            // stall if the memory stalls, if we can't push to dec, or have no value
            // of pc to pop from dec
            bool stall = STALL_SIC.read() || IF2DEC_FULL_SI.read() || DEC2IF_EMPTY_SI.read();
            IF2DEC_PUSH_SI.write(!stall);
            DEC2IF_POP_SI.write(!stall);
            ADR_VALID_SI.write(!DEC2IF_EMPTY_SI.read());
        }

    } else {
        // data sent in if2dec
        // If an exception is detected
        // Pipeline pass in M-mode
        // Fifo send nop instruction
        if2dec_in_var[96]           = PRED_ADR_TAKEN_SI.read();
        if2dec_in_var.range(95, 64) = PRED_NEXT_ADR_SI.read();
        if2dec_in_var.range(63, 32) = nop_encoding;
        if2dec_in_var.range(31, 0) =
            (PRED_TAKEN_SD.read() && !PRED_FAILED_RD.read()) ? PRED_ADR_SD.read() : PC_RD.read();
        if (PRED_TAKEN_SD.read() && !PRED_FAILED_RD.read()) {
            ADR_SI.write(PRED_ADR_SD.read());
        } else {
            ADR_SI.write(PC_RD.read());
        }

        if2dec_in_si.write(if2dec_in_var);

        // data coming out from if2dec :
        PRED_TAKEN_RI.write((bool)instr_ri_var[96]);
        PRED_ADR_RI.write((sc_bv_base)instr_ri_var.range(95, 64));
        INSTR_RI.write((sc_bv_base)instr_ri_var.range(63, 32));
        PC_IF2DEC_RI.write((sc_bv_base)instr_ri_var.range(31, 0));
        IF2DEC_PUSH_SI.write(true);
        DEC2IF_POP_SI.write(true);
        ADR_VALID_SI.write(false);
    }
}

void ifetch::exception()
// No exception in IFECTH
{
    EXCEPTION_RI.write(0);
}
void ifetch::write_pred_reg() {
    int index = 0;
    if (INSTR_IS_BRANCH_RD.read() && !IF2DEC_EMPTY_SI.read()) {
        bool found = false;
        for (int i = 0; i < predictor_register_size; ++i) {
            if (BRANCH_ADR_REG[i] == BRANCH_INST_ADR_RD.read()) {
                found = true;
                index = i;
                break;
            }
        }
        if (!found) {
            BRANCH_ADR_REG[pred_write_pointer_si.read()]    = BRANCH_INST_ADR_RD.read();
            PREDICTED_ADR_REG[pred_write_pointer_si.read()] = BRANCH_TARGET_ADR_RD.read();
            PRED_STATE_REG[pred_write_pointer_si.read()]    = weakly_taken;
            VALID_PRED_REG[pred_write_pointer_si.read()]    = true;
            sc_uint<size_of_pred_pointer> pointer           = pred_write_pointer_si.read();
            pred_write_pointer_si                           = pred_write_pointer_si.read() + 1;
        } else {
            PRED_STATE_REG[index] = next_state_pred_si;
        }
    }
}


// ---------------------------------------------
//            BRANCH PREDICTION GESTION
// ---------------------------------------------
void ifetch::read_pred_reg() {
    bool found = false;
    for (int i = 0; i < predictor_register_size; ++i) {
        if (BRANCH_ADR_REG[i].read() == PC_RD.read() && VALID_PRED_REG[i].read() && !EXCEPTION_SM && !DEC2IF_EMPTY_SI) {
#ifdef BRANCH_PREDICTION
            found = true;
#endif
            pred_branch_next_adr_si = PREDICTED_ADR_REG[i];
            pred_branch_taken_si =
                (bool)(PRED_STATE_REG[i].read() == strongly_taken) || (PRED_STATE_REG[i].read() == weakly_taken);
            break;
        }
    }
    if (!found) { pred_branch_taken_si = false; }
}
void ifetch::calc_prob_pred() {
    for (int i = 0; i < predictor_register_size; ++i) {
        if (BRANCH_ADR_REG[i].read() & BRANCH_INST_ADR_RD.read()) {
            switch (PRED_STATE_REG[i].read()) {
                case strongly_taken: next_state_pred_si = PRED_SUCCESS_RD.read() ? strongly_taken : weakly_taken; break;
                case weakly_taken:
                    next_state_pred_si = PRED_SUCCESS_RD.read() ? strongly_taken : weakly_not_taken;
                    break;
                case weakly_not_taken:
                    next_state_pred_si = PRED_SUCCESS_RD.read() ? weakly_taken : strongly_not_taken;
                    break;
                case strongly_not_taken:
                    next_state_pred_si = PRED_SUCCESS_RD.read() ? weakly_not_taken : strongly_not_taken;
                    break;
            }
            break;
        }
    }
}
void ifetch::write_pred_ret_reg() {
    bool found = false;
    if (RET_INST_RD.read() && !IF2DEC_EMPTY_SI.read()) {
        for (int i = 0; i < ret_predictor_register_size; ++i) {
            if (RET_ADR_RI[i] == BRANCH_INST_ADR_RD.read()) {
                found = true;
                break;
            }
        }
        if (!found) {
            RET_ADR_RI[ret_write_pointer_si.read()]    = BRANCH_INST_ADR_RD.read();
            VALID_RET_REG[ret_write_pointer_si.read()] = true;
            ret_write_pointer_si                       = ret_write_pointer_si.read() << 1;
        }
    }
}
void ifetch::ret_stack() {
    if (!RESET) {
        ret_stack_pointer_si = 1;
        pred_ret_taken_si    = false;
    } else {
        bool found = false;
        for (int i = 0; i < ret_predictor_register_size; ++i) {
            if (RET_ADR_RI[i].read() == PC_RD.read() && VALID_RET_REG[i].read()) {
#ifdef RET_BRANCH_PREDICTION
                found = true;
#endif
                break;
            }
        }
        pred_ret_taken_si = found;

        sc_uint<ret_stack_size> ras_temp   = ret_stack_pointer_si.read();
        bool                    adr_pushed = false;
        if (!IF2DEC_EMPTY_SI.read()) {
            if (PUSH_ADR_RAS_RD.read()) {
                // ret inst found on decod stage => pop @
                for (int i = 0; i < ret_stack_size; i++)
                    if (ras_temp[i]) {
                        RET_STACK_RI[i] = RETURN_ADR_RD.read();
                        adr_pushed      = true;
                    }

                ras_temp = ras_temp << 1;

            } else if (POP_ADR_RAS_RD.read()) {
                // push valid adr to ret on stack
                ras_temp = ras_temp >> 1;
            }

            if (found) {
                // pc@ == ret inst => pop @ to ret
                for (int i = 0; i < ret_stack_size - 1; i++) {
                    if (ras_temp[i + 1]) { pred_ret_next_adr_si = adr_pushed ? RETURN_ADR_RD.read() : RET_STACK_RI[i]; }
                }
                ras_temp = ras_temp >> 1;
            }
        }
        ret_stack_pointer_si = ras_temp;
    }
}

void ifetch::next_pred_adr() {
    if (pred_branch_taken_si)
        PRED_NEXT_ADR_SI.write(pred_branch_next_adr_si);
    else if (pred_ret_taken_si)
        PRED_NEXT_ADR_SI.write(pred_ret_next_adr_si);

    PRED_ADR_TAKEN_SI = pred_branch_taken_si | pred_ret_taken_si;
}
void ifetch::trace(sc_trace_file* tf) {
    sc_trace(tf, ADR_SI, GET_NAME(ADR_SI));
    sc_trace(tf, ADR_VALID_SI, GET_NAME(ADR_VALID_SI));
    sc_trace(tf, INST_SIC, GET_NAME(INST_SIC));
    sc_trace(tf, STALL_SIC, GET_NAME(STALL_SIC));
    sc_trace(tf, DEC2IF_EMPTY_SI, GET_NAME(DEC2IF_EMPTY_SI));
    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));
    sc_trace(tf, IF2DEC_FLUSH_SD, GET_NAME(IF2DEC_FLUSH_SD));
    sc_trace(tf, IF2DEC_POP_SD, GET_NAME(IF2DEC_POP_SD));
    sc_trace(tf, IF2DEC_PUSH_SI, GET_NAME(IF2DEC_PUSH_SI));
    sc_trace(tf, IF2DEC_FULL_SI, GET_NAME(IF2DEC_FULL_SI));
    sc_trace(tf, IF2DEC_EMPTY_SI, GET_NAME(IF2DEC_EMPTY_SI));
    sc_trace(tf, PC_RD, GET_NAME(PC_RD));
    sc_trace(tf, INSTR_RI, GET_NAME(INSTR_RI));
    sc_trace(tf, PC_IF2DEC_RI, GET_NAME(PC_IF2DEC_RI));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, if2dec_in_si, GET_NAME(if2dec_in_si));
    sc_trace(tf, instr_ri, GET_NAME(instr_ri));
    sc_trace(tf, EXCEPTION_RI, GET_NAME(EXCEPTION_RI));
    sc_trace(tf, EXCEPTION_SM, GET_NAME(EXCEPTION_SM));
    sc_trace(tf, INTERRUPTION_SE, GET_NAME(INTERRUPTION_SE));
    sc_trace(tf, MRET_SM, GET_NAME(MRET_SM));
    sc_trace(tf, INSTR_IS_BRANCH_RD, GET_NAME(INSTR_IS_BRANCH_RD));
    sc_trace(tf, BRANCH_INST_ADR_RD, GET_NAME(BRANCH_INST_ADR_RD));
    sc_trace(tf, BRANCH_TARGET_ADR_RD, GET_NAME(BRANCH_TARGET_ADR_RD));
    sc_trace(tf, PRED_ADR_RI, GET_NAME(PRED_ADR_RI));
    sc_trace(tf, PRED_TAKEN_RI, GET_NAME(PRED_TAKEN_RI));
    sc_trace(tf, PRED_NEXT_ADR_SI, GET_NAME(PRED_NEXT_ADR_SI));
    sc_trace(tf, PRED_ADR_TAKEN_SI, GET_NAME(PRED_ADR_TAKEN_SI));
    sc_trace(tf, pred_write_pointer_si, GET_NAME(pred_write_pointer_si));
    sc_trace(tf, PRED_SUCCESS_RD, GET_NAME(PRED_SUCCESS_RD));
    sc_trace(tf, next_state_pred_si, GET_NAME(next_state_pred_si));
    sc_trace(tf, PRED_FAILED_RD, GET_NAME(PRED_FAILED_RD));

    for (int i = 0; i < predictor_register_size; i++) {
        std::string regname = "REG_ADR_";
        regname += std::to_string(i);
        sc_trace(tf, BRANCH_ADR_REG[i], signal_get_name(BRANCH_ADR_REG[i].name(), regname.c_str()));

        regname = "REG_PRED_";
        regname += std::to_string(i);
        sc_trace(tf, PREDICTED_ADR_REG[i], signal_get_name(PREDICTED_ADR_REG[i].name(), regname.c_str()));

        regname = "REG_STATE_";
        regname += std::to_string(i);
        sc_trace(tf, PRED_STATE_REG[i], signal_get_name(PRED_STATE_REG[i].name(), regname.c_str()));
    }

    for (int i = 0; i < ret_stack_size; i++) {
        std::string regname = "STACK_ADR_";
        regname += std::to_string(i);
        sc_trace(tf, RET_STACK_RI[i], signal_get_name(RET_STACK_RI[i].name(), regname.c_str()));
    }

    for (int i = 0; i < ret_predictor_register_size; i++) {
        std::string regname = "RET_REG_ADR_";
        regname += std::to_string(i);
        sc_trace(tf, RET_ADR_RI[i], signal_get_name(RET_ADR_RI[i].name(), regname.c_str()));
    }

    sc_trace(tf, PUSH_ADR_RAS_RD, GET_NAME(PUSH_ADR_RAS_RD));
    sc_trace(tf, POP_ADR_RAS_RD, GET_NAME(POP_ADR_RAS_RD));
    sc_trace(tf, RETURN_ADR_RD, GET_NAME(RETURN_ADR_RD));
    sc_trace(tf, RET_INST_RD, GET_NAME(RET_INST_RD));
    sc_trace(tf, ret_stack_pointer_si, GET_NAME(ret_stack_pointer_si));
    sc_trace(tf, pred_branch_taken_si, GET_NAME(pred_branch_taken_si));
    sc_trace(tf, pred_ret_taken_si, GET_NAME(pred_ret_taken_si));
    sc_trace(tf, pred_ret_next_adr_si, GET_NAME(pred_ret_next_adr_si));
    sc_trace(tf, pred_branch_next_adr_si, GET_NAME(pred_branch_next_adr_si));

    fifo_inst.trace(tf);
}