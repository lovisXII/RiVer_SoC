
1. AGREEMENT FOR INPUT, OUTPUT or INNER SIGNALS DECLARATION

The agreement for declare a wire is compound by a name or 
an acronym, and a prefix connected by an underscore.

The prefix is compound by 2 capital letters, 
the wire type and the stage.

TYPES:

S: signal
R: registre


STAGE:

I: ifetch  
D: decod  
E: exec  
M: memory  
W: writeback

EXAMPLE:

    "RES_SE"

    name  :  resultat   
    type  :  signal
    stage :  exec


// CLZ: count leading zero

2. STAGE AND COMPONENTS DESCRIPTION

2.1 ALU

The ALU are made for make logique operations (AND, OR, NOR and XOR),
arithmetique operations (addition and substraction), shift operations and slt compare operation.

