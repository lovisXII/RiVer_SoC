    sc_bv<dec2exe_size_s2> dec2exe_out_var = dec2exe_out_sd_s2.read();

    PC_BRANCH_VALUE_RD_S2.write((sc_bv_base)dec2exe_out_var.range(251, 220));
    MULT_INST_RD_S2.write((bool)dec2exe_out_var[219]);
    EBREAK_RD_S2.write((bool)dec2exe_out_var[218]);
    INSTRUCTION_ACCESS_FAULT_RD_S2.write((bool)dec2exe_out_var[217]);
    MRET_RD_S2.write((bool)dec2exe_out_var[216]);
    BLOCK_BP_RD_S2.write((bool)dec2exe_out_var[215]);
    EXCEPTION_RD_S2.write((bool)dec2exe_out_var[214]);
    ENV_CALL_WRONG_MODE_RD_S2.write((bool)dec2exe_out_var[213]);
    ENV_CALL_U_MODE_RD_S2.write((bool)dec2exe_out_var[212]);
    ILLEGAL_INSTRUCTION_RD_S2.write((bool)dec2exe_out_var[211]);
    ADRESS_MISSALIGNED_RD_S2.write((bool)dec2exe_out_var[210]);
    ENV_CALL_M_MODE_RD_S2.write((bool)dec2exe_out_var[209]);
    ENV_CALL_S_MODE_RD_S2.write((bool)dec2exe_out_var[208]);
    CSR_RDATA_RD_S2.write((sc_bv_base)dec2exe_out_var.range(207, 176));
    CSR_WENABLE_RD_S2.write((bool)dec2exe_out_var[175]);
    CSR_WADR_RD_S2.write((sc_bv_base)dec2exe_out_var.range(174, 163));
    PC_DEC2EXE_RD_S2.write((sc_bv_base)dec2exe_out_var.range(162, 131));
    BP_R1_VALID_RD_S1.write((bool)dec2exe_out_var[130]);
    BP_R2_VALID_RD_S1.write((bool)dec2exe_out_var[129]);

    BP_RADR1_RD_S1.write((sc_bv_base)dec2exe_out_var.range(128, 123));
    BP_RADR2_RD_S1.write((sc_bv_base)dec2exe_out_var.range(122, 117));

    EXE_CMD_RD_S2.write((sc_bv_base)dec2exe_out_var.range(116, 115));
    OP1_RD_S2.write((sc_bv_base)dec2exe_out_var.range(114, 83));
    OP2_RD_S2.write((sc_bv_base)dec2exe_out_var.range(82, 51));
    NEG_OP2_RD_S2.write((bool)dec2exe_out_var[50]);
    WB_RD_S2.write((bool)dec2exe_out_var[49]);

    MEM_DATA_RD_S2.write((sc_bv_base)dec2exe_out_var.range(48, 17));

    MEM_LOAD_RD_S2.write((bool)dec2exe_out_var[16]);
    MEM_STORE_RD_S2.write((bool)dec2exe_out_var[15]);

    MEM_SIGN_EXTEND_RD_S2.write((bool)dec2exe_out_var[14]);
    MEM_SIZE_RD_S2.write((sc_bv_base)dec2exe_out_var.range(13, 12));
    SELECT_TYPE_OPERATIONS_RD_S2.write((sc_bv_base)dec2exe_out_var.range(11, 8));
    EXE_DEST_SD_S2.write((sc_bv_base)dec2exe_out_var.range(7, 2));
    SLT_RD_S2.write((bool)dec2exe_out_var[1]);
    SLTU_RD_S2.write((bool)dec2exe_out_var[0]);