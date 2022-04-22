
/**********************************************************
        File : 
        Author : 
        Date : 
**********************************************************/

/* Grouping sections into segments for the link editor. */


SECTIONS
{
    . = 0x1004C ;
    seg_bad_good :
    {
        *(.good_bad)
    }
    . = 0x10054 ;
    seg_text :
    {
        *(.text)
    }
    . = 0x80000000 ;
    seg_kernel :
    {
        *(.kernel) 
    }
    . = 0x80000000 ;
    seg_irq_vector :
    {
        *(.irq_vector)
    }
}

INPUT(tests/exception)
