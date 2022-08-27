#include "lib.h"

int main()
{
    lock_proc();

    if(get_proc_id())
        function_1();
    else
        function_0();

    unlock_proc();

    wait_until_end();
}
int function_1()
{
    return get_proc_id();
}
int function_0()
{
    return !get_proc_id();
}