
//number of 'nop' instruction for reach one second
#define NB_INST_FOR_SECOND 1000
//max times that the programme will wait until failed
#define MAX_WAIT_INST 1000

//return processor ID
int get_proc_id();

//lock current processor, put 0 at "_lock_proc" addresse 
void lock_proc();
//unlock current processor, put 1 at "_lock_proc" addresse 
void unlock_proc();

//check processor lock state
int check_proc_lock_state(int);

//wait until all processor are unlocked
void wait_until_end();

//wait '1s'
void wait(int);