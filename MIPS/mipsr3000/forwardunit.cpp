#include "forwardunit.h"


void forwardunit::processCMP_SDE_SD()
{
  CMP_SDE_SD.write(RS_SD.read() ^ RD_RD.read());
}

void forwardunit::processCMP_SDM_SD()
{
  CMP_SDM_SD.write(RS_SD.read() ^ RD_RE.read());
}

void forwardunit::processCMP_SDW_SD()
{
  CMP_SDW_SD.write(RS_SD.read() ^ RD_RM.read());
}

void forwardunit::processCMP_TDE_SD()
{
  CMP_TDE_SD.write(RT_SD.read() ^ RD_RD.read());
}

void forwardunit::processCMP_TDM_SD()
{
  CMP_TDM_SD.write(RT_SD.read() ^ RD_RE.read());
}

void forwardunit::processCMP_TDW_SD()
{
  CMP_TDW_SD.write(RT_SD.read() ^ RD_RM.read());
}

void forwardunit::processSREADR0_SD()
{
  SREADR0_SD.write((RS_SD.read() == 0x0) ? 1 : 0) ;
}

void forwardunit::processTREADR0_SD()
{
  TREADR0_SD.write((RT_SD.read() == 0x0) ? 1 : 0) ;
}

void forwardunit::processHZD_SDE_SD()
{
  HZD_SDE_SD.write(((CMP_SDE_SD.read() == 0x0) && (SREADR0_SD.read() == 0) &&
                   (I_READS_SD.read() == 1)   && ( I_WRITE_SE.read() ==1))? 1 : 0);
}

void forwardunit::processHZD_SDM_SD()
{
  HZD_SDM_SD.write(((CMP_SDM_SD.read() == 0x0) && (SREADR0_SD.read() == 0) &&
                   (I_READS_SD.read() == 1)   && ( I_WRITE_SM.read() ==1))? 1 : 0);
}

void forwardunit::processHZD_SDW_SD()
{
  HZD_SDW_SD.write(((CMP_SDW_SD.read() == 0x0) && (SREADR0_SD.read() == 0) &&
                   (I_READS_SD.read() == 1)   && ( I_WRITE_SW.read() ==1))? 1 : 0);
}

void forwardunit::processHZD_TDE_SD()
{
  HZD_TDE_SD.write(((CMP_TDE_SD.read() == 0x0) && (TREADR0_SD.read() == 0) &&
                   (I_READT_SD.read() == 1)   && ( I_WRITE_SE.read() ==1))? 1 : 0);
}

void forwardunit::processHZD_TDM_SD()
{
  HZD_TDM_SD.write(((CMP_TDM_SD.read() == 0x0) && (TREADR0_SD.read() == 0) &&
                   (I_READT_SD.read() == 1)   && ( I_WRITE_SM.read() ==1))? 1 : 0);
}

void forwardunit::processHZD_TDW_SD()
{
  HZD_TDW_SD.write(((CMP_TDW_SD.read() == 0x0) && (TREADR0_SD.read() == 0) &&
                   (I_READT_SD.read() == 1)   && ( I_WRITE_SW.read() ==1))? 1 : 0);
}

void forwardunit::processHAZARDS_SD()
{
  HAZARDS_SD.write(((HZD_SDE_SD.read() | HZD_TDE_SD.read()) & I_DUSE_SD.read() & I_EPDC_SE.read() ) |
				  ((HZD_SDE_SD.read() | HZD_TDE_SD.read()) & I_DUSE_SD.read() & I_MPDC_SE.read() ) |
				  ((HZD_SDM_SD.read() | HZD_TDM_SD.read()) & I_DUSE_SD.read() & I_MPDC_SM.read() )) ;
}

void forwardunit::processSOPER_SD()
{
  if (HZD_SDM_SD.read() == 1)
    SOPER_SD.write(RES_RE.read());
  else if (HZD_SDW_SD.read() == 1)
    SOPER_SD.write(DATA_RM.read());
  else
    SOPER_SD.write(S_SD.read());
}

void forwardunit::processTOPER_SD()
{
  if (HZD_TDM_SD.read() == 1)
    TOPER_SD.write(RES_RE.read());
  else if (HZD_TDW_SD.read() == 1)
    TOPER_SD.write(DATA_RM.read());
  else
    TOPER_SD.write(T_SD.read());
}

void forwardunit::processCMP_SDM_SE()
{
  CMP_SDM_SE.write(RS_RD.read() ^ RD_RE.read());
}

void forwardunit::processCMP_SDW_SE()
{
  CMP_SDW_SE.write(RS_RD.read() ^ RD_RM.read());
}

void forwardunit::processCMP_TDM_SE()
{
  CMP_TDM_SE.write(RT_RD.read() ^ RD_RE.read());
}

void forwardunit::processCMP_TDW_SE()
{
  CMP_TDW_SE.write(RT_RD.read() ^ RD_RM.read());
}

void forwardunit::processSREADR0_SE()
{
  SREADR0_SE.write((RS_RD.read() == 0x0) ? 1 : 0);
}

void forwardunit::processTREADR0_SE()
{
  TREADR0_SE.write((RT_RD.read() == 0x0) ? 1 : 0);
}

void forwardunit::processHZD_SDM_SE()
{
  HZD_SDM_SE.write(((CMP_SDM_SE.read() == 0x0) && (SREADR0_SE.read() == 0) &&
                   (I_READS_SE.read() == 1)   && ( I_WRITE_SM.read() ==1))? 1 : 0);
}

void forwardunit::processHZD_SDW_SE()
{
  HZD_SDW_SE.write(((CMP_SDW_SE.read() == 0x0) && (SREADR0_SE.read() == 0) &&
                   (I_READS_SE.read() == 1)   && ( I_WRITE_SW.read() ==1))? 1 : 0);
}

void forwardunit::processHZD_TDM_SE()
{
  HZD_TDM_SE.write(((CMP_TDM_SE.read() == 0x0) && (TREADR0_SE.read() == 0) &&
                   (I_READT_SE.read() == 1)   && ( I_WRITE_SM.read() ==1))? 1 : 0);
}

void forwardunit::processHZD_TDW_SE()
{
  HZD_TDW_SE.write(((CMP_TDW_SE.read() == 0x0) && (TREADR0_SE.read() == 0) &&
                   (I_READT_SE.read() == 1)   && ( I_WRITE_SW.read() ==1))? 1 : 0);
}

void forwardunit::processHAZARDS_SE()
{
  HAZARDS_SE.write((HZD_SDM_SE.read() | HZD_TDM_SE.read()) & I_EUSE_SE.read() & I_MPDC_SM.read()) ;
}

void forwardunit::processX_SE()
{
  if (HZD_SDM_SE.read() == 1)
    X_SE.write(RES_RE.read());
  else if (HZD_SDW_SE.read() == 1)
    X_SE.write(DATA_RM.read());
  else
    X_SE.write(SOPER_RD.read());
}

void forwardunit::processY_SE()
{
  if (HZD_TDM_SE.read() == 1)
    Y_SE.write(RES_RE.read());
  else if (HZD_TDW_SE.read() == 1)
    Y_SE.write(DATA_RM.read());
  else
    Y_SE.write(TOPER_RD.read());
}
