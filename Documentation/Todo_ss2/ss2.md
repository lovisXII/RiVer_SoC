# Introduction

This document will help for ss2 implementation

# How does ss2 will works :

* DEC send 2 à to IFETCH : PC+4; PC+8
* DEC gets 2 inst from IFETCH
* DEC decods those 2 inst, meaning :
    * we have 2 signals telling which inst will go in which pipeline
    * we'll have :
        * radr1_s1 : @ sent to REG to get the data for rs1_s1
        * radr2_s1 : @ sent to REG to get the data for rs2_s1
        * radr1_s2 : @ sent to REG to get the data for rs1_s2
        * radr2_s2 : @ sent to REG to get the data for rs2_s2