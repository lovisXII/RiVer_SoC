#include "pipeunit.h"


void pipeunit::processKILL_SI()
{
  KILL_SI.write(EXCRQ_XM.read() | INTRQ_XX.read() | RESET_RX.read() | SLEEP_SD.read() );
}

void pipeunit::processSTALL_SI()
{
  STALL_SI.write((!KILL_SI.read()) & (COPY_SD.read() | STALL_SD.read() | I_FRZ.read()));
}

void pipeunit::processCOPY_SI()
{
  COPY_SI.write(0);
}

void pipeunit::processEXEC_SI()
{
  EXEC_SI.write(!(KILL_SI.read() | STALL_SI.read() | COPY_SI.read()));
}

void pipeunit::processKILL_SD()
{
  KILL_SD.write(EXCRQ_XM.read() | INTRQ_XX.read() | RESET_RX.read());
}

void pipeunit::processSTALL_SD()
{
  STALL_SD.write(!(KILL_SD.read()) & (COPY_SE.read() | STALL_SE.read() | HAZARDS_SD.read() | I_FRZ.read()));
}

void pipeunit::processCOPY_SD()
{
  COPY_SD.write(!(KILL_SD.read() | STALL_SD.read()) & SLEEP_SD.read());
}

void pipeunit::processEXEC_SD()
{
  EXEC_SD.write(!(KILL_SD.read() | STALL_SD.read() | COPY_SD.read()));
}

void pipeunit::processKILL_SE()
{
  KILL_SE.write(EXCRQ_XM.read() | INTRQ_XX.read() | RESET_RX.read());
}

void pipeunit::processSTALL_SE()
{
  STALL_SE.write(!KILL_SE.read() & (COPY_SM.read() | STALL_SM.read() | HAZARDS_SE.read()));
}

void pipeunit::processCOPY_SE()
{
  COPY_SE.write(0);
}

void pipeunit::processEXEC_SE()
{
  EXEC_SE.write(!(KILL_SE.read() | STALL_SE.read() | COPY_SE.read()));
}

void pipeunit::processKILL_SM()
{
  KILL_SM.write(EXCRQ_XM.read() | RESET_RX.read());
}

void pipeunit::processSTALL_SM()
{
  STALL_SM.write(!KILL_SM.read() & D_FRZ.read()) ;
}

void pipeunit::processCOPY_SM()
{
  COPY_SM.write(!(KILL_SM.read() | STALL_SM.read() ) & COPYCAP_RE.read());
}

void pipeunit::processEXEC_SM()
{
  EXEC_SM.write(!(KILL_SM.read() or STALL_SM.read() or COPY_SM.read()));
}

void pipeunit::processKILL_SW()
{
  KILL_SW.write(0) ;
}

void pipeunit::processSTALL_SW()
{
  STALL_SW.write(0) ;
}

void pipeunit::processCOPY_SW()
{
  COPY_SW.write(0) ;
}

void pipeunit::processEXEC_SW()
{
  EXEC_SW.write(!(KILL_SW.read() | STALL_SW.read() | COPY_SW.read()));
}

void pipeunit::processBUBBLE_SI()
{
  BUBBLE_SI.write((STALL_SI.read() and EXEC_SD.read()) or (KILL_SI.read() and (EXEC_SD.read() or KILL_SD.read())));
}

void pipeunit::processHOLD_SI()
{
  HOLD_SI.write(STALL_SD.read() |  COPY_SD.read());
}

void pipeunit::processSHIFT_SI()
{
  SHIFT_SI.write(EXEC_SI.read()  |  COPY_SI.read());
}


void pipeunit::processBUBBLE_SD()
{
  BUBBLE_SD.write((STALL_SD.read() & EXEC_SE.read()) | (KILL_SD.read() & (EXEC_SE.read() | KILL_SE.read())));
}

void pipeunit::processHOLD_SD()
{
  HOLD_SD.write(STALL_SE.read() |  COPY_SE.read());
}

void pipeunit::processSHIFT_SD()
{
  SHIFT_SD.write(EXEC_SD.read()  |  COPY_SD.read());
}

void pipeunit::processBUBBLE_SE()
{
  BUBBLE_SE.write((STALL_SE.read() & EXEC_SM.read()) | (KILL_SE.read() & (EXEC_SM.read() | KILL_SM.read())));
}

void pipeunit::processHOLD_SE()
{
  HOLD_SE.write(STALL_SM.read() |  COPY_SM.read());
}

void pipeunit::processSHIFT_SE()
{
  SHIFT_SE.write(EXEC_SE.read()  |  COPY_SE.read());
}

void pipeunit::processBUBBLE_SM()
{
  BUBBLE_SM.write((STALL_SM.read() & EXEC_SW.read()) | (KILL_SM.read() & (EXEC_SW.read() | KILL_SW.read())));
}

void pipeunit::processHOLD_SM()
{
  HOLD_SM.write(STALL_SW.read() |  COPY_SW.read());
}

void pipeunit::processSHIFT_SM()
{
  SHIFT_SM.write(EXEC_SM.read()  |  COPY_SM.read());
}

void pipeunit::processLOAD_SI()
{
  LOAD_SI.write(COPY_SI.read() | EXEC_SI.read()) ;
}

void pipeunit::processKEEP_SI()
{
  KEEP_SI.write(KILL_SI.read() | STALL_SI.read());
}

void pipeunit::processLOAD_SD()
{
  LOAD_SD.write(COPY_SD.read() | EXEC_SD.read()) ;
}

void pipeunit::processKEEP_SD()
{
  KEEP_SD.write(KILL_SD.read() | STALL_SD.read());
}

void pipeunit::processLOAD_SE()
{
  LOAD_SE.write(COPY_SE.read() | EXEC_SE.read()) ;
}

void pipeunit::processKEEP_SE()
{
  KEEP_SE.write(KILL_SE.read() | STALL_SE.read());
}

void pipeunit::processLOAD_SM()
{
  LOAD_SM.write(COPY_SM.read() | EXEC_SM.read()) ;
}

void pipeunit::processKEEP_SM()
{
  KEEP_SM.write(KILL_SM.read() | STALL_SM.read());
}
