        SC_METHOD(dependencies)
        sensitive << adr_dest_sd_s2
                  << RADR1_SD_S2
                  << RADR2_SD_S2 ;
        SC_METHOD(concat_dec2exe_s2)
        sensitive << dec2exe_in_sd_s2 << exe_op1_sd_s2 << exe_op2_sd_s2 << exe_cmd_sd_s2 << exe_neg_op2_sd_s2
                  << exe_wb_sd_s2

                  << mem_data_sd_s2 << mem_load_sd_s2 << mem_store_sd_s2

                  << mem_sign_extend_sd_s2 << mem_size_sd_s2 << select_type_operations_sd_s2 << adr_dest_sd_s2
                  << slti_i_sd_s2 << slt_i_sd_s2

                  << sltiu_i_sd_s2 << sltu_i_sd_s2 << RADR1_SD_S2 << CSR_RDATA_SC_S2 << csr_radr_sd_s2 << RADR2_SD_S2
                  << r1_valid_sd << EXCEPTION_SM << r2_valid_sd << PC_IF2DEC_RI_S2 << csr_wenable_sd_s2
                  << illegal_instruction_sd_s2 << instruction_adress_missaligned_sd_s2 << env_call_m_mode_sd_s2
                  << block_bp_sd_s2 << env_call_s_mode_sd_s2 << env_call_u_mode_sd_s2 << env_call_wrong_mode_s2
                  << mret_i_sd_s2 << instruction_access_fault_sd_s2 << mul_i_sd_s2 << mulh_i_sd_s2 << mulhsu_i_sd_s2
                  << mulhu_i_sd_s2;