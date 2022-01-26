#include "ff_misc.h"


void ff_misc::processClock()
{
// ### ------------------------------------------------------ ###
// #   assign the the coprocessor zero's registers :		#
// #     the Cause register					#
// ### ------------------------------------------------------ ###

  CAUSE_RX.write(MUX_CAUSE_SX.read());

// ### ------------------------------------------------------ ###
// #   assign the the coprocessor zero's registers :		#
// #     the Exception Program Counter register			#
// ### ------------------------------------------------------ ###

  EPC_RX.write(MUX_EPC_SX.read());

// ### ------------------------------------------------------ ###
// #   assign the the coprocessor zero's registers :		#
// #     the Status register					#
// ### ------------------------------------------------------ ###

  NEXTSR_RX.write(MUX_NEXTSR_SX.read());

// ### ------------------------------------------------------ ###
// #   assign registers (those directly controled by hardware)	#
// ### ------------------------------------------------------ ###

  RESET_RX.write(MUX_RESET_SX.read());
}
