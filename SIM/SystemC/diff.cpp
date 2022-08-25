Common subdirectories: CORE/CACHES and /home/lovis12/Desktop/RiVer_SoC/CORE/CACHES
diff CORE/config.h /home/lovis12/Desktop/RiVer_SoC/CORE/config.h
13,14c13,14
< //#define BRANCH_PREDICTION
< //#define RET_BRANCH_PREDICTION
---
> #define BRANCH_PREDICTION
> #define RET_BRANCH_PREDICTION
25c25
< #define dec2exe_size                253
---
> #define dec2exe_size                252
diff CORE/core.h /home/lovis12/Desktop/RiVer_SoC/CORE/core.h
100c100
<     sc_signal<sc_uint<32>> KERNEL_ADR_SC;
---
> 
103d102
<     sc_signal<bool> CSRRC_I_RD;
113c112
<     sc_signal<sc_uint<6>> EXE_DEST_RD;
---
>     sc_signal<sc_uint<6>> EXE_DEST_SD;
215c214
<     sc_signal<bool>        CSR_ENABLE_SM;
---
>     sc_signal<bool>        CSR_ENABLE_BEFORE_FIFO_SM;
387,388c386
<         dec_inst.EXE_DEST_RD(EXE_DEST_RD);
<         dec_inst.KERNEL_ADR_SC(KERNEL_ADR_SC);
---
>         dec_inst.EXE_DEST_SD(EXE_DEST_SD);
394,399c392,397
<         dec_inst.DEST_RE(DEST_RE);
<         dec_inst.EXE_RES_RE(EXE_RES_RE);
<         dec_inst.DEST_RM(DEST_RM);
<         dec_inst.MEM_RES_RM(MEM_RES_RM);
<         dec_inst.EXE2MEM_EMPTY_SE(EXE2MEM_EMPTY_SE);
<         dec_inst.MEM2WBK_EMPTY_SM(MEM2WBK_EMPTY_SM);
---
>         dec_inst.BP_DEST_RE(DEST_RE);
>         dec_inst.BP_EXE_RES_RE(EXE_RES_RE);
>         dec_inst.BP_DEST_RM(DEST_RM);
>         dec_inst.BP_MEM_RES_RM(MEM_RES_RM);
>         dec_inst.BP_EXE2MEM_EMPTY_SE(EXE2MEM_EMPTY_SE);
>         dec_inst.BP_MEM2WBK_EMPTY_SM(MEM2WBK_EMPTY_SM);
414c412
<         dec_inst.MEM_LOAD_RE(MEM_LOAD_RE);
---
>         dec_inst.BP_MEM_LOAD_RE(MEM_LOAD_RE);
418d415
<         dec_inst.CSRRC_I_RD(CSRRC_I_RD);
463c460
<         exec_inst.DEST_RD(EXE_DEST_RD);
---
>         exec_inst.DEST_RD(EXE_DEST_SD);
488d484
<         exec_inst.CSRRC_I_RD(CSRRC_I_RD);
502d497
<         exec_inst.KERNEL_ADR_SC(KERNEL_ADR_SC);
555c550
<         exec_inst.MEM2WBK_EMPTY_SM(MEM2WBK_EMPTY_SM);
---
>         exec_inst.BP_MEM2WBK_EMPTY_SM(MEM2WBK_EMPTY_SM);
668c663
<         mem_inst.CSR_ENABLE_SM(CSR_ENABLE_SM);
---
>         mem_inst.CSR_ENABLE_BEFORE_FIFO_SM(CSR_ENABLE_BEFORE_FIFO_SM);
743c738
<         csr_inst.CSR_ENABLE_SM(CSR_ENABLE_SM);
---
>         csr_inst.CSR_ENABLE_BEFORE_FIFO_SM(CSR_ENABLE_BEFORE_FIFO_SM);
765d759
<         csr_inst.KERNEL_ADR_SC(KERNEL_ADR_SC);
diff CORE/core_tb.cpp /home/lovis12/Desktop/RiVer_SoC/CORE/core_tb.cpp
19c19
< #include "CACHES/icache.h"ios::out
---
> #include "CACHES/icache.h"
58d57
<     string                  test_filename(argv[1]);
69,72d67
<     int                     rvtest_end;
< 
<     fstream test_stats;
<     string filename_stats;
79,93c74,75
< /*
<     ##############################################################
<                     PARSING ELF/.s/.c file 
<     ##############################################################
< */
<     if(argc == 2 && std::string(argv[1]) == "--help"){
<         cout << endl << endl;
<         cout << "Usage: ./core_tb test_filename [options] ..." << endl;
<         cout << "Options:" << endl << endl;
<         cout << "-O                          \t Optimise the .c file" << endl;
<         cout << "--riscof signature_filename \t Allow to enable the riscof gestion and store the signature in the file named signature_filename" << endl ;
<         cout << "--stats                     \t Allow to use the statistic such as the number of cycle needed to end the program" << endl;
<         exit(0);
<     }
<     else if (argc >= 3 && std::string(argv[2]) == "-O") {
---
> 
>     if (argc >= 3 && std::string(argv[2]) == "-O") {
95,96c77
<     } 
<     else if (argc >= 3 && std::string(argv[2]) == "--riscof") {
---
>     } else if (argc >= 3 && std::string(argv[2]) == "--riscof") {
99,128c80
<         stats          = true;
< 
<         int tmp = test_filename.find("src/");
<         int tmp2 = test_filename.find("dut/");
< 
<         string tempo_string = test_filename.substr(0,tmp);
<         string tempo_string2 = test_filename.substr(0,tmp2);
<         int tmp3 = tempo_string2.size() - (tempo_string.size() + 4);
< 
<         test_filename = test_filename.substr(tmp+4, tmp3);
<         
<         #ifdef BRANCH_PREDICTION 
<         filename_stats = "stats_branch_SS1.txt";        
<         #elif  RET_BRANCH_PREDICTION
<         filename_stats = "stats_stack_branch_SS1.txt";        
<         #elif BRANCH_PREDICTION & RET_BRANCH_PREDICTION
<         filename_stats = "stat_all_branch_SS1.txt";
<         #elif ICACHE_ON & DCACHE_ON
<         filename_stats = "stats_caches_SS1.txt";
<         #else
<         filename_stats = "test_stats_SS1.txt";
<         #endif
<         test_stats.open(filename_stats, fstream::app);
<         if(!test_stats.is_open())
<         {
<             cout << "Impossible to open " << filename_stats << endl ;
<             exit(1);
<         }
<     } 
<     else if(argc >= 3 && std::string(argv[2]) == "--stats")
---
>     } else if(argc >= 3 && std::string(argv[2]) == "--stats")
131,150d82
<         int tmp = test_filename.rfind("/");
<         test_filename = test_filename.substr(tmp+1, test_filename.size());
<         
<         #ifdef BRANCH_PREDICTION 
<         filename_stats = "stats_branch_SS1.txt";        
<         #elif  RET_BRANCH_PREDICTION
<         filename_stats = "stats_stack_branch_SS1.txt";        
<         #elif BRANCH_PREDICTION & RET_BRANCH_PREDICTION
<         filename_stats = "stat_all_branch_SS1.txt";
<         #elif ICACHE_ON & DCACHE_ON
<         filename_stats = "stats_caches_SS1.txt";
<         #else
<         filename_stats = "test_stats_SS1.txt";
<         #endif
<         test_stats.open(filename_stats, fstream::app);
<         if(!test_stats.is_open())
<         {
<             cout << "Impossible to open " << filename_stats << endl ;
<             exit(1);
<         }
154c86
<     if (path.substr(path.find_last_of(".") + 1) == "s") {  
---
>     if (path.substr(path.find_last_of(".") + 1) == "s") {  // checking if the argument is a assembly file
158c90
<                 "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T ../../../SOFT/app.ld %s %s",
---
>                 "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T ../SW/app.ld %s %s",
160,163c92,95
<                 path.c_str());  
<                                 
<         system((char*)temp);    
<         path = "a.out";         
---
>                 path.c_str());  // writting "riscv32-unknown-elf-gcc -nostdlib
>                                 // path" in temp
>         system((char*)temp);    // send the command in temp to the terminal
>         path = "a.out";         // give the output
165c97
<     if (path.substr(path.find_last_of(".") + 1) == "c") { 
---
>     if (path.substr(path.find_last_of(".") + 1) == "c") {  // do the same but for .c file
167c99
<         sprintf(temp, "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T ../../../SOFT/app.ld %s %s", opt.c_str(), path.c_str());
---
>         sprintf(temp, "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T ../SW/app.ld %s %s", opt.c_str(), path.c_str());
171c103
<     if (!reader.load(path)) {  
---
>     if (!reader.load(path)) {  // verify if the path is correctly loadkernelle "
179,180d110
<  
<     int n_sec = reader.sections.size();  // get the total amount of sections
182,186c112,124
< /*
<     ##############################################################
<                     PLACING DATA INTO THE RAM 
<     ##############################################################
< */
---
>     /*
>     An Elf binary file consist of segments and sections. Each sections has its
>     own responsability, some contains executable code, others programs
>     data...etc.
>     We need to find out the sections of ELF file. The code below find out theses
>     sections.
>     */
>     // section* text_sec = reader.sections.add( ".section .kernel" );
>     // text_sec->set_type( SHT_PROGBITS );
>     // text_sec->set_flags( SHF_ALLOC | SHF_EXECINSTR );
>     // text_sec->set_address( 0x80000000 );
> 
>     int n_sec = reader.sections.size();  // get the total amount of sections
198c136
<                 ram[adr + j] = data[j / 4]; 
---
>                 ram[adr + j] = data[j / 4];  // put every adress segment in the ram
203,208d140
< /*
<     ##############################################################
<                     LOOKING FOR SECTIONS IN ELF FILE 
<     ##############################################################
< */
< 
258,261d189
<                 if (name == "rvtest_end") {
<                     rvtest_end = value;
<                     cout << "Found rvtest_end at adr " << std::hex << rvtest_end << endl;
<                 }
266,270c194,195
< /*
<     ##############################################################
<                     COMPONENT INSTANCIATION
<     ##############################################################
< */
---
> 
> // Components instanciation
380c305,308
<     // ICache mapios::out
---
>     // ICache map
>     icache_inst.CLK(CLK);
>     icache_inst.RESET_N(RESET);
>     icache_inst.trace(tf);
400c328
<     RESET.write(false);  
---
>     RESET.write(false);  // reset
402,403c330,331
<     sc_start(3, SC_NS);  
<     RESET.write(true);   
---
>     sc_start(3, SC_NS);  // wait for 1 cycle
>     RESET.write(true);   // end of reset
405a334
>     // STATS
407a337,338
>     //
> 
607,612d537
< /*
<     ##############################################################
<                     END OF TEST GESTION
<     ##############################################################
< */
< 
623,627c548,551
<                 #ifdef BRANCH_PREDICTION || RET_BRANCH_PREDICTION
<                     cout << "NB BRANCH TAKEN = "    <<  nb_jump_taken   <<  endl;
<                 #endif
<                 test_stats << test_filename << " " << NB_CYCLES  << " " << "SCALAR" << endl;
<                 test_stats.close();
---
>                 cout <<"#-- STATS -- #"<<endl<<endl;
>                 cout << "NBCYCLES        = "<<std::dec<<NB_CYCLES<<endl;
>                 cout << "NB BRANCH TAKEN = "<<nb_jump_taken<<endl<<endl;
>                 cout <<"#------------#"<<endl;
639c563,564
<         else if (countdown == 0 && ((pc_adr == rvtest_code_end) || (pc_adr ==  rvtest_end) || (signature_name != "" && cycles > 2000000))) {
---
>         else if (countdown == 0 && (pc_adr == rvtest_code_end || (signature_name != "" && cycles > 2000000))) {
>             cerr << "inside if : " << endl ; 
640a566
>             cout << "coutndown value : " << countdown << endl ;
645,650d570
<             
<             // Stats Gestion riscof
<             test_stats << test_filename << " " << NB_CYCLES  << " " << "SCALAR" << endl;
<             test_stats.close();
<             
<             
656c576,580
<            
---
>             
>             int j = 1;
>             for (int i = begin_signature; i < end_signature; i += 4){
>                 j++;
>             }
663,668d586
< /*
<     ##############################################################
<                     MEMORY ACCESS GESTION
<     ##############################################################
< */
< 
737c655
< }
---
>     }
Common subdirectories: CORE/CSR and /home/lovis12/Desktop/RiVer_SoC/CORE/CSR
Common subdirectories: CORE/DEC and /home/lovis12/Desktop/RiVer_SoC/CORE/DEC
Common subdirectories: CORE/EXE and /home/lovis12/Desktop/RiVer_SoC/CORE/EXE
Common subdirectories: CORE/IFETCH and /home/lovis12/Desktop/RiVer_SoC/CORE/IFETCH
Only in CORE: kernel
diff CORE/Makefile /home/lovis12/Desktop/RiVer_SoC/CORE/Makefile
4c4
< INCLUDE_PATH=-I$(SYSTEMC)/include -I../../ELFIO
---
> INCLUDE_PATH=-I$(SYSTEMC)/include -I../ELFIO
13c13
< SW_DIR=../../../SOFT/
---
> SW_DIR=../SW
Common subdirectories: CORE/MEM and /home/lovis12/Desktop/RiVer_SoC/CORE/MEM
Common subdirectories: CORE/REG and /home/lovis12/Desktop/RiVer_SoC/CORE/REG
Only in CORE: riscof_stats.sh
diff CORE/run_all_tests.sh /home/lovis12/Desktop/RiVer_SoC/CORE/run_all_tests.sh
8c8
< for file in $(ls ../../../SOFT/TESTS/I/); do 
---
> for file in $(ls ../TESTS/tests/); do 
11c11
<     timeout 20s ./core_tb ../../../SOFT/TESTS/I/$file >/dev/null 2>&1;
---
>     timeout 20s ./core_tb ../TESTS/tests/$file >/dev/null 2>&1;
23,52c23
<     timeout 20s ./core_tb ../../../SOFT/TESTS/I/$file -O >/dev/null 2>&1
<     RES=$?;
<     
<     if (( $RES == 0 ))
<     then
<         printf "${GREEN} passed\n${NOC}"
<     elif (( $RES == 2 ))
<     then
<         printf "${YELLOW} exception\n${NOC}"
<     else
<         printf "${RED} failed\n${NOC}"
<     fi
< 
< done
< for file in $(ls ../../../SOFT/TESTS/M/); do 
<     printf "Test ${file} non opt..." 
< 
<     timeout 20s ./core_tb ../../../SOFT/TESTS/M/$file >/dev/null 2>&1;
<     RES=$?;
<     if (( $RES == 0 ))
<     then
<         printf "${GREEN} passed\n${NOC}"
<     elif (( $RES == 2 ))
<     then
<         printf "${YELLOW} exception\n${NOC}"
<     else
<         printf "${RED} failed\n${NOC}"
<     fi
<     printf "Test ${file} opt..." 
<     timeout 20s ./core_tb ../../../SOFT/TESTS/M/$file -O >/dev/null 2>&1
---
>     timeout 20s ./core_tb ../TESTS/tests/$file -O >/dev/null 2>&1
Only in CORE: run_stats.sh
Only in /home/lovis12/Desktop/RiVer_SoC/CORE: tests
Common subdirectories: CORE/TIMER and /home/lovis12/Desktop/RiVer_SoC/CORE/TIMER
Common subdirectories: CORE/UTIL and /home/lovis12/Desktop/RiVer_SoC/CORE/UTIL
Common subdirectories: CORE/WBK and /home/lovis12/Desktop/RiVer_SoC/CORE/WBK
