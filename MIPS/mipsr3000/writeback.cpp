#include "writeback.h"

void writeback::processI_WRITE_SW()
{
  sc_uint<25> i_type_rm=I_TYPE_RM.read();

  I_WRITE_SW.write(i_type_rm[8] | i_type_rm[7]);
}

void writeback::processWLO_SW()
{
  if ((int)OPCOD_RM.read()==mtlo_i)
    WLO_SW.write(1);
  else
    WLO_SW.write(0);
}

void writeback::processWHI_SW()
{
  if ((int)OPCOD_RM.read()==mthi_i)
    WHI_SW.write(1);
  else
    WHI_SW.write(0);
}
