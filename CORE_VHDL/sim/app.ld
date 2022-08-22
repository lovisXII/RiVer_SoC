
/**********************************************************
        File : 
        Author : 
        Date : 
**********************************************************/

/* Grouping sections into segments for the link editor. */


SECTIONS
{
    . = 0x00000000 ;
    seg_text :
    {
        *(.text)
    }

}

