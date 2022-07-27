.section .text
.global _start

_start:
    ori x1, x0, 3
    ori x2, x0, 10

    ori x3, x0, 20
    addi x1, x1, 7  # x1=10

    bne x1, x2, _bad
    add x1, x1, x2 # x1 = 20

    bne x1, x3, _bad
    slti x1, x0, -3 # x0 < -3 -> x1 =0

    bne  x0, x1, _bad
    sltiu x1, x0, -3 # x0 < 3 -> x1 = 1

    beq  x0, x1, _bad
    slti x1, x0, 3  # x0 < 3 -> x1 = 1

    beq  x0, x1, _bad
    xori x1, x2, 10   # x1 = 10

    bne  x0, x1, _bad
    andi x1, x2, 10 # x1 = 10

    bne  x2, x1, _bad
    slli x1, x2, 1  # x1 = 20

    bne  x1, x3, _bad
    ori  x4, x0, -20    # x4 = -20 // 4294967276

    ori  x5, x0, -10    # x4 = -10
    srai x1, x4, 1      # x1 = 	-10 // 4294967286

    bne  x1, x5, _bad
    ori  x6, x0, 1  # x6 = 1

    slli x6, x6, 29 # x6 = 536870912
    srli x1, x4, 1 # x1 = -28 // 4294967268

    blt  x1, x6, _bad
    j _good

    nop
    nop

