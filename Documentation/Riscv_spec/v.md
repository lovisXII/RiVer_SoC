# Quick synthese of the riscv v extension

* ELEN : maximum size in bits of a vector element that any operation can produce or consume, ELEN >= 8
* VLEN : number of bits in a single vector (max is 2^16)
* 32 vector registers v0-v31, each one has VLEN bits
* 7 unprivileged CSRs :
    * vstart : 0x008, URW || vector start position
    * vxsat  : 0x009, URW || fixed point saturate flag
    * vxrm   : 0x00A, URW || fixed point saturate mode
    * vcsr   : 0x00F, URW || vector control and status register
    * vtype  : 0xC20, URO || vector length
    * vl     : 0xC21, URO || vector data type register
    * vlenb  : 0xC22, URO || vlen / 8
* VS (vector context status) added to mstatus[10:9] and shadowed in sstatus[10:9]. Analogously to floating point context status field FS

# vtype register

Determine the organisation of elements in each vector registers. Several field in the csr :
* vtype[31]     : vill, illegal value if set 
* vtype[7]      : vma, vector mask agnostic
* vtype[6]      : vta, vector tail agnostic
* vtype[5:3]    : vsew[2:0], selected element width setting
* vtype[2:0]    : vlmul[2:0], vector register group multiplier setting

## vsew

Set the dynamic selected element width (SEW). By default a vector register is viewed as divided into VLEN/SEW elements.

* vsew = 0b000 : SEW = 9
* vsew = 0b001 : SEW = 16
* vsew = 0b010 : SEW = 32
* vsew = 0b011 : SEW = 64
* vsew = 0b1xx : SEW = Reserved

## vlmul

Multiple vector registers can be grouped together, so that a single vector instruction can operate on multiple vector registers.

The vector length multiplier, LMUL, when greater than 1 represents the default number of vector registers that are combined to form a vector register group.
Implementation must support 1, 2, 4 and 8.

## vector length register vl

## vlenb

Holds the value VLEN/8, i.e the vector register length in bytes.