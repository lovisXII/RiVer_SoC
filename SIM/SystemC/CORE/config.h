#ifndef CONFIG_CORE
#define CONFIG_CORE

//CACHES config
//#define ICACHE_ON
//#define DCACHE_ON

//ifetch config
#define nop_encoding                0x0000013
#define if2dec_size                 97

//Branch prediction config
#define BRANCH_PREDICTION
#define RET_BRANCH_PREDICTION

#define predictor_register_size     128
#define size_of_pred_pointer        7          //2^n = predictor_register_size

#define ret_predictor_register_size 16
#define ret_predictor_pointer_size  4

#define ret_stack_size              16

//decode config
#define dec2exe_size                252
#define dec2if_size                 134

//exe config        
#define exe2mem_size                200

//mem config
#define mem2wbk_size                108

//kernel address config
#define start_kernel_adress         0xF0000000

#endif