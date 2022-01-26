/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  systemc.h - Top-level include file for the SystemC library.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SYSTEMC_H
#define SYSTEMC_H


// include this file first
#include "systemc/kernel/sc_cmnhdr.h"

#include "systemc/kernel/sc_externs.h"
#include "systemc/kernel/sc_module.h"
#include "systemc/kernel/sc_process_b.h"
#include "systemc/kernel/sc_simcontext.h"
#include "systemc/kernel/sc_ver.h"

#include "systemc/communication/sc_buffer.h"
#include "systemc/communication/sc_clock.h"
#include "systemc/communication/sc_clock_ports.h"
#include "systemc/communication/sc_fifo.h"
#include "systemc/communication/sc_fifo_ports.h"
#include "systemc/communication/sc_mutex.h"
#include "systemc/communication/sc_semaphore.h"
#include "systemc/communication/sc_signal.h"
#include "systemc/communication/sc_signal_ports.h"
#include "systemc/communication/sc_signal_resolved.h"
#include "systemc/communication/sc_signal_resolved_ports.h"
#include "systemc/communication/sc_signal_rv.h"
#include "systemc/communication/sc_signal_rv_ports.h"

#include "systemc/datatypes/bit/sc_bit.h"
#include "systemc/datatypes/bit/sc_logic.h"
#include "systemc/datatypes/bit/sc_bv.h"
#include "systemc/datatypes/bit/sc_lv.h"

using sc_dt::sc_bit;
using sc_dt::sc_logic;
using sc_dt::sc_bv_base;
using sc_dt::sc_bv;
using sc_dt::sc_lv_base;
using sc_dt::sc_lv;

using sc_dt::SC_LOGIC_0;
using sc_dt::SC_LOGIC_1;
using sc_dt::SC_LOGIC_Z;
using sc_dt::SC_LOGIC_X;
// #ifdef SC_DT_DEPRECATED
using sc_dt::sc_logic_0;
using sc_dt::sc_logic_1;
using sc_dt::sc_logic_Z;
using sc_dt::sc_logic_X;
// #endif

#include "systemc/datatypes/int/sc_bigint.h"
#include "systemc/datatypes/int/sc_biguint.h"
#include "systemc/datatypes/int/sc_int.h"
#include "systemc/datatypes/int/sc_uint.h"

using sc_dt::sc_length_param;
using sc_dt::sc_length_context;
using sc_dt::sc_signed;
using sc_dt::sc_bigint;
using sc_dt::sc_unsigned;
using sc_dt::sc_biguint;
using sc_dt::sc_int_base;
using sc_dt::sc_int;
using sc_dt::sc_uint_base;
using sc_dt::sc_uint;

#ifdef SC_INCLUDE_FX

#include "systemc/datatypes/fx/fx.h"

using sc_dt::sc_fxnum;
using sc_dt::sc_fxnum_fast;
using sc_dt::sc_fix;
using sc_dt::sc_fix_fast;
using sc_dt::sc_ufix;
using sc_dt::sc_ufix_fast;
using sc_dt::sc_fixed;
using sc_dt::sc_fixed_fast;
using sc_dt::sc_ufixed;
using sc_dt::sc_ufixed_fast;
using sc_dt::sc_fxval;
using sc_dt::sc_fxval_fast;
using sc_dt::sc_fxcast_switch;
using sc_dt::sc_fxcast_context;
using sc_dt::sc_fxtype_params;
using sc_dt::sc_fxtype_context;
using sc_dt::sc_q_mode;
using sc_dt::SC_RND;
using sc_dt::SC_RND_ZERO;
using sc_dt::SC_RND_MIN_INF;
using sc_dt::SC_RND_INF;
using sc_dt::SC_RND_CONV;
using sc_dt::SC_TRN;
using sc_dt::SC_TRN_ZERO;
using sc_dt::sc_o_mode;
using sc_dt::SC_SAT;
using sc_dt::SC_SAT_ZERO;
using sc_dt::SC_SAT_SYM;
using sc_dt::SC_WRAP;
using sc_dt::SC_WRAP_SM;
using sc_dt::sc_switch;
using sc_dt::SC_OFF;
using sc_dt::SC_ON;
using sc_dt::sc_fmt;
using sc_dt::SC_F;
using sc_dt::SC_E;
using sc_dt::sc_context_begin;
using sc_dt::SC_NOW;
using sc_dt::SC_LATER;

#endif

#include "systemc/tracing/sc_vcd_trace.h"
#include "systemc/tracing/sc_wif_trace.h"
#include "systemc/tracing/sc_isdb_trace.h"

#include "systemc/utils/sc_exception.h"
#include "systemc/utils/sc_iostream.h"


#ifdef _MSC_VER
using sc_dt::equal;
using sc_dt::not_equal;
using sc_dt::b_not;
using sc_dt::b_and;
using sc_dt::b_or;
using sc_dt::b_xor;
using sc_dt::lrotate;
using sc_dt::rrotate;
using sc_dt::reverse;
using sc_dt::concat;
using sc_dt::and_reduce;
using sc_dt::or_reduce;
using sc_dt::xor_reduce;
using sc_dt::nand_reduce;
using sc_dt::nor_reduce;
using sc_dt::xnor_reduce;
#endif


#endif
