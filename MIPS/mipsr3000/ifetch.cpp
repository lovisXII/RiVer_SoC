#include "ifetch.h"

void ifetch::processBDSLOT_XI()
{
  BDSLOT_XI.write(I_BRNCH_SD.read());
}

void ifetch::processSR_SI()
{
  sc_uint<32> nextsr_rx=NEXTSR_RX.read();
  
  if (((int)OPCOD_SD.read() == rfe_i) || ((int)OPCOD_RD.read() == rfe_i) ||
     ((int)OPCOD_RE.read() == rfe_i))
    SR_SI.write(nextsr_rx.range(31,4) 
			<< 4 | nextsr_rx.range(5,2));
  else
    SR_SI.write(nextsr_rx);
}
