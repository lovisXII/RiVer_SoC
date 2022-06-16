pragma Warnings (Off);
pragma Ada_95;
pragma Source_File_Name (ada_main, Spec_File_Name => "b~ghdl_llvm.ads");
pragma Source_File_Name (ada_main, Body_File_Name => "b~ghdl_llvm.adb");
pragma Suppress (Overflow_Check);
with Ada.Exceptions;

package body ada_main is

   E077 : Short_Integer; pragma Import (Ada, E077, "system__os_lib_E");
   E017 : Short_Integer; pragma Import (Ada, E017, "system__soft_links_E");
   E027 : Short_Integer; pragma Import (Ada, E027, "system__exception_table_E");
   E072 : Short_Integer; pragma Import (Ada, E072, "ada__io_exceptions_E");
   E057 : Short_Integer; pragma Import (Ada, E057, "ada__strings_E");
   E042 : Short_Integer; pragma Import (Ada, E042, "ada__containers_E");
   E029 : Short_Integer; pragma Import (Ada, E029, "system__exceptions_E");
   E019 : Short_Integer; pragma Import (Ada, E019, "system__soft_links__initialize_E");
   E059 : Short_Integer; pragma Import (Ada, E059, "ada__strings__maps_E");
   E063 : Short_Integer; pragma Import (Ada, E063, "ada__strings__maps__constants_E");
   E047 : Short_Integer; pragma Import (Ada, E047, "interfaces__c_E");
   E083 : Short_Integer; pragma Import (Ada, E083, "system__object_reader_E");
   E052 : Short_Integer; pragma Import (Ada, E052, "system__dwarf_lines_E");
   E041 : Short_Integer; pragma Import (Ada, E041, "system__traceback__symbolic_E");
   E103 : Short_Integer; pragma Import (Ada, E103, "ada__tags_E");
   E101 : Short_Integer; pragma Import (Ada, E101, "ada__streams_E");
   E121 : Short_Integer; pragma Import (Ada, E121, "gnat_E");
   E255 : Short_Integer; pragma Import (Ada, E255, "system__aux_dec_E");
   E151 : Short_Integer; pragma Import (Ada, E151, "system__file_control_block_E");
   E150 : Short_Integer; pragma Import (Ada, E150, "system__finalization_root_E");
   E148 : Short_Integer; pragma Import (Ada, E148, "ada__finalization_E");
   E147 : Short_Integer; pragma Import (Ada, E147, "system__file_io_E");
   E215 : Short_Integer; pragma Import (Ada, E215, "system__storage_pools_E");
   E211 : Short_Integer; pragma Import (Ada, E211, "system__finalization_masters_E");
   E209 : Short_Integer; pragma Import (Ada, E209, "system__storage_pools__subpools_E");
   E203 : Short_Integer; pragma Import (Ada, E203, "ada__strings__unbounded_E");
   E116 : Short_Integer; pragma Import (Ada, E116, "ada__calendar_E");
   E120 : Short_Integer; pragma Import (Ada, E120, "ada__calendar__time_zones_E");
   E143 : Short_Integer; pragma Import (Ada, E143, "ada__text_io_E");
   E123 : Short_Integer; pragma Import (Ada, E123, "gnat__directory_operations_E");
   E132 : Short_Integer; pragma Import (Ada, E132, "gnat__secure_hashes_E");
   E134 : Short_Integer; pragma Import (Ada, E134, "gnat__secure_hashes__sha1_E");
   E130 : Short_Integer; pragma Import (Ada, E130, "gnat__sha1_E");
   E155 : Short_Integer; pragma Import (Ada, E155, "system__assertions_E");
   E313 : Short_Integer; pragma Import (Ada, E313, "system__pool_global_E");
   E225 : Short_Integer; pragma Import (Ada, E225, "system__regexp_E");
   E185 : Short_Integer; pragma Import (Ada, E185, "ada__directories_E");
   E527 : Short_Integer; pragma Import (Ada, E527, "system__response_file_E");
   E168 : Short_Integer; pragma Import (Ada, E168, "types_E");
   E236 : Short_Integer; pragma Import (Ada, E236, "areapools_E");
   E110 : Short_Integer; pragma Import (Ada, E110, "dyn_tables_E");
   E238 : Short_Integer; pragma Import (Ada, E238, "elab__memtype_E");
   E181 : Short_Integer; pragma Import (Ada, E181, "flags_E");
   E426 : Short_Integer; pragma Import (Ada, E426, "grt__algos_E");
   E518 : Short_Integer; pragma Import (Ada, E518, "grt__readline_none_E");
   E362 : Short_Integer; pragma Import (Ada, E362, "grt__files_operations_E");
   E444 : Short_Integer; pragma Import (Ada, E444, "grt__to_strings_E");
   E388 : Short_Integer; pragma Import (Ada, E388, "hash_E");
   E386 : Short_Integer; pragma Import (Ada, E386, "dyn_maps_E");
   E422 : Short_Integer; pragma Import (Ada, E422, "dyn_interning_E");
   E461 : Short_Integer; pragma Import (Ada, E461, "interning_E");
   E234 : Short_Integer; pragma Import (Ada, E234, "mutils_E");
   E141 : Short_Integer; pragma Import (Ada, E141, "simple_io_E");
   E139 : Short_Integer; pragma Import (Ada, E139, "logging_E");
   E449 : Short_Integer; pragma Import (Ada, E449, "synth__ieee__std_logic_1164_E");
   E167 : Short_Integer; pragma Import (Ada, E167, "tables_E");
   E259 : Short_Integer; pragma Import (Ada, E259, "flists_E");
   E246 : Short_Integer; pragma Import (Ada, E246, "lists_E");
   E153 : Short_Integer; pragma Import (Ada, E153, "name_table_E");
   E271 : Short_Integer; pragma Import (Ada, E271, "std_names_E");
   E170 : Short_Integer; pragma Import (Ada, E170, "str_table_E");
   E114 : Short_Integer; pragma Import (Ada, E114, "files_map_E");
   E112 : Short_Integer; pragma Import (Ada, E112, "errorout_E");
   E352 : Short_Integer; pragma Import (Ada, E352, "elab__vhdl_errors_E");
   E531 : Short_Integer; pragma Import (Ada, E531, "errorout__console_E");
   E263 : Short_Integer; pragma Import (Ada, E263, "psl__errors_E");
   E265 : Short_Integer; pragma Import (Ada, E265, "psl__nodes_E");
   E267 : Short_Integer; pragma Import (Ada, E267, "psl__hash_E");
   E269 : Short_Integer; pragma Import (Ada, E269, "psl__nodes_meta_E");
   E261 : Short_Integer; pragma Import (Ada, E261, "psl__dump_tree_E");
   E330 : Short_Integer; pragma Import (Ada, E330, "psl__nfas_E");
   E332 : Short_Integer; pragma Import (Ada, E332, "psl__nfas__utils_E");
   E326 : Short_Integer; pragma Import (Ada, E326, "psl__prints_E");
   E324 : Short_Integer; pragma Import (Ada, E324, "psl__cse_E");
   E328 : Short_Integer; pragma Import (Ada, E328, "psl__disp_nfas_E");
   E334 : Short_Integer; pragma Import (Ada, E334, "psl__optimize_E");
   E336 : Short_Integer; pragma Import (Ada, E336, "psl__qm_E");
   E322 : Short_Integer; pragma Import (Ada, E322, "psl__build_E");
   E338 : Short_Integer; pragma Import (Ada, E338, "psl__rewrites_E");
   E455 : Short_Integer; pragma Import (Ada, E455, "psl__subsets_E");
   E401 : Short_Integer; pragma Import (Ada, E401, "synth__errors_E");
   E393 : Short_Integer; pragma Import (Ada, E393, "types_utils_E");
   E384 : Short_Integer; pragma Import (Ada, E384, "netlists_E");
   E391 : Short_Integer; pragma Import (Ada, E391, "netlists__utils_E");
   E399 : Short_Integer; pragma Import (Ada, E399, "netlists__builders_E");
   E414 : Short_Integer; pragma Import (Ada, E414, "netlists__gates_ports_E");
   E420 : Short_Integer; pragma Import (Ada, E420, "netlists__internings_E");
   E434 : Short_Integer; pragma Import (Ada, E434, "netlists__iterators_E");
   E397 : Short_Integer; pragma Import (Ada, E397, "netlists__locations_E");
   E469 : Short_Integer; pragma Import (Ada, E469, "netlists__butils_E");
   E465 : Short_Integer; pragma Import (Ada, E465, "netlists__cleanup_E");
   E418 : Short_Integer; pragma Import (Ada, E418, "netlists__errors_E");
   E395 : Short_Integer; pragma Import (Ada, E395, "netlists__folds_E");
   E412 : Short_Integer; pragma Import (Ada, E412, "netlists__concats_E");
   E416 : Short_Integer; pragma Import (Ada, E416, "netlists__inference_E");
   E424 : Short_Integer; pragma Import (Ada, E424, "netlists__memories_E");
   E467 : Short_Integer; pragma Import (Ada, E467, "netlists__expands_E");
   E410 : Short_Integer; pragma Import (Ada, E410, "synth__environment_E");
   E375 : Short_Integer; pragma Import (Ada, E375, "utils_io_E");
   E432 : Short_Integer; pragma Import (Ada, E432, "netlists__dump_E");
   E558 : Short_Integer; pragma Import (Ada, E558, "netlists__disp_dot_E");
   E560 : Short_Integer; pragma Import (Ada, E560, "netlists__disp_verilog_E");
   E562 : Short_Integer; pragma Import (Ada, E562, "netlists__disp_vhdl_E");
   E430 : Short_Integer; pragma Import (Ada, E430, "synth__environment__debug_E");
   E529 : Short_Integer; pragma Import (Ada, E529, "bug_E");
   E257 : Short_Integer; pragma Import (Ada, E257, "vhdl__flists_E");
   E256 : Short_Integer; pragma Import (Ada, E256, "vhdl__flists_E");
   E244 : Short_Integer; pragma Import (Ada, E244, "vhdl__lists_E");
   E243 : Short_Integer; pragma Import (Ada, E243, "vhdl__lists_E");
   E242 : Short_Integer; pragma Import (Ada, E242, "vhdl__nodes_E");
   E248 : Short_Integer; pragma Import (Ada, E248, "vhdl__nodes_meta_E");
   E232 : Short_Integer; pragma Import (Ada, E232, "elab__vhdl_objtypes_E");
   E360 : Short_Integer; pragma Import (Ada, E360, "elab__vhdl_values_E");
   E382 : Short_Integer; pragma Import (Ada, E382, "elab__vhdl_heap_E");
   E373 : Short_Integer; pragma Import (Ada, E373, "elab__vhdl_values__debug_E");
   E447 : Short_Integer; pragma Import (Ada, E447, "synth__ieee__numeric_std_E");
   E298 : Short_Integer; pragma Import (Ada, E298, "vhdl__elocations_E");
   E300 : Short_Integer; pragma Import (Ada, E300, "vhdl__elocations_meta_E");
   E288 : Short_Integer; pragma Import (Ada, E288, "vhdl__nodes_utils_E");
   E284 : Short_Integer; pragma Import (Ada, E284, "vhdl__errors_E");
   E290 : Short_Integer; pragma Import (Ada, E290, "vhdl__sem_utils_E");
   E286 : Short_Integer; pragma Import (Ada, E286, "vhdl__std_package_E");
   E292 : Short_Integer; pragma Import (Ada, E292, "vhdl__ieee_E");
   E282 : Short_Integer; pragma Import (Ada, E282, "vhdl__utils_E");
   E294 : Short_Integer; pragma Import (Ada, E294, "vhdl__ieee__std_logic_1164_E");
   E438 : Short_Integer; pragma Import (Ada, E438, "synth__source_E");
   E369 : Short_Integer; pragma Import (Ada, E369, "vhdl__annotations_E");
   E358 : Short_Integer; pragma Import (Ada, E358, "elab__vhdl_context_E");
   E371 : Short_Integer; pragma Import (Ada, E371, "elab__vhdl_context__debug_E");
   E274 : Short_Integer; pragma Import (Ada, E274, "vhdl__disp_tree_E");
   E471 : Short_Integer; pragma Import (Ada, E471, "vhdl__ieee__math_real_E");
   E477 : Short_Integer; pragma Import (Ada, E477, "vhdl__ieee__numeric_E");
   E491 : Short_Integer; pragma Import (Ada, E491, "vhdl__ieee__numeric_std_unsigned_E");
   E493 : Short_Integer; pragma Import (Ada, E493, "vhdl__ieee__std_logic_arith_E");
   E495 : Short_Integer; pragma Import (Ada, E495, "vhdl__ieee__std_logic_misc_E");
   E497 : Short_Integer; pragma Import (Ada, E497, "vhdl__ieee__std_logic_unsigned_E");
   E515 : Short_Integer; pragma Import (Ada, E515, "vhdl__nodes_walk_E");
   E507 : Short_Integer; pragma Import (Ada, E507, "vhdl__prints_E");
   E564 : Short_Integer; pragma Import (Ada, E564, "synth__disp_vhdl_E");
   E305 : Short_Integer; pragma Import (Ada, E305, "vhdl__scanner_E");
   E549 : Short_Integer; pragma Import (Ada, E549, "vhdl__formatters_E");
   E481 : Short_Integer; pragma Import (Ada, E481, "vhdl__sem_inst_E");
   E501 : Short_Integer; pragma Import (Ada, E501, "vhdl__sem_scopes_E");
   E505 : Short_Integer; pragma Import (Ada, E505, "vhdl__std_env_E");
   E309 : Short_Integer; pragma Import (Ada, E309, "vhdl__xrefs_E");
   E296 : Short_Integer; pragma Import (Ada, E296, "vhdl__parse_E");
   E302 : Short_Integer; pragma Import (Ada, E302, "vhdl__parse_psl_E");
   E227 : Short_Integer; pragma Import (Ada, E227, "libraries_E");
   E229 : Short_Integer; pragma Import (Ada, E229, "options_E");
   E524 : Short_Integer; pragma Import (Ada, E524, "ghdlmain_E");
   E487 : Short_Integer; pragma Import (Ada, E487, "vhdl__nodes_gc_E");
   E356 : Short_Integer; pragma Import (Ada, E356, "elab__debugger_E");
   E499 : Short_Integer; pragma Import (Ada, E499, "vhdl__ieee__vital_timing_E");
   E489 : Short_Integer; pragma Import (Ada, E489, "vhdl__post_sems_E");
   E485 : Short_Integer; pragma Import (Ada, E485, "vhdl__sem_lib_E");
   E509 : Short_Integer; pragma Import (Ada, E509, "vhdl__sem_psl_E");
   E511 : Short_Integer; pragma Import (Ada, E511, "vhdl__sem_stmts_E");
   E483 : Short_Integer; pragma Import (Ada, E483, "vhdl__sem_names_E");
   E513 : Short_Integer; pragma Import (Ada, E513, "vhdl__sem_types_E");
   E479 : Short_Integer; pragma Import (Ada, E479, "vhdl__sem_decls_E");
   E503 : Short_Integer; pragma Import (Ada, E503, "vhdl__sem_specs_E");
   E342 : Short_Integer; pragma Import (Ada, E342, "vhdl__sem_E");
   E475 : Short_Integer; pragma Import (Ada, E475, "vhdl__sem_expr_E");
   E459 : Short_Integer; pragma Import (Ada, E459, "synth__vhdl_insts_E");
   E463 : Short_Integer; pragma Import (Ada, E463, "synthesis_E");
   E457 : Short_Integer; pragma Import (Ada, E457, "synth__vhdl_decls_E");
   E436 : Short_Integer; pragma Import (Ada, E436, "synth__vhdl_oper_E");
   E380 : Short_Integer; pragma Import (Ada, E380, "synth__vhdl_expr_E");
   E440 : Short_Integer; pragma Import (Ada, E440, "synth__vhdl_eval_E");
   E354 : Short_Integer; pragma Import (Ada, E354, "elab__vhdl_expr_E");
   E522 : Short_Integer; pragma Import (Ada, E522, "elab__vhdl_stmts_E");
   E520 : Short_Integer; pragma Import (Ada, E520, "elab__vhdl_insts_E");
   E350 : Short_Integer; pragma Import (Ada, E350, "elab__vhdl_decls_E");
   E348 : Short_Integer; pragma Import (Ada, E348, "elab__vhdl_types_E");
   E403 : Short_Integer; pragma Import (Ada, E403, "synth__vhdl_aggr_E");
   E346 : Short_Integer; pragma Import (Ada, E346, "vhdl__evaluation_E");
   E344 : Short_Integer; pragma Import (Ada, E344, "vhdl__sem_assocs_E");
   E318 : Short_Integer; pragma Import (Ada, E318, "vhdl__configuration_E");
   E442 : Short_Integer; pragma Import (Ada, E442, "elab__vhdl_files_E");
   E473 : Short_Integer; pragma Import (Ada, E473, "synth__vhdl_static_proc_E");
   E408 : Short_Integer; pragma Import (Ada, E408, "synth__vhdl_environment_E");
   E377 : Short_Integer; pragma Import (Ada, E377, "elab__vhdl_debug_E");
   E320 : Short_Integer; pragma Import (Ada, E320, "vhdl__canon_E");
   E340 : Short_Integer; pragma Import (Ada, E340, "vhdl__canon_psl_E");
   E453 : Short_Integer; pragma Import (Ada, E453, "synth__vhdl_stmts_E");
   E405 : Short_Integer; pragma Import (Ada, E405, "synth__vhdl_context_E");
   E183 : Short_Integer; pragma Import (Ada, E183, "ghdllocal_E");
   E556 : Short_Integer; pragma Import (Ada, E556, "ghdlcomp_E");
   E005 : Short_Integer; pragma Import (Ada, E005, "ghdldrv_E");
   E533 : Short_Integer; pragma Import (Ada, E533, "ghdllib_E");
   E535 : Short_Integer; pragma Import (Ada, E535, "ghdlprint_E");
   E554 : Short_Integer; pragma Import (Ada, E554, "ghdlsynth_E");
   E566 : Short_Integer; pragma Import (Ada, E566, "ghdlvpi_E");
   E568 : Short_Integer; pragma Import (Ada, E568, "ghdlxml_E");

   Sec_Default_Sized_Stacks : array (1 .. 1) of aliased System.Secondary_Stack.SS_Stack (System.Parameters.Runtime_Default_Sec_Stack_Size);

   Local_Priority_Specific_Dispatching : constant String := "";
   Local_Interrupt_States : constant String := "";

   Is_Elaborated : Boolean := False;

   procedure finalize_library is
   begin
      declare
         procedure F1;
         pragma Import (Ada, F1, "ghdlxml__finalize_body");
      begin
         E568 := E568 - 1;
         F1;
      end;
      declare
         procedure F2;
         pragma Import (Ada, F2, "ghdlvpi__finalize_body");
      begin
         E566 := E566 - 1;
         F2;
      end;
      declare
         procedure F3;
         pragma Import (Ada, F3, "ghdlsynth__finalize_body");
      begin
         E554 := E554 - 1;
         F3;
      end;
      declare
         procedure F4;
         pragma Import (Ada, F4, "ghdlprint__finalize_body");
      begin
         E535 := E535 - 1;
         F4;
      end;
      declare
         procedure F5;
         pragma Import (Ada, F5, "ghdldrv__finalize_body");
      begin
         E005 := E005 - 1;
         F5;
      end;
      declare
         procedure F6;
         pragma Import (Ada, F6, "ghdlcomp__finalize_body");
      begin
         E556 := E556 - 1;
         F6;
      end;
      declare
         procedure F7;
         pragma Import (Ada, F7, "ghdllocal__finalize_body");
      begin
         E183 := E183 - 1;
         F7;
      end;
      declare
         procedure F8;
         pragma Import (Ada, F8, "ghdlmain__finalize_body");
      begin
         E524 := E524 - 1;
         F8;
      end;
      declare
         procedure F9;
         pragma Import (Ada, F9, "ghdlmain__finalize_spec");
      begin
         F9;
      end;
      declare
         procedure F10;
         pragma Import (Ada, F10, "vhdl__formatters__finalize_body");
      begin
         E549 := E549 - 1;
         F10;
      end;
      declare
         procedure F11;
         pragma Import (Ada, F11, "vhdl__prints__finalize_body");
      begin
         E507 := E507 - 1;
         F11;
      end;
      E358 := E358 - 1;
      declare
         procedure F12;
         pragma Import (Ada, F12, "elab__vhdl_context__finalize_spec");
      begin
         F12;
      end;
      E185 := E185 - 1;
      declare
         procedure F13;
         pragma Import (Ada, F13, "ada__directories__finalize_spec");
      begin
         F13;
      end;
      E225 := E225 - 1;
      declare
         procedure F14;
         pragma Import (Ada, F14, "system__regexp__finalize_spec");
      begin
         F14;
      end;
      E313 := E313 - 1;
      declare
         procedure F15;
         pragma Import (Ada, F15, "system__pool_global__finalize_spec");
      begin
         F15;
      end;
      E143 := E143 - 1;
      declare
         procedure F16;
         pragma Import (Ada, F16, "ada__text_io__finalize_spec");
      begin
         F16;
      end;
      E203 := E203 - 1;
      declare
         procedure F17;
         pragma Import (Ada, F17, "ada__strings__unbounded__finalize_spec");
      begin
         F17;
      end;
      E209 := E209 - 1;
      declare
         procedure F18;
         pragma Import (Ada, F18, "system__storage_pools__subpools__finalize_spec");
      begin
         F18;
      end;
      E211 := E211 - 1;
      declare
         procedure F19;
         pragma Import (Ada, F19, "system__finalization_masters__finalize_spec");
      begin
         F19;
      end;
      declare
         procedure F20;
         pragma Import (Ada, F20, "system__file_io__finalize_body");
      begin
         E147 := E147 - 1;
         F20;
      end;
      declare
         procedure Reraise_Library_Exception_If_Any;
            pragma Import (Ada, Reraise_Library_Exception_If_Any, "__gnat_reraise_library_exception_if_any");
      begin
         Reraise_Library_Exception_If_Any;
      end;
   end finalize_library;

   procedure adafinal is
      procedure s_stalib_adafinal;
      pragma Import (C, s_stalib_adafinal, "system__standard_library__adafinal");

      procedure Runtime_Finalize;
      pragma Import (C, Runtime_Finalize, "__gnat_runtime_finalize");

   begin
      if not Is_Elaborated then
         return;
      end if;
      Is_Elaborated := False;
      Runtime_Finalize;
      s_stalib_adafinal;
   end adafinal;

   type No_Param_Proc is access procedure;

   procedure adainit is
      Main_Priority : Integer;
      pragma Import (C, Main_Priority, "__gl_main_priority");
      Time_Slice_Value : Integer;
      pragma Import (C, Time_Slice_Value, "__gl_time_slice_val");
      WC_Encoding : Character;
      pragma Import (C, WC_Encoding, "__gl_wc_encoding");
      Locking_Policy : Character;
      pragma Import (C, Locking_Policy, "__gl_locking_policy");
      Queuing_Policy : Character;
      pragma Import (C, Queuing_Policy, "__gl_queuing_policy");
      Task_Dispatching_Policy : Character;
      pragma Import (C, Task_Dispatching_Policy, "__gl_task_dispatching_policy");
      Priority_Specific_Dispatching : System.Address;
      pragma Import (C, Priority_Specific_Dispatching, "__gl_priority_specific_dispatching");
      Num_Specific_Dispatching : Integer;
      pragma Import (C, Num_Specific_Dispatching, "__gl_num_specific_dispatching");
      Main_CPU : Integer;
      pragma Import (C, Main_CPU, "__gl_main_cpu");
      Interrupt_States : System.Address;
      pragma Import (C, Interrupt_States, "__gl_interrupt_states");
      Num_Interrupt_States : Integer;
      pragma Import (C, Num_Interrupt_States, "__gl_num_interrupt_states");
      Unreserve_All_Interrupts : Integer;
      pragma Import (C, Unreserve_All_Interrupts, "__gl_unreserve_all_interrupts");
      Exception_Tracebacks : Integer;
      pragma Import (C, Exception_Tracebacks, "__gl_exception_tracebacks");
      Detect_Blocking : Integer;
      pragma Import (C, Detect_Blocking, "__gl_detect_blocking");
      Default_Stack_Size : Integer;
      pragma Import (C, Default_Stack_Size, "__gl_default_stack_size");
      Default_Secondary_Stack_Size : System.Parameters.Size_Type;
      pragma Import (C, Default_Secondary_Stack_Size, "__gnat_default_ss_size");
      Leap_Seconds_Support : Integer;
      pragma Import (C, Leap_Seconds_Support, "__gl_leap_seconds_support");
      Bind_Env_Addr : System.Address;
      pragma Import (C, Bind_Env_Addr, "__gl_bind_env_addr");

      procedure Runtime_Initialize (Install_Handler : Integer);
      pragma Import (C, Runtime_Initialize, "__gnat_runtime_initialize");

      Finalize_Library_Objects : No_Param_Proc;
      pragma Import (C, Finalize_Library_Objects, "__gnat_finalize_library_objects");
      Binder_Sec_Stacks_Count : Natural;
      pragma Import (Ada, Binder_Sec_Stacks_Count, "__gnat_binder_ss_count");
      Default_Sized_SS_Pool : System.Address;
      pragma Import (Ada, Default_Sized_SS_Pool, "__gnat_default_ss_pool");

   begin
      if Is_Elaborated then
         return;
      end if;
      Is_Elaborated := True;
      Main_Priority := -1;
      Time_Slice_Value := -1;
      WC_Encoding := 'b';
      Locking_Policy := ' ';
      Queuing_Policy := ' ';
      Task_Dispatching_Policy := ' ';
      Priority_Specific_Dispatching :=
        Local_Priority_Specific_Dispatching'Address;
      Num_Specific_Dispatching := 0;
      Main_CPU := -1;
      Interrupt_States := Local_Interrupt_States'Address;
      Num_Interrupt_States := 0;
      Unreserve_All_Interrupts := 0;
      Exception_Tracebacks := 1;
      Detect_Blocking := 0;
      Default_Stack_Size := -1;
      Leap_Seconds_Support := 0;

      ada_main'Elab_Body;
      Default_Secondary_Stack_Size := System.Parameters.Runtime_Default_Sec_Stack_Size;
      Binder_Sec_Stacks_Count := 1;
      Default_Sized_SS_Pool := Sec_Default_Sized_Stacks'Address;

      Runtime_Initialize (1);

      Finalize_Library_Objects := finalize_library'access;

      System.Soft_Links'Elab_Spec;
      System.Exception_Table'Elab_Body;
      E027 := E027 + 1;
      Ada.Io_Exceptions'Elab_Spec;
      E072 := E072 + 1;
      Ada.Strings'Elab_Spec;
      E057 := E057 + 1;
      Ada.Containers'Elab_Spec;
      E042 := E042 + 1;
      System.Exceptions'Elab_Spec;
      E029 := E029 + 1;
      System.Soft_Links.Initialize'Elab_Body;
      E019 := E019 + 1;
      E017 := E017 + 1;
      System.Os_Lib'Elab_Body;
      E077 := E077 + 1;
      Ada.Strings.Maps'Elab_Spec;
      Ada.Strings.Maps.Constants'Elab_Spec;
      E063 := E063 + 1;
      Interfaces.C'Elab_Spec;
      E059 := E059 + 1;
      E047 := E047 + 1;
      System.Object_Reader'Elab_Spec;
      System.Dwarf_Lines'Elab_Spec;
      E052 := E052 + 1;
      System.Traceback.Symbolic'Elab_Body;
      E041 := E041 + 1;
      E083 := E083 + 1;
      Ada.Tags'Elab_Spec;
      Ada.Tags'Elab_Body;
      E103 := E103 + 1;
      Ada.Streams'Elab_Spec;
      E101 := E101 + 1;
      Gnat'Elab_Spec;
      E121 := E121 + 1;
      System.Aux_Dec'Elab_Spec;
      E255 := E255 + 1;
      System.File_Control_Block'Elab_Spec;
      E151 := E151 + 1;
      System.Finalization_Root'Elab_Spec;
      E150 := E150 + 1;
      Ada.Finalization'Elab_Spec;
      E148 := E148 + 1;
      System.File_Io'Elab_Body;
      E147 := E147 + 1;
      System.Storage_Pools'Elab_Spec;
      E215 := E215 + 1;
      System.Finalization_Masters'Elab_Spec;
      System.Finalization_Masters'Elab_Body;
      E211 := E211 + 1;
      System.Storage_Pools.Subpools'Elab_Spec;
      E209 := E209 + 1;
      Ada.Strings.Unbounded'Elab_Spec;
      E203 := E203 + 1;
      Ada.Calendar'Elab_Spec;
      Ada.Calendar'Elab_Body;
      E116 := E116 + 1;
      Ada.Calendar.Time_Zones'Elab_Spec;
      E120 := E120 + 1;
      Ada.Text_Io'Elab_Spec;
      Ada.Text_Io'Elab_Body;
      E143 := E143 + 1;
      Gnat.Directory_Operations'Elab_Spec;
      Gnat.Directory_Operations'Elab_Body;
      E123 := E123 + 1;
      E132 := E132 + 1;
      E134 := E134 + 1;
      Gnat.Sha1'Elab_Spec;
      E130 := E130 + 1;
      System.Assertions'Elab_Spec;
      E155 := E155 + 1;
      System.Pool_Global'Elab_Spec;
      E313 := E313 + 1;
      System.Regexp'Elab_Spec;
      E225 := E225 + 1;
      Ada.Directories'Elab_Spec;
      Ada.Directories'Elab_Body;
      E185 := E185 + 1;
      System.Response_File'Elab_Spec;
      E527 := E527 + 1;
      Types'Elab_Spec;
      E168 := E168 + 1;
      E236 := E236 + 1;
      E110 := E110 + 1;
      E238 := E238 + 1;
      E181 := E181 + 1;
      E426 := E426 + 1;
      E518 := E518 + 1;
      Grt.Files_Operations'Elab_Body;
      E362 := E362 + 1;
      E444 := E444 + 1;
      E388 := E388 + 1;
      E386 := E386 + 1;
      E422 := E422 + 1;
      E461 := E461 + 1;
      E234 := E234 + 1;
      E141 := E141 + 1;
      E139 := E139 + 1;
      E449 := E449 + 1;
      E167 := E167 + 1;
      E259 := E259 + 1;
      E246 := E246 + 1;
      Name_Table'Elab_Body;
      E153 := E153 + 1;
      E271 := E271 + 1;
      Str_Table'Elab_Body;
      E170 := E170 + 1;
      Files_Map'Elab_Spec;
      E114 := E114 + 1;
      Errorout'Elab_Spec;
      Errorout'Elab_Body;
      E112 := E112 + 1;
      E352 := E352 + 1;
      E531 := E531 + 1;
      PSL.HASH'ELAB_BODY;
      E267 := E267 + 1;
      E263 := E263 + 1;
      E269 := E269 + 1;
      PSL.NODES'ELAB_BODY;
      E265 := E265 + 1;
      E261 := E261 + 1;
      PSL.NFAS'ELAB_BODY;
      E330 := E330 + 1;
      E332 := E332 + 1;
      E326 := E326 + 1;
      PSL.CSE'ELAB_BODY;
      E324 := E324 + 1;
      E328 := E328 + 1;
      E334 := E334 + 1;
      PSL.QM'ELAB_SPEC;
      E336 := E336 + 1;
      PSL.BUILD'ELAB_BODY;
      E322 := E322 + 1;
      E338 := E338 + 1;
      E455 := E455 + 1;
      E401 := E401 + 1;
      E393 := E393 + 1;
      E391 := E391 + 1;
      Netlists'Elab_Body;
      E384 := E384 + 1;
      E399 := E399 + 1;
      E414 := E414 + 1;
      E420 := E420 + 1;
      E434 := E434 + 1;
      Netlists.Locations'Elab_Body;
      E397 := E397 + 1;
      E469 := E469 + 1;
      E465 := E465 + 1;
      E418 := E418 + 1;
      E395 := E395 + 1;
      E412 := E412 + 1;
      E416 := E416 + 1;
      E424 := E424 + 1;
      E467 := E467 + 1;
      E410 := E410 + 1;
      E375 := E375 + 1;
      E432 := E432 + 1;
      E558 := E558 + 1;
      E560 := E560 + 1;
      E562 := E562 + 1;
      E430 := E430 + 1;
      E529 := E529 + 1;
      Vhdl.Flists'Elab_Body;
      E257 := E257 + 1;
      Vhdl.Lists'Elab_Body;
      E244 := E244 + 1;
      E248 := E248 + 1;
      Vhdl.Nodes'Elab_Body;
      E242 := E242 + 1;
      Elab.Vhdl_Objtypes'Elab_Spec;
      E232 := E232 + 1;
      E360 := E360 + 1;
      Elab.Vhdl_Heap'Elab_Body;
      E382 := E382 + 1;
      E373 := E373 + 1;
      Synth.Ieee.Numeric_Std'Elab_Body;
      E447 := E447 + 1;
      E300 := E300 + 1;
      Vhdl.Elocations'Elab_Body;
      E298 := E298 + 1;
      E288 := E288 + 1;
      E284 := E284 + 1;
      E290 := E290 + 1;
      E286 := E286 + 1;
      E292 := E292 + 1;
      E282 := E282 + 1;
      E294 := E294 + 1;
      E438 := E438 + 1;
      Vhdl.Annotations'Elab_Body;
      E369 := E369 + 1;
      Elab.Vhdl_Context'Elab_Spec;
      Elab.Vhdl_Context'Elab_Body;
      E358 := E358 + 1;
      E371 := E371 + 1;
      E274 := E274 + 1;
      E471 := E471 + 1;
      Vhdl.Ieee.Numeric'Elab_Body;
      E477 := E477 + 1;
      Vhdl.Ieee.Numeric_Std_Unsigned'Elab_Body;
      E491 := E491 + 1;
      Vhdl.Ieee.Std_Logic_Arith'Elab_Body;
      E493 := E493 + 1;
      Vhdl.Ieee.Std_Logic_Misc'Elab_Body;
      E495 := E495 + 1;
      Vhdl.Ieee.Std_Logic_Unsigned'Elab_Body;
      E497 := E497 + 1;
      E515 := E515 + 1;
      Vhdl.Prints'Elab_Spec;
      Vhdl.Prints'Elab_Body;
      E507 := E507 + 1;
      E564 := E564 + 1;
      Vhdl.Scanner'Elab_Body;
      E305 := E305 + 1;
      Vhdl.Formatters'Elab_Body;
      E549 := E549 + 1;
      Vhdl.Sem_Inst'Elab_Body;
      E481 := E481 + 1;
      Vhdl.Sem_Scopes'Elab_Body;
      E501 := E501 + 1;
      E505 := E505 + 1;
      Vhdl.Xrefs'Elab_Body;
      E309 := E309 + 1;
      E296 := E296 + 1;
      E302 := E302 + 1;
      Options'Elab_Spec;
      Libraries'Elab_Body;
      E227 := E227 + 1;
      E229 := E229 + 1;
      Ghdlmain'Elab_Spec;
      Ghdlmain'Elab_Body;
      E524 := E524 + 1;
      E487 := E487 + 1;
      Synthesis'Elab_Spec;
      Vhdl.Configuration'Elab_Spec;
      Elab.Vhdl_Files'Elab_Spec;
      Synth.Vhdl_Environment'Elab_Spec;
      Elab.Debugger'Elab_Body;
      E356 := E356 + 1;
      E499 := E499 + 1;
      E489 := E489 + 1;
      E485 := E485 + 1;
      E509 := E509 + 1;
      E511 := E511 + 1;
      E483 := E483 + 1;
      E513 := E513 + 1;
      E479 := E479 + 1;
      E503 := E503 + 1;
      E342 := E342 + 1;
      E475 := E475 + 1;
      E463 := E463 + 1;
      E440 := E440 + 1;
      E354 := E354 + 1;
      E522 := E522 + 1;
      E520 := E520 + 1;
      E350 := E350 + 1;
      E348 := E348 + 1;
      E346 := E346 + 1;
      E344 := E344 + 1;
      Vhdl.Configuration'Elab_Body;
      E318 := E318 + 1;
      E442 := E442 + 1;
      E473 := E473 + 1;
      E408 := E408 + 1;
      E377 := E377 + 1;
      E320 := E320 + 1;
      E340 := E340 + 1;
      Synth.Vhdl_Stmts'Elab_Body;
      E453 := E453 + 1;
      Synth.Vhdl_Insts'Elab_Body;
      E459 := E459 + 1;
      E457 := E457 + 1;
      E436 := E436 + 1;
      E380 := E380 + 1;
      E403 := E403 + 1;
      Synth.Vhdl_Context'Elab_Body;
      E405 := E405 + 1;
      Ghdllocal'Elab_Spec;
      Ghdllocal'Elab_Body;
      E183 := E183 + 1;
      Ghdlcomp'Elab_Spec;
      Ghdlcomp'Elab_Body;
      E556 := E556 + 1;
      Ghdldrv'Elab_Body;
      E005 := E005 + 1;
      E533 := E533 + 1;
      Ghdlprint'Elab_Body;
      E535 := E535 + 1;
      Ghdlsynth'Elab_Body;
      E554 := E554 + 1;
      Ghdlvpi'Elab_Body;
      E566 := E566 + 1;
      Ghdlxml'Elab_Body;
      E568 := E568 + 1;
   end adainit;

   procedure Ada_Main_Program;
   pragma Import (Ada, Ada_Main_Program, "_ada_ghdl_llvm");

   function main
     (argc : Integer;
      argv : System.Address;
      envp : System.Address)
      return Integer
   is
      procedure Initialize (Addr : System.Address);
      pragma Import (C, Initialize, "__gnat_initialize");

      procedure Finalize;
      pragma Import (C, Finalize, "__gnat_finalize");
      SEH : aliased array (1 .. 2) of Integer;

      Ensure_Reference : aliased System.Address := Ada_Main_Program_Name'Address;
      pragma Volatile (Ensure_Reference);

   begin
      gnat_argc := argc;
      gnat_argv := argv;
      gnat_envp := envp;

      Initialize (SEH'Address);
      adainit;
      Ada_Main_Program;
      adafinal;
      Finalize;
      return (gnat_exit_status);
   end;

--  BEGIN Object file/option list
   --   ./elab.o
   --   ./grt.o
   --   ./grt-c.o
   --   ./grt-fcvt.o
   --   ./grt-severity.o
   --   ./grt-stdio.o
   --   ./grt-types.o
   --   ./grt-vhdl_types.o
   --   ./grt-vstrings.o
   --   ./psl.o
   --   ./synth.o
   --   ./synth-ieee.o
   --   ./types.o
   --   ./vhdl.o
   --   ./vhdl-nodes_priv.o
   --   ./vhdl-tokens.o
   --   ./vhdl-types.o
   --   ./areapools.o
   --   ./default_paths.o
   --   ./dyn_tables.o
   --   ./elab-memtype.o
   --   ./flags.o
   --   ./grt-algos.o
   --   ./grt-readline_none.o
   --   ./grt-readline.o
   --   ./grt-table.o
   --   ./grt-files_operations.o
   --   ./grt-to_strings.o
   --   ./hash.o
   --   ./dyn_maps.o
   --   ./dyn_interning.o
   --   ./interning.o
   --   ./mutils.o
   --   ./psl-nodes_priv.o
   --   ./psl-priorities.o
   --   ./psl-types.o
   --   ./simple_io.o
   --   ./logging.o
   --   ./synth-flags.o
   --   ./synth-ieee-std_logic_1164.o
   --   ./tables.o
   --   ./flists.o
   --   ./lists.o
   --   ./name_table.o
   --   ./std_names.o
   --   ./str_table.o
   --   ./files_map.o
   --   ./errorout.o
   --   ./elab-vhdl_errors.o
   --   ./errorout-console.o
   --   ./psl-hash.o
   --   ./psl-errors.o
   --   ./psl-nodes_meta.o
   --   ./psl-nodes.o
   --   ./psl-dump_tree.o
   --   ./psl-nfas.o
   --   ./psl-nfas-utils.o
   --   ./psl-prints.o
   --   ./psl-cse.o
   --   ./psl-disp_nfas.o
   --   ./psl-optimize.o
   --   ./psl-qm.o
   --   ./psl-build.o
   --   ./psl-rewrites.o
   --   ./psl-subsets.o
   --   ./synth-errors.o
   --   ./types_utils.o
   --   ./netlists-gates.o
   --   ./netlists-utils.o
   --   ./netlists.o
   --   ./netlists-builders.o
   --   ./netlists-gates_ports.o
   --   ./netlists-internings.o
   --   ./netlists-iterators.o
   --   ./netlists-locations.o
   --   ./netlists-butils.o
   --   ./netlists-cleanup.o
   --   ./netlists-errors.o
   --   ./netlists-folds.o
   --   ./netlists-concats.o
   --   ./netlists-inference.o
   --   ./netlists-memories.o
   --   ./netlists-expands.o
   --   ./synth-context.o
   --   ./synth-environment.o
   --   ./utils_io.o
   --   ./netlists-dump.o
   --   ./netlists-disp_dot.o
   --   ./netlists-disp_verilog.o
   --   ./netlists-disp_vhdl.o
   --   ./synth-environment-debug.o
   --   ./version.o
   --   ./bug.o
   --   ./vhdl-flists.o
   --   ./vhdl-lists.o
   --   ./vhdl-nodes_meta.o
   --   ./vhdl-nodes.o
   --   ./elab-vhdl_objtypes.o
   --   ./elab-vhdl_values.o
   --   ./elab-vhdl_heap.o
   --   ./elab-vhdl_values-debug.o
   --   ./synth-ieee-numeric_std.o
   --   ./vhdl-back_end.o
   --   ./vhdl-elocations_meta.o
   --   ./vhdl-elocations.o
   --   ./vhdl-nodes_utils.o
   --   ./vhdl-errors.o
   --   ./vhdl-sem_utils.o
   --   ./vhdl-std_package.o
   --   ./vhdl-ieee.o
   --   ./vhdl-utils.o
   --   ./vhdl-ieee-std_logic_1164.o
   --   ./synth-source.o
   --   ./vhdl-annotations.o
   --   ./elab-vhdl_context.o
   --   ./elab-vhdl_context-debug.o
   --   ./vhdl-disp_tree.o
   --   ./vhdl-ieee-math_real.o
   --   ./vhdl-ieee-numeric.o
   --   ./vhdl-ieee-numeric_std_unsigned.o
   --   ./vhdl-ieee-std_logic_arith.o
   --   ./vhdl-ieee-std_logic_misc.o
   --   ./vhdl-ieee-std_logic_unsigned.o
   --   ./vhdl-nodes_walk.o
   --   ./vhdl-prints.o
   --   ./synth-disp_vhdl.o
   --   ./vhdl-scanner.o
   --   ./vhdl-formatters.o
   --   ./vhdl-sem_inst.o
   --   ./vhdl-sem_scopes.o
   --   ./vhdl-std_env.o
   --   ./vhdl-xrefs.o
   --   ./vhdl-parse.o
   --   ./vhdl-parse_psl.o
   --   ./libraries.o
   --   ./options.o
   --   ./ghdlmain.o
   --   ./vhdl-nodes_gc.o
   --   ./elab-debugger.o
   --   ./vhdl-ieee-vital_timing.o
   --   ./vhdl-post_sems.o
   --   ./vhdl-sem_lib.o
   --   ./vhdl-sem_psl.o
   --   ./vhdl-sem_stmts.o
   --   ./vhdl-sem_names.o
   --   ./vhdl-sem_types.o
   --   ./vhdl-sem_decls.o
   --   ./vhdl-sem_specs.o
   --   ./vhdl-sem.o
   --   ./vhdl-sem_expr.o
   --   ./synthesis.o
   --   ./synth-vhdl_eval.o
   --   ./elab-vhdl_expr.o
   --   ./elab-vhdl_stmts.o
   --   ./elab-vhdl_insts.o
   --   ./elab-vhdl_decls.o
   --   ./elab-vhdl_types.o
   --   ./vhdl-evaluation.o
   --   ./vhdl-sem_assocs.o
   --   ./vhdl-configuration.o
   --   ./elab-vhdl_files.o
   --   ./synth-vhdl_static_proc.o
   --   ./synth-vhdl_environment.o
   --   ./elab-vhdl_debug.o
   --   ./vhdl-canon.o
   --   ./vhdl-canon_psl.o
   --   ./synth-vhdl_stmts.o
   --   ./synth-vhdl_insts.o
   --   ./synth-vhdl_decls.o
   --   ./synth-vhdl_oper.o
   --   ./synth-vhdl_expr.o
   --   ./synth-vhdl_aggr.o
   --   ./synth-vhdl_context.o
   --   ./ghdllocal.o
   --   ./ghdlcomp.o
   --   ./ghdldrv.o
   --   ./ghdllib.o
   --   ./ghdlprint.o
   --   ./ghdlsynth.o
   --   ./ghdlsynth_maybe.o
   --   ./ghdlvpi.o
   --   ./ghdlxml.o
   --   ./ghdl_llvm.o
   --   -L./
   --   -L/usr/lib/gcc/x86_64-linux-gnu/9/adalib/
   --   -shared
   --   -lm
   --   -lgnat-9
   --   -ldl
--  END Object file/option list   

end ada_main;
