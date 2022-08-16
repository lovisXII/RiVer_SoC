SECTIONS
{
    . = 0x10054 ;
    seg_text :
    {
        *(.text)
    }
}

INPUT(reset)
