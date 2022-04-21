
/**********************************************************
        File : 
        Author : 
        Date : 
**********************************************************/

/* Grouping sections into segments for the link editor. */


SECTIONS
{
    . = 0x0 ;
    seg_text :
    {
        *(.text) 
    }
    . = 0x80000000 ;
    seg_kernel :
    {
        *(.kernel) 
    }
}
