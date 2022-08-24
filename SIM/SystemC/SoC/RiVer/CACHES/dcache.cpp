#include "dcache.h"

void dcache::adresse_parcer()
{
  sc_uint<32> DT_A_SM = DATA_ADR_SM.read();
  address_tag.write(DT_A_SM.range(31,11));
  address_index.write(DT_A_SM.range(10,4));
  address_offset.write(DT_A_SM.range(3,0));

  sc_uint<32> DT_A_I = A_O.read();
  mp_address_tag.write(DT_A_I.range(31,11));
  mp_address_index.write(DT_A_I.range(10,4));
  mp_address_offset.write(DT_A_I.range(3,0));

  sc_uint<32> DT_A_BUS = ADR_I.read();
  bus_tag.write(DT_A_BUS.range(31,11));
  bus_index.write(DT_A_BUS.range(10,4));
  bus_offset.write(DT_A_BUS.range(3,0));
}

void dcache::miss_detection()
{
  // COMPARE HIT WAY0
  if(address_tag == w0_TAG[address_index.read()])
  {
    way0_hit = w0_LINE_VALIDATE[address_index.read()];
    if(LOAD_SM.read() && way0_hit)
    {
      DATA_O.write(w0_word[address_index.read()][address_offset.read()/4]);
    }
  }
  else
    way0_hit = false;

  // COMPARE HIT WAY1
  if(address_tag == w1_TAG[address_index.read()])
  {
    way1_hit = w1_LINE_VALIDATE[address_index.read()];
    if(LOAD_SM.read() && way1_hit)
    {
      DATA_O.write(w1_word[address_index.read()][address_offset.read()/4]);
    }
  }
  else
    way1_hit = false;
  
  miss = (!way0_hit) & (!way1_hit);
  STALL_SC.write((miss && LOAD_SM) || (STORE_SM && full));


}
void dcache::new_state()
{
  if(RESET_N)
    current_state = future_state;
  else
    current_state = DC_IDLE;
}
void dcache::state_transition()
{ 
  switch(current_state.read())
  {
    case DC_IDLE:
      if(LOAD_SM.read() && VALID_ADR_SM.read())
      {
        if(miss && !full)
          future_state = DC_WAIT_MEM;
        else
          future_state = DC_IDLE;
      }
      else if(STORE_SM.read() && VALID_ADR_SM.read())
      {
        if(!full)
          future_state = DC_IDLE;
      }
      else
        future_state = DC_IDLE;
    break;
    case DC_WAIT_MEM:
      if(ACK.read())
        future_state = DC_UPDT;
      else
        future_state = DC_WAIT_MEM;
    break;
    case DC_UPDT:
      if(ACK.read())
        future_state = DC_UPDT;
      else
        future_state = DC_IDLE;
    break;
  }

}
void dcache::mae_output()
{
  //SNOOPY
  if(!GRANT.read())
  {
    // COMPARE HIT WAY0
    if(bus_tag == w0_TAG[bus_index.read()])
    {
      w0_LINE_VALIDATE[bus_index.read()] = false;
    }
    // COMPARE HIT WAY1
    if(bus_tag == w1_TAG[bus_index.read()])
    {
      w1_LINE_VALIDATE[bus_index.read()] = false;
    }
  }

  switch(current_state.read())
  {
    case DC_IDLE:
      write_buff = ((LOAD_SM.read() && miss) || STORE_SM.read()) && VALID_ADR_SM.read() && !full;
      DTA_VALID_SC = !empty;
      
      if(STORE_SM.read() && VALID_ADR_SM.read())
      {
        // DATA MASK
        sc_uint<32> dt = DATA_SM;
        sc_uint<32> adr = DATA_ADR_SM;
        sc_uint<32> data_mask;
        int mask = adr & 0x00000003;
        adr_sc = adr & 0xFFFFFFFC;

        if(MEM_SIZE_SM.read() == 2)
        {
          switch(mask)
          {
            case 0:
              data_mask = 0x000000FF;
            break;
            case 1:
              data_mask = 0x0000FF00;
            break;
            case 2:
              data_mask = 0x00FF0000;
            break;
            case 3:
              data_mask = 0xFF000000;
            break;
          }
        }
        else if(MEM_SIZE_SM.read() == 1)
        {
          switch(mask)
          {
            case 0:
              data_mask = 0x0000FFFF;
            break;
            case 1:
              data_mask = 0xFFFF0000;
            break;
          }
        }
        else
        {
          data_mask = 0xFFFFFFFF;
        }
        data_mask_sc = data_mask;

        // STORE BITS WITH MASK

        if(miss)
        {
          if(LRU_bit_check[address_index.read()])
          {
            sc_uint<32> dt = w0_word[address_index.read()][address_offset.read()/4];
            dt = dt & (~data_mask);
            dt = dt | (DATA_SM.read() & data_mask);
            w0_word[address_index.read()][address_offset.read()/4] = dt;
            w0_TAG[address_index.read()] = address_tag.read();
            w0_LINE_VALIDATE[address_index.read()] = true;
          }
          else
          {
            sc_uint<32> dt = w1_word[address_index.read()][address_offset.read()/4];
            dt = dt & (~data_mask);
            dt = dt | (DATA_SM.read() & data_mask);
            w1_word[address_index.read()][address_offset.read()/4] = dt;
            w1_TAG[address_index.read()] = address_tag.read();
            w1_LINE_VALIDATE[address_index.read()] = true;
          }
        }
        else if(way0_hit)
        {
          sc_uint<32> dt = w0_word[address_index.read()][address_offset.read()/4];
          dt = dt & (~data_mask);
          dt = dt | (DATA_SM.read() & data_mask);
          w0_word[address_index.read()][address_offset.read()/4] = dt;
        }
        else if(way1_hit)
        {
          sc_uint<32> dt = w1_word[address_index.read()][address_offset.read()/4];
          dt = dt & (~data_mask);
          dt = dt | (DATA_SM.read() & data_mask);
          w1_word[address_index.read()][address_offset.read()/4] = dt;
        }
      }
      break;
    case DC_WAIT_MEM:
      if(ACK.read())
      {
        DTA_VALID_SC = false;
        sc_uint<32> DT_A_MP = A_O.read();
        if(LRU_bit_check[DT_A_MP.range(10,4)])
        {
          w0_word[DT_A_MP.range(10,4)][burst_cpt++] = DT_I.read();
          w0_TAG[DT_A_MP.range(10,4)] = DT_A_MP.range(31,11);
          w0_LINE_VALIDATE[DT_A_MP.range(10,4)] = true;
        }
        else
        {
          w1_word[DT_A_MP.range(10,4)][burst_cpt++] = DT_I.read();
          w1_TAG[DT_A_MP.range(10,4)] = DT_A_MP.range(31,11);
          w1_LINE_VALIDATE[DT_A_MP.range(10,4)] = true;
        }
        
        mp_last_addr_offset = mp_address_offset;
      }
      else
      {
        DTA_VALID_SC = true;
        burst_cpt = 0;
      }
      write_buff = false;
      break;
    case DC_UPDT:
      if(!ACK.read())
      {
        LRU_bit_check[mp_address_index.read()] = !LRU_bit_check[mp_address_index.read()];
      }
      else
      {
        sc_uint<32> DT_A_MP = A_O.read();
        if(DT_A_MP.range(3,0) == (mp_last_addr_offset.read() + 4))
        {
          mp_last_addr_offset = DT_A_MP.range(3,0);
          if(LRU_bit_check[DT_A_MP.range(10,4)])
          {
            w0_word[DT_A_MP.range(10,4)][burst_cpt++] = DT_I.read();
          }
          else
          {
            w1_word[DT_A_MP.range(10,4)][burst_cpt++] = DT_I.read();
          }
        }
      }
    break;
  }
}
void dcache::trace(sc_trace_file* tf)
{
  sc_trace(tf, CLK, GET_NAME(CLK));
  sc_trace(tf, RESET_N, GET_NAME(RESET_N));

  // interfaz processeur
  sc_trace(tf, DATA_ADR_SM, GET_NAME(DATA_ADR_SM));
  sc_trace(tf, DATA_SM, GET_NAME(DATA_SM));
  sc_trace(tf, LOAD_SM, GET_NAME(LOAD_SM));
  sc_trace(tf, STORE_SM, GET_NAME(STORE_SM));
  sc_trace(tf, VALID_ADR_SM, GET_NAME(VALID_ADR_SM));

  sc_trace(tf, DATA_O, GET_NAME(DATA_O));
  sc_trace(tf, STALL_SC, GET_NAME(STALL_SC));

  // interfaz bus
  sc_trace(tf, READ_SC, GET_NAME(READ_SC)); 
  sc_trace(tf, WRITE_SC, GET_NAME(WRITE_SC));
  sc_trace(tf, DT_O, GET_NAME(DT_O));
  sc_trace(tf, A_O, GET_NAME(A_O));
  sc_trace(tf, DT_I, GET_NAME(DT_I));
  sc_trace(tf, ACK, GET_NAME(ACK));

  // signals
  sc_trace(tf, way0_hit, GET_NAME(way0_hit));
  sc_trace(tf, way1_hit, GET_NAME(way1_hit));
  sc_trace(tf, miss, GET_NAME(miss));

  sc_trace(tf, full, GET_NAME(full));
  sc_trace(tf, empty, GET_NAME(empty));

  sc_trace(tf, current_state, GET_NAME(current_state));
  sc_trace(tf, future_state, GET_NAME(future_state));

  sc_trace(tf, write_buff, GET_NAME(write_buff));

  sc_trace(tf, address_tag, GET_NAME(address_tag));
  sc_trace(tf, address_index, GET_NAME(address_index));
  sc_trace(tf, address_offset, GET_NAME(address_offset));

  sc_trace(tf, mp_address_tag, GET_NAME(mp_address_tag));
  sc_trace(tf, mp_address_index, GET_NAME(mp_address_index));
  sc_trace(tf, MEM_SIZE_SM, GET_NAME(MEM_SIZE_SM));
  sc_trace(tf, SIZE_SC, GET_NAME(SIZE_SC));

  sc_trace(tf, DTA_VALID_SC, GET_NAME(DTA_VALID_SC));

  sc_trace(tf, data_mask_sc, GET_NAME(data_mask_sc));
  sc_trace(tf, adr_sc, GET_NAME(adr_sc));

  for (int i = 0; i < 128; i++) {
    std::string icname = "DCACHE_";
    for(int j = 0; j < 4; j++)
    {
        icname = "D_W0_I_" + std::to_string(i) + "_O_" + std::to_string(j);
        sc_trace(tf, w0_word[i][j], signal_get_name(w0_word[i][j].name(), icname.c_str()));
        icname = "D_W1_I_" + std::to_string(i) + "_O_" + std::to_string(j);
        sc_trace(tf, w1_word[i][j], signal_get_name(w1_word[i][j].name(), icname.c_str()));
    }
    icname = "W0_TAG_" + std::to_string(i);
    sc_trace(tf, w0_TAG[i], signal_get_name(w0_TAG[i].name(), icname.c_str()));
    icname = "W1_TAG_" + std::to_string(i);
    sc_trace(tf, w1_TAG[i], signal_get_name(w1_TAG[i].name(), icname.c_str()));

    icname = "W0_VALID_" + std::to_string(i);
    sc_trace(tf, w0_LINE_VALIDATE[i], signal_get_name(w0_LINE_VALIDATE[i].name(), icname.c_str()));
    icname = "W1_VALID_" + std::to_string(i);
    sc_trace(tf, w1_LINE_VALIDATE[i], signal_get_name(w1_LINE_VALIDATE[i].name(), icname.c_str()));

    icname = "WAY_LRU_" + std::to_string(i);
    sc_trace(tf, LRU_bit_check[i], signal_get_name(LRU_bit_check[i].name(), icname.c_str()));
  }

  buffcache_inst.trace(tf); 
}
