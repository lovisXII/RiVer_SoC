        exec_inst_s2.RADR1_RD_S2(BP_RADR1_RD);
        exec_inst_s2.RADR2_RD_S2(BP_RADR2_RD);
        exec_inst_s2.OP1_VALID_RD_S2(BP_R1_VALID_RD);
        exec_inst_s2.OP2_VALID_RD_S2(BP_R2_VALID_RD);
        exec_inst_s2.MEM_DEST_RM_S2(DEST_RM_S2);
        exec_inst_s2.MEM_RES_RM_S2(MEM_RES_RM_S2);
        exec_inst_s2.OP1_RD_S2(OP1_RD_S2);
        exec_inst_s2.OP2_RD_S2(OP2_RD_S2);
        exec_inst_s2.CMD_RD_S2(EXE_CMD_RD_S2);
        exec_inst_s2.DEST_RD_S2(EXE_DEST_SD_S2);
        exec_inst_s2.NEG_OP2_RD_S2(NEG_OP2_RD_S2);
        exec_inst_s2.WB_RD_S2(WB_RD_S2);
        exec_inst_s2.SELECT_TYPE_OPERATIONS_RD_S2(SELECT_TYPE_OPERATIONS_RD_S2);
        exec_inst_s2.EBREAK_RD_S2(EBREAK_RD_S2);
        exec_inst_s2.EBREAK_RE_S2(EBREAK_RE_S2);

        exec_inst_s2.MULT_INST_RD_S2(MULT_INST_RD_S2);
        exec_inst_s2.PC_DEC2EXE_RD_S2(PC_DEC2EXE_RD_S2);
        exec_inst_s2.PC_EXE2MEM_RE_S2(PC_EXE2MEM_RE_S2);

        exec_inst_s2.MEM_DATA_RD_S2(MEM_DATA_RD_S2);
        exec_inst_s2.MEM_LOAD_RD_S2(MEM_LOAD_RD_S2);
        exec_inst_s2.MEM_STORE_RD_S2(MEM_STORE_RD_S2);
        exec_inst_s2.MEM_SIGN_EXTEND_RD_S2(MEM_SIGN_EXTEND_RD_S2);
        exec_inst_s2.MEM_SIZE_RD_S2(MEM_SIZE_RD_S2);
        exec_inst_s2.SLT_RD_S2(SLT_RD_S2);
        exec_inst_s2.SLTU_RD_S2(SLTU_RD_S2);

        exec_inst_s2.DEC2EXE_POP_SE_S2(DEC2EXE_POP_SE_S2);
        exec_inst_s2.DEC2EXE_EMPTY_SD_S2(DEC2EXE_EMPTY_SD_S2);

        exec_inst_s2.EXE_RES_RE_S2(EXE_RES_RE_S2);
        exec_inst_s2.MEM_DATA_RE_S2(MEM_DATA_RE_S2);
        exec_inst_s2.DEST_RE_S2(DEST_RE_S2);
        exec_inst_s2.MEM_SIZE_RE_S2(MEM_SIZE_RE_S2);

        exec_inst_s2.WB_RE_S2(MEM_WB);
        exec_inst_s2.MEM_SIGN_EXTEND_RE_S2(MEM_SIGN_EXTEND_RE_S2);
        exec_inst_s2.MEM_LOAD_RE_S2(MEM_LOAD_RE_S2);
        exec_inst_s2.MEM_STORE_RE_S2(MEM_STORE_RE_S2);
        exec_inst_s2.MULT_INST_RE_S2(MULT_INST_RE_S2);
        exec_inst_s2.MULT_SEL_HIGH_RE_S2(MULT_SEL_HIGH_RE_S2);
        exec_inst_s2.EXE2MEM_EMPTY_SE_S2(EXE2MEM_EMPTY_SE_S2);
        exec_inst_s2.EXE2MEM_POP_SM_S2(EXE2MEM_POP_SM_S2);

        exec_inst_s2.CSR_WENABLE_RM_S2(CSR_WENABLE_RM_S2);
        exec_inst_s2.CSR_RDATA_RM_S2(CSR_RDATA_RM_S2);

        exec_inst_s2.INTERRUPTION_SE(INTERRUPTION_SE);
        exec_inst_s2.MACHINE_SOFTWARE_INTERRUPT_SX(MACHINE_SOFTWARE_INTERRUPT_SX);
        exec_inst_s2.MACHINE_TIMER_INTERRUPT_SX(MACHINE_TIMER_INTERRUPT_SX);
        exec_inst_s2.MACHINE_EXTERNAL_INTERRUPT_SX(MACHINE_EXTERNAL_INTERRUPT_SX);
        exec_inst_s2.MACHINE_SOFTWARE_INTERRUPT_SE_S2(MACHINE_SOFTWARE_INTERRUPT_SE_S2);
        exec_inst_s2.MACHINE_TIMER_INTERRUPT_SE(MACHINE_TIMER_INTERRUPT_SE);
        exec_inst_s2.MACHINE_EXTERNAL_INTERRUPT_SE(MACHINE_EXTERNAL_INTERRUPT_SE);
        exec_inst_s2.EXCEPTION_RD_S2(EXCEPTION_RD_S2);
        exec_inst_s2.CSR_WENABLE_RD_S2(CSR_WENABLE_RD_S2);
        exec_inst_s2.CSR_WENABLE_RE_S2(CSR_WENABLE_RE_S2);
        exec_inst_s2.CSR_WADR_RD_S2(CSR_WADR_RD_S2);
        exec_inst_s2.CSR_WADR_RE(CSR_WADR_RE);
        exec_inst_s2.CSR_RDATA_RE_S2(CSR_RDATA_RE_S2);
        exec_inst_s2.CSR_RDATA_RD_S2(CSR_RDATA_RD_S2);
        exec_inst_s2.ENV_CALL_S_MODE_RD_S2(ENV_CALL_S_MODE_RD_S2);
        exec_inst_s2.ENV_CALL_WRONG_MODE_RD_S2(ENV_CALL_WRONG_MODE_RD_S2);
        exec_inst_s2.ILLEGAL_INSTRUCTION_RD_S2(ILLEGAL_INSTRUCTION_RD_S2);  // accessing stuff in wrong mode
        exec_inst_s2.ADRESS_MISSALIGNED_RD_S2(ADRESS_MISSALIGNED_RD_S2);    // branch offset is misaligned
        exec_inst_s2.ENV_CALL_U_MODE_RD_S2(ENV_CALL_U_MODE_RD_S2);
        exec_inst_s2.ENV_CALL_M_MODE_RD_S2(ENV_CALL_M_MODE_RD_S2);
        exec_inst_s2.MRET_RD_S2(MRET_RD_S2);

        exec_inst_s2.EXCEPTION_RE_S2(EXCEPTION_RE_S2);
        exec_inst_s2.LOAD_ADRESS_MISSALIGNED_RE_S2(LOAD_ADRESS_MISSALIGNED_RE_S2);
        exec_inst_s2.LOAD_ACCESS_FAULT_RE_S2(LOAD_ACCESS_FAULT_RE_S2);
        exec_inst_s2.STORE_ADRESS_MISSALIGNED_RE_S2(STORE_ADRESS_MISSALIGNED_RE_S2);
        exec_inst_s2.STORE_ACCESS_FAULT_RE_S2(STORE_ACCESS_FAULT_RE_S2);
        exec_inst_s2.ENV_CALL_WRONG_MODE_RE_S2(ENV_CALL_WRONG_MODE_RE_S2);
        exec_inst_s2.ENV_CALL_U_MODE_RE_S2(ENV_CALL_U_MODE_RE_S2);
        exec_inst_s2.ILLEGAL_INSTRUCTION_RE_S2(ILLEGAL_INSTRUCTION_RE_S2);
        exec_inst_s2.INSTRUCTION_ADRESS_MISSALIGNED_RE_S2(INSTRUCTION_ADRESS_MISSALIGNED_RE_S2);
        exec_inst_s2.ENV_CALL_S_MODE_RE_S2(ENV_CALL_S_MODE_RE_S2);
        exec_inst_s2.ENV_CALL_M_MODE_RE_S2(ENV_CALL_M_MODE_RE_S2);
        exec_inst_s2.EXCEPTION_SM(EXCEPTION_SM);
        exec_inst_s2.BLOCK_BP_RD_S2(BLOCK_BP_RD_S2);
        exec_inst_s2.CURRENT_MODE_SM_S2(CURRENT_MODE_SM_S2);
        exec_inst_s2.MRET_RE_S2(MRET_RE_S2);
        exec_inst_s2.INSTRUCTION_ACCESS_FAULT_RD_S2(INSTRUCTION_ACCESS_FAULT_RD_S2);
        exec_inst_s2.INSTRUCTION_ACCESS_FAULT_RE_S2(INSTRUCTION_ACCESS_FAULT_RE_S2);
        exec_inst_s2.PC_BRANCH_VALUE_RD_S2(PC_BRANCH_VALUE_RD_S2);
        exec_inst_s2.PC_BRANCH_VALUE_RE_S2(PC_BRANCH_VALUE_RE_S2);

        exec_inst_s2.OP1_SE_S2(op1_se_s2);
        exec_inst_s2.OP2_SE_S2(op2_se);

        exec_inst_s2.MULT_INST_RM_S2(MULT_INST_RM_S2);
        exec_inst_s2.BP_MEM2WBK_EMPTY_SM_S2(MEM2WBK_EMPTY_SM_S2);

        exec_inst_s2.CLK(CLK);
        exec_inst_s2.RESET(RESET);
