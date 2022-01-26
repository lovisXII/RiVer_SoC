#include "regfile.h"


void regfile::mReadRegs()
{
  int numregA=(int)RRA.read();
  int numregB=(int)RRB.read();
  if (numregA!=0)
    DA.write(registerFile[numregA].read());
  else
    DA.write(0);
  if (numregB!=0)
    DB.write(registerFile[numregB].read());
  else
    DB.write(0);
}

void regfile::mWriteReg()
{
  int numreg=(int)WRC.read();
  if (numreg!=0)
    registerFile[numreg].write(WDATA.read());
}
