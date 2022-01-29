
;  ###----------------------------------------------------------------###
;  # file	: add000.r						#
;  # date	: Mar 26 1996						#
;  # descr.	: functional test for Mips R3000			#
;  ###----------------------------------------------------------------###

system_stack	.equ	0xc0000000

reset_sw_int	.equ	0x00000000

user_status	.equ	0x0000ff3c
user_prog	.equ	0x00400000

		.org	0xbfc00000

	;  ###--------------------------------------------------------###
	;  #   initialization prgram (hardware RESET)			#
	;  ###--------------------------------------------------------###

hardware_reset:
		loadi	r30, system_stack	; init system stack pointer

		loadi	r1 , user_status
		mtc0	r1 , status

		loadi	r1 , reset_sw_int
		mtc0	r1 , cause

		loadi	r26, user_prog

		jr	r26
		rfe				; return from reset

		.end

