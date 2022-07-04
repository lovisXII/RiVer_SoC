

        // WBK_S1 port map :

        wbk_inst_s1.MEM_RES_RM_S1(MEM_RES_RM_S1);
        wbk_inst_s1.DEST_RM_S1(DEST_RM_S1);
        wbk_inst_s1.WB_RM_S1(WB_RM_S1);
        wbk_inst_s1.MEM2WBK_EMPTY_SM_S1(MEM2WBK_EMPTY_SM_S1);
        wbk_inst_s1.MEM2WBK_POP_SW_S1(MEM2WBK_POP_SW_S1);

        wbk_inst_s1.WADR_SW_S1(WADR_SW_S1);
        wbk_inst_s1.WDATA_SW_S1(WDATA_SW_S1);
        wbk_inst_s1.WENABLE_SW_S1(WENABLE_SW_S1);
        wbk_inst_s1.CSR_RDATA_RM_S1(CSR_RDATA_RM_S1);
        wbk_inst_s1.CSR_WENABLE_RM_S1(CSR_WENABLE_RM_S1);

        wbk_inst_s1.PC_MEM2WBK_RM_S1(PC_MEM2WBK_RM_S1);

        wbk_inst_s1.INTERRUPTION_SE_S1(INTERRUPTION_SE_S1);
        wbk_inst_s1.CURRENT_MODE_SM_S1(CURRENT_MODE_SM_S1);
        
        wbk_inst_s1.MULT_INST_RM_S1(MULT_INST_RM_S1);
        wbk_inst_s1.X2_RES_RX2(multiplier_out_sx2);

        wbk_inst_s1.CLK(CLK);
        wbk_inst_s1.RESET(RESET);