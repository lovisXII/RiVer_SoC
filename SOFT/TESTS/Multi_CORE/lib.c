#include "lib.h"

int get_proc_id()
{
    int id;
    __asm__ __volatile__("csrr %0, mhartid" : "=r"(id));
    return id;
}
void lock_proc()
{
    int id = get_proc_id();

    switch (id)
    {
    case 1:    // proc 1
        __asm__ __volatile__("la x21, _lock_proc1\n"
                             "sw x0, 0(x21)");
        break;
    default:   // proc 0
        __asm__ __volatile__("la x21, _lock_proc0\n"
                             "sw x0, 0(x21)");
        break;
    }
}
void unlock_proc()
{
    int id = get_proc_id();

    switch (id)
    {
    case 1:    // proc 1
        __asm__ __volatile__("la x21, _lock_proc1\n"
                             "li x22, 1\n"
                             "sw x22, 0(x21)");
        break;
    default:   // proc 0
        __asm__ __volatile__("la x21, _lock_proc0\n"
                             "li x22, 1\n"
                             "sw x22, 0(x21)");
        break;
    }
}
int check_proc_lock_state(int id)
{
    int state;
    switch (id)
    {
    case 1:    // proc 1
        __asm__ __volatile__("la x21, _lock_proc1\n"
                             "lw %0, 0(x21)" : "=r"(state));
        break;
    default:   // proc 0
        __asm__ __volatile__("la x21, _lock_proc0\n"
                             "lw %0, 0(x21)" : "=r"(state));
        break;
    }
    return state;
}
void wait(int times)
{
    int nb = times*NB_INST_FOR_SECOND;
    for(int i = 0; i < nb; ++i)
        __asm__ __volatile__("nop");
}
void wait_until_end()
{   
    int counter = 0;
    while(1)
    {
        if(check_proc_lock_state(0)
           && check_proc_lock_state(1))
            break;
        if(counter >= MAX_WAIT_INST)
            _bad();
        counter++;
        wait(1);
    }
    _good();
}