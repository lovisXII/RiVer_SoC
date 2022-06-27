
/**********************************************************
        File : 
        Author : 
        Date : 
**********************************************************/

/* Grouping sections into segments for the link editor. */


SECTIONS
{
    . = 0x10054 ;
    seg_text :
    {
        *(.text)
    }
    . = 0x110054 ;
    seg_result :
    {
        *(.result)
    }
    . = 0x80000000 ;
    seg_reset :
    {
        *(.reset)
    }
    . = 0x81000000 ;
    seg_kernel :
    {
        *(.kernel) 
    }
}

INPUT(exception.o)
