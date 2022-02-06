
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

cette alu non seulement fait les operation logique de base (ET, OU, NOR et XOR)
et les addition et substraction, mais aussi il traite les shifts et l'operations slt/i, sltu/i.