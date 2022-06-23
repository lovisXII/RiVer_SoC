# Timer documentation

## Timer registers 
- 0x5C0 is the timer config register
- 0x5C1 is the timer divider register

both can only be written to, and a read will return 0.

### Timer config
Currently, only the first two bits on the register are used. 

- bit 0 : timer enable, whether timer interrupts will be fired. Default : 0
- bit 1 : repeat mode, whether the timer will restart after an interrupt, or stop. 