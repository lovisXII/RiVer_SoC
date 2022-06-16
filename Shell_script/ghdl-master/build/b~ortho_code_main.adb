pragma Warnings (Off);
pragma Ada_95;
pragma Source_File_Name (ada_main, Spec_File_Name => "b~ortho_code_main.ads");
pragma Source_File_Name (ada_main, Body_File_Name => "b~ortho_code_main.adb");
pragma Suppress (Overflow_Check);
with Ada.Exceptions;

package body ada_main is

   E075 : Short_Integer; pragma Import (Ada, E075, "system__os_lib_E");
   E015 : Short_Integer; pragma Import (Ada, E015, "system__soft_links_E");
   E025 : Short_Integer; pragma Import (Ada, E025, "system__exception_table_E");
   E070 : Short_Integer; pragma Import (Ada, E070, "ada__io_exceptions_E");
   E055 : Short_Integer; pragma Import (Ada, E055, "ada__strings_E");
   E040 : Short_Integer; pragma Import (Ada, E040, "ada__containers_E");
   E027 : Short_Integer; pragma Import (Ada, E027, "system__exceptions_E");
   E017 : Short_Integer; pragma Import (Ada, E017, "system__soft_links__initialize_E");
   E057 : Short_Integer; pragma Import (Ada, E057, "ada__strings__maps_E");
   E061 : Short_Integer; pragma Import (Ada, E061, "ada__strings__maps__constants_E");
   E045 : Short_Integer; pragma Import (Ada, E045, "interfaces__c_E");
   E081 : Short_Integer; pragma Import (Ada, E081, "system__object_reader_E");
   E050 : Short_Integer; pragma Import (Ada, E050, "system__dwarf_lines_E");
   E039 : Short_Integer; pragma Import (Ada, E039, "system__traceback__symbolic_E");
   E099 : Short_Integer; pragma Import (Ada, E099, "ada__tags_E");
   E107 : Short_Integer; pragma Import (Ada, E107, "ada__streams_E");
   E120 : Short_Integer; pragma Import (Ada, E120, "gnat_E");
   E223 : Short_Integer; pragma Import (Ada, E223, "system__aux_dec_E");
   E115 : Short_Integer; pragma Import (Ada, E115, "system__file_control_block_E");
   E114 : Short_Integer; pragma Import (Ada, E114, "system__finalization_root_E");
   E112 : Short_Integer; pragma Import (Ada, E112, "ada__finalization_E");
   E111 : Short_Integer; pragma Import (Ada, E111, "system__file_io_E");
   E274 : Short_Integer; pragma Import (Ada, E274, "system__storage_pools_E");
   E270 : Short_Integer; pragma Import (Ada, E270, "system__finalization_masters_E");
   E268 : Short_Integer; pragma Import (Ada, E268, "system__storage_pools__subpools_E");
   E262 : Short_Integer; pragma Import (Ada, E262, "ada__strings__unbounded_E");
   E149 : Short_Integer; pragma Import (Ada, E149, "ada__calendar_E");
   E153 : Short_Integer; pragma Import (Ada, E153, "ada__calendar__time_zones_E");
   E105 : Short_Integer; pragma Import (Ada, E105, "ada__text_io_E");
   E122 : Short_Integer; pragma Import (Ada, E122, "gnat__directory_operations_E");
   E157 : Short_Integer; pragma Import (Ada, E157, "gnat__secure_hashes_E");
   E159 : Short_Integer; pragma Import (Ada, E159, "gnat__secure_hashes__sha1_E");
   E155 : Short_Integer; pragma Import (Ada, E155, "gnat__sha1_E");
   E168 : Short_Integer; pragma Import (Ada, E168, "system__assertions_E");
   E320 : Short_Integer; pragma Import (Ada, E320, "system__pool_global_E");
   E143 : Short_Integer; pragma Import (Ada, E143, "types_E");
   E204 : Short_Integer; pragma Import (Ada, E204, "areapools_E");
   E140 : Short_Integer; pragma Import (Ada, E140, "dyn_tables_E");
   E206 : Short_Integer; pragma Import (Ada, E206, "elab__memtype_E");
   E191 : Short_Integer; pragma Import (Ada, E191, "flags_E");
   E451 : Short_Integer; pragma Import (Ada, E451, "grt__algos_E");
   E541 : Short_Integer; pragma Import (Ada, E541, "grt__readline_none_E");
   E393 : Short_Integer; pragma Import (Ada, E393, "grt__files_operations_E");
   E469 : Short_Integer; pragma Import (Ada, E469, "grt__to_strings_E");
   E142 : Short_Integer; pragma Import (Ada, E142, "hash_E");
   E138 : Short_Integer; pragma Import (Ada, E138, "dyn_maps_E");
   E136 : Short_Integer; pragma Import (Ada, E136, "dyn_interning_E");
   E193 : Short_Integer; pragma Import (Ada, E193, "interning_E");
   E202 : Short_Integer; pragma Import (Ada, E202, "mutils_E");
   E313 : Short_Integer; pragma Import (Ada, E313, "ortho_ident_E");
   E316 : Short_Integer; pragma Import (Ada, E316, "ortho_llvm_E");
   E129 : Short_Integer; pragma Import (Ada, E129, "simple_io_E");
   E164 : Short_Integer; pragma Import (Ada, E164, "logging_E");
   E474 : Short_Integer; pragma Import (Ada, E474, "synth__ieee__std_logic_1164_E");
   E176 : Short_Integer; pragma Import (Ada, E176, "tables_E");
   E227 : Short_Integer; pragma Import (Ada, E227, "flists_E");
   E214 : Short_Integer; pragma Import (Ada, E214, "lists_E");
   E166 : Short_Integer; pragma Import (Ada, E166, "name_table_E");
   E239 : Short_Integer; pragma Import (Ada, E239, "std_names_E");
   E178 : Short_Integer; pragma Import (Ada, E178, "str_table_E");
   E147 : Short_Integer; pragma Import (Ada, E147, "files_map_E");
   E145 : Short_Integer; pragma Import (Ada, E145, "errorout_E");
   E383 : Short_Integer; pragma Import (Ada, E383, "elab__vhdl_errors_E");
   E189 : Short_Integer; pragma Import (Ada, E189, "errorout__console_E");
   E231 : Short_Integer; pragma Import (Ada, E231, "psl__errors_E");
   E233 : Short_Integer; pragma Import (Ada, E233, "psl__nodes_E");
   E235 : Short_Integer; pragma Import (Ada, E235, "psl__hash_E");
   E237 : Short_Integer; pragma Import (Ada, E237, "psl__nodes_meta_E");
   E229 : Short_Integer; pragma Import (Ada, E229, "psl__dump_tree_E");
   E343 : Short_Integer; pragma Import (Ada, E343, "psl__nfas_E");
   E345 : Short_Integer; pragma Import (Ada, E345, "psl__nfas__utils_E");
   E361 : Short_Integer; pragma Import (Ada, E361, "psl__prints_E");
   E359 : Short_Integer; pragma Import (Ada, E359, "psl__cse_E");
   E363 : Short_Integer; pragma Import (Ada, E363, "psl__disp_nfas_E");
   E365 : Short_Integer; pragma Import (Ada, E365, "psl__optimize_E");
   E367 : Short_Integer; pragma Import (Ada, E367, "psl__qm_E");
   E357 : Short_Integer; pragma Import (Ada, E357, "psl__build_E");
   E369 : Short_Integer; pragma Import (Ada, E369, "psl__rewrites_E");
   E347 : Short_Integer; pragma Import (Ada, E347, "psl__subsets_E");
   E428 : Short_Integer; pragma Import (Ada, E428, "synth__errors_E");
   E420 : Short_Integer; pragma Import (Ada, E420, "types_utils_E");
   E415 : Short_Integer; pragma Import (Ada, E415, "netlists_E");
   E418 : Short_Integer; pragma Import (Ada, E418, "netlists__utils_E");
   E426 : Short_Integer; pragma Import (Ada, E426, "netlists__builders_E");
   E441 : Short_Integer; pragma Import (Ada, E441, "netlists__gates_ports_E");
   E447 : Short_Integer; pragma Import (Ada, E447, "netlists__internings_E");
   E459 : Short_Integer; pragma Import (Ada, E459, "netlists__iterators_E");
   E424 : Short_Integer; pragma Import (Ada, E424, "netlists__locations_E");
   E492 : Short_Integer; pragma Import (Ada, E492, "netlists__butils_E");
   E488 : Short_Integer; pragma Import (Ada, E488, "netlists__cleanup_E");
   E445 : Short_Integer; pragma Import (Ada, E445, "netlists__errors_E");
   E422 : Short_Integer; pragma Import (Ada, E422, "netlists__folds_E");
   E439 : Short_Integer; pragma Import (Ada, E439, "netlists__concats_E");
   E443 : Short_Integer; pragma Import (Ada, E443, "netlists__inference_E");
   E449 : Short_Integer; pragma Import (Ada, E449, "netlists__memories_E");
   E490 : Short_Integer; pragma Import (Ada, E490, "netlists__expands_E");
   E437 : Short_Integer; pragma Import (Ada, E437, "synth__environment_E");
   E406 : Short_Integer; pragma Import (Ada, E406, "utils_io_E");
   E457 : Short_Integer; pragma Import (Ada, E457, "netlists__dump_E");
   E455 : Short_Integer; pragma Import (Ada, E455, "synth__environment__debug_E");
   E119 : Short_Integer; pragma Import (Ada, E119, "bug_E");
   E225 : Short_Integer; pragma Import (Ada, E225, "vhdl__flists_E");
   E224 : Short_Integer; pragma Import (Ada, E224, "vhdl__flists_E");
   E212 : Short_Integer; pragma Import (Ada, E212, "vhdl__lists_E");
   E211 : Short_Integer; pragma Import (Ada, E211, "vhdl__lists_E");
   E210 : Short_Integer; pragma Import (Ada, E210, "vhdl__nodes_E");
   E216 : Short_Integer; pragma Import (Ada, E216, "vhdl__nodes_meta_E");
   E200 : Short_Integer; pragma Import (Ada, E200, "elab__vhdl_objtypes_E");
   E391 : Short_Integer; pragma Import (Ada, E391, "elab__vhdl_values_E");
   E413 : Short_Integer; pragma Import (Ada, E413, "elab__vhdl_heap_E");
   E404 : Short_Integer; pragma Import (Ada, E404, "elab__vhdl_values__debug_E");
   E472 : Short_Integer; pragma Import (Ada, E472, "synth__ieee__numeric_std_E");
   E318 : Short_Integer; pragma Import (Ada, E318, "trans_E");
   E294 : Short_Integer; pragma Import (Ada, E294, "vhdl__elocations_E");
   E296 : Short_Integer; pragma Import (Ada, E296, "vhdl__elocations_meta_E");
   E284 : Short_Integer; pragma Import (Ada, E284, "vhdl__nodes_utils_E");
   E260 : Short_Integer; pragma Import (Ada, E260, "vhdl__errors_E");
   E286 : Short_Integer; pragma Import (Ada, E286, "vhdl__sem_utils_E");
   E282 : Short_Integer; pragma Import (Ada, E282, "vhdl__std_package_E");
   E288 : Short_Integer; pragma Import (Ada, E288, "vhdl__ieee_E");
   E258 : Short_Integer; pragma Import (Ada, E258, "vhdl__utils_E");
   E290 : Short_Integer; pragma Import (Ada, E290, "vhdl__ieee__std_logic_1164_E");
   E463 : Short_Integer; pragma Import (Ada, E463, "synth__source_E");
   E400 : Short_Integer; pragma Import (Ada, E400, "vhdl__annotations_E");
   E389 : Short_Integer; pragma Import (Ada, E389, "elab__vhdl_context_E");
   E402 : Short_Integer; pragma Import (Ada, E402, "elab__vhdl_context__debug_E");
   E246 : Short_Integer; pragma Import (Ada, E246, "vhdl__disp_tree_E");
   E494 : Short_Integer; pragma Import (Ada, E494, "vhdl__ieee__math_real_E");
   E500 : Short_Integer; pragma Import (Ada, E500, "vhdl__ieee__numeric_E");
   E514 : Short_Integer; pragma Import (Ada, E514, "vhdl__ieee__numeric_std_unsigned_E");
   E516 : Short_Integer; pragma Import (Ada, E516, "vhdl__ieee__std_logic_arith_E");
   E518 : Short_Integer; pragma Import (Ada, E518, "vhdl__ieee__std_logic_misc_E");
   E520 : Short_Integer; pragma Import (Ada, E520, "vhdl__ieee__std_logic_unsigned_E");
   E538 : Short_Integer; pragma Import (Ada, E538, "vhdl__nodes_walk_E");
   E530 : Short_Integer; pragma Import (Ada, E530, "vhdl__prints_E");
   E553 : Short_Integer; pragma Import (Ada, E553, "trans_analyzes_E");
   E301 : Short_Integer; pragma Import (Ada, E301, "vhdl__scanner_E");
   E504 : Short_Integer; pragma Import (Ada, E504, "vhdl__sem_inst_E");
   E524 : Short_Integer; pragma Import (Ada, E524, "vhdl__sem_scopes_E");
   E528 : Short_Integer; pragma Import (Ada, E528, "vhdl__std_env_E");
   E305 : Short_Integer; pragma Import (Ada, E305, "vhdl__xrefs_E");
   E292 : Short_Integer; pragma Import (Ada, E292, "vhdl__parse_E");
   E298 : Short_Integer; pragma Import (Ada, E298, "vhdl__parse_psl_E");
   E195 : Short_Integer; pragma Import (Ada, E195, "libraries_E");
   E197 : Short_Integer; pragma Import (Ada, E197, "options_E");
   E510 : Short_Integer; pragma Import (Ada, E510, "vhdl__nodes_gc_E");
   E387 : Short_Integer; pragma Import (Ada, E387, "elab__debugger_E");
   E522 : Short_Integer; pragma Import (Ada, E522, "vhdl__ieee__vital_timing_E");
   E512 : Short_Integer; pragma Import (Ada, E512, "vhdl__post_sems_E");
   E508 : Short_Integer; pragma Import (Ada, E508, "vhdl__sem_lib_E");
   E547 : Short_Integer; pragma Import (Ada, E547, "vhdl__configuration_E");
   E543 : Short_Integer; pragma Import (Ada, E543, "elab__vhdl_insts_E");
   E545 : Short_Integer; pragma Import (Ada, E545, "elab__vhdl_stmts_E");
   E381 : Short_Integer; pragma Import (Ada, E381, "elab__vhdl_decls_E");
   E482 : Short_Integer; pragma Import (Ada, E482, "synth__vhdl_decls_E");
   E484 : Short_Integer; pragma Import (Ada, E484, "synth__vhdl_insts_E");
   E486 : Short_Integer; pragma Import (Ada, E486, "synthesis_E");
   E461 : Short_Integer; pragma Import (Ada, E461, "synth__vhdl_oper_E");
   E411 : Short_Integer; pragma Import (Ada, E411, "synth__vhdl_expr_E");
   E465 : Short_Integer; pragma Import (Ada, E465, "synth__vhdl_eval_E");
   E385 : Short_Integer; pragma Import (Ada, E385, "elab__vhdl_expr_E");
   E467 : Short_Integer; pragma Import (Ada, E467, "elab__vhdl_files_E");
   E496 : Short_Integer; pragma Import (Ada, E496, "synth__vhdl_static_proc_E");
   E379 : Short_Integer; pragma Import (Ada, E379, "elab__vhdl_types_E");
   E430 : Short_Integer; pragma Import (Ada, E430, "synth__vhdl_aggr_E");
   E377 : Short_Integer; pragma Import (Ada, E377, "vhdl__evaluation_E");
   E536 : Short_Integer; pragma Import (Ada, E536, "vhdl__sem_types_E");
   E534 : Short_Integer; pragma Import (Ada, E534, "vhdl__sem_stmts_E");
   E532 : Short_Integer; pragma Import (Ada, E532, "vhdl__sem_psl_E");
   E506 : Short_Integer; pragma Import (Ada, E506, "vhdl__sem_names_E");
   E526 : Short_Integer; pragma Import (Ada, E526, "vhdl__sem_specs_E");
   E502 : Short_Integer; pragma Import (Ada, E502, "vhdl__sem_decls_E");
   E498 : Short_Integer; pragma Import (Ada, E498, "vhdl__sem_expr_E");
   E408 : Short_Integer; pragma Import (Ada, E408, "elab__vhdl_debug_E");
   E375 : Short_Integer; pragma Import (Ada, E375, "vhdl__sem_assocs_E");
   E373 : Short_Integer; pragma Import (Ada, E373, "vhdl__sem_E");
   E355 : Short_Integer; pragma Import (Ada, E355, "vhdl__canon_E");
   E371 : Short_Integer; pragma Import (Ada, E371, "vhdl__canon_psl_E");
   E435 : Short_Integer; pragma Import (Ada, E435, "synth__vhdl_environment_E");
   E480 : Short_Integer; pragma Import (Ada, E480, "synth__vhdl_stmts_E");
   E432 : Short_Integer; pragma Import (Ada, E432, "synth__vhdl_context_E");
   E351 : Short_Integer; pragma Import (Ada, E351, "trans__foreach_non_composite_E");
   E555 : Short_Integer; pragma Import (Ada, E555, "trans__chap12_E");
   E311 : Short_Integer; pragma Import (Ada, E311, "translation_E");
   E349 : Short_Integer; pragma Import (Ada, E349, "trans__chap8_E");
   E341 : Short_Integer; pragma Import (Ada, E341, "trans__chap9_E");
   E339 : Short_Integer; pragma Import (Ada, E339, "trans__chap5_E");
   E337 : Short_Integer; pragma Import (Ada, E337, "trans__chap4_E");
   E335 : Short_Integer; pragma Import (Ada, E335, "trans__chap7_E");
   E551 : Short_Integer; pragma Import (Ada, E551, "trans__rtis_E");
   E331 : Short_Integer; pragma Import (Ada, E331, "trans__chap14_E");
   E333 : Short_Integer; pragma Import (Ada, E333, "trans__chap6_E");
   E353 : Short_Integer; pragma Import (Ada, E353, "trans__helpers2_E");
   E329 : Short_Integer; pragma Import (Ada, E329, "trans__chap3_E");
   E327 : Short_Integer; pragma Import (Ada, E327, "trans__chap2_E");
   E325 : Short_Integer; pragma Import (Ada, E325, "trans__chap1_E");
   E309 : Short_Integer; pragma Import (Ada, E309, "trans_be_E");
   E117 : Short_Integer; pragma Import (Ada, E117, "ortho_front_E");

   Sec_Default_Sized_Stacks : array (1 .. 1) of aliased System.Secondary_Stack.SS_Stack (System.Parameters.Runtime_Default_Sec_Stack_Size);

   Local_Priority_Specific_Dispatching : constant String := "";
   Local_Interrupt_States : constant String := "";

   Is_Elaborated : Boolean := False;

   procedure finalize_library is
   begin
      declare
         procedure F1;
         pragma Import (Ada, F1, "trans__chap8__finalize_body");
      begin
         E349 := E349 - 1;
         F1;
      end;
      declare
         procedure F2;
         pragma Import (Ada, F2, "trans__chap8__finalize_spec");
      begin
         F2;
      end;
      declare
         procedure F3;
         pragma Import (Ada, F3, "vhdl__prints__finalize_body");
      begin
         E530 := E530 - 1;
         F3;
      end;
      E389 := E389 - 1;
      declare
         procedure F4;
         pragma Import (Ada, F4, "elab__vhdl_context__finalize_spec");
      begin
         F4;
      end;
      E320 := E320 - 1;
      declare
         procedure F5;
         pragma Import (Ada, F5, "system__pool_global__finalize_spec");
      begin
         F5;
      end;
      E105 := E105 - 1;
      declare
         procedure F6;
         pragma Import (Ada, F6, "ada__text_io__finalize_spec");
      begin
         F6;
      end;
      E262 := E262 - 1;
      declare
         procedure F7;
         pragma Import (Ada, F7, "ada__strings__unbounded__finalize_spec");
      begin
         F7;
      end;
      E268 := E268 - 1;
      declare
         procedure F8;
         pragma Import (Ada, F8, "system__storage_pools__subpools__finalize_spec");
      begin
         F8;
      end;
      E270 := E270 - 1;
      declare
         procedure F9;
         pragma Import (Ada, F9, "system__finalization_masters__finalize_spec");
      begin
         F9;
      end;
      declare
         procedure F10;
         pragma Import (Ada, F10, "system__file_io__finalize_body");
      begin
         E111 := E111 - 1;
         F10;
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
      E025 := E025 + 1;
      Ada.Io_Exceptions'Elab_Spec;
      E070 := E070 + 1;
      Ada.Strings'Elab_Spec;
      E055 := E055 + 1;
      Ada.Containers'Elab_Spec;
      E040 := E040 + 1;
      System.Exceptions'Elab_Spec;
      E027 := E027 + 1;
      System.Soft_Links.Initialize'Elab_Body;
      E017 := E017 + 1;
      E015 := E015 + 1;
      System.Os_Lib'Elab_Body;
      E075 := E075 + 1;
      Ada.Strings.Maps'Elab_Spec;
      Ada.Strings.Maps.Constants'Elab_Spec;
      E061 := E061 + 1;
      Interfaces.C'Elab_Spec;
      E057 := E057 + 1;
      E045 := E045 + 1;
      System.Object_Reader'Elab_Spec;
      System.Dwarf_Lines'Elab_Spec;
      E050 := E050 + 1;
      System.Traceback.Symbolic'Elab_Body;
      E039 := E039 + 1;
      E081 := E081 + 1;
      Ada.Tags'Elab_Spec;
      Ada.Tags'Elab_Body;
      E099 := E099 + 1;
      Ada.Streams'Elab_Spec;
      E107 := E107 + 1;
      Gnat'Elab_Spec;
      E120 := E120 + 1;
      System.Aux_Dec'Elab_Spec;
      E223 := E223 + 1;
      System.File_Control_Block'Elab_Spec;
      E115 := E115 + 1;
      System.Finalization_Root'Elab_Spec;
      E114 := E114 + 1;
      Ada.Finalization'Elab_Spec;
      E112 := E112 + 1;
      System.File_Io'Elab_Body;
      E111 := E111 + 1;
      System.Storage_Pools'Elab_Spec;
      E274 := E274 + 1;
      System.Finalization_Masters'Elab_Spec;
      System.Finalization_Masters'Elab_Body;
      E270 := E270 + 1;
      System.Storage_Pools.Subpools'Elab_Spec;
      E268 := E268 + 1;
      Ada.Strings.Unbounded'Elab_Spec;
      E262 := E262 + 1;
      Ada.Calendar'Elab_Spec;
      Ada.Calendar'Elab_Body;
      E149 := E149 + 1;
      Ada.Calendar.Time_Zones'Elab_Spec;
      E153 := E153 + 1;
      Ada.Text_Io'Elab_Spec;
      Ada.Text_Io'Elab_Body;
      E105 := E105 + 1;
      Gnat.Directory_Operations'Elab_Spec;
      Gnat.Directory_Operations'Elab_Body;
      E122 := E122 + 1;
      E157 := E157 + 1;
      E159 := E159 + 1;
      Gnat.Sha1'Elab_Spec;
      E155 := E155 + 1;
      System.Assertions'Elab_Spec;
      E168 := E168 + 1;
      System.Pool_Global'Elab_Spec;
      E320 := E320 + 1;
      Types'Elab_Spec;
      E143 := E143 + 1;
      E204 := E204 + 1;
      E140 := E140 + 1;
      E206 := E206 + 1;
      E191 := E191 + 1;
      E451 := E451 + 1;
      E541 := E541 + 1;
      Grt.Files_Operations'Elab_Body;
      E393 := E393 + 1;
      E469 := E469 + 1;
      E142 := E142 + 1;
      E138 := E138 + 1;
      E136 := E136 + 1;
      E193 := E193 + 1;
      E202 := E202 + 1;
      E313 := E313 + 1;
      Ortho_Llvm'Elab_Spec;
      E316 := E316 + 1;
      E129 := E129 + 1;
      E164 := E164 + 1;
      E474 := E474 + 1;
      E176 := E176 + 1;
      E227 := E227 + 1;
      E214 := E214 + 1;
      Name_Table'Elab_Body;
      E166 := E166 + 1;
      E239 := E239 + 1;
      Str_Table'Elab_Body;
      E178 := E178 + 1;
      Files_Map'Elab_Spec;
      E147 := E147 + 1;
      Errorout'Elab_Spec;
      Errorout'Elab_Body;
      E145 := E145 + 1;
      E383 := E383 + 1;
      E189 := E189 + 1;
      PSL.HASH'ELAB_BODY;
      E235 := E235 + 1;
      E231 := E231 + 1;
      PSL.NODES'ELAB_BODY;
      E233 := E233 + 1;
      E237 := E237 + 1;
      E229 := E229 + 1;
      PSL.NFAS'ELAB_BODY;
      E343 := E343 + 1;
      E345 := E345 + 1;
      E361 := E361 + 1;
      PSL.CSE'ELAB_BODY;
      E359 := E359 + 1;
      E363 := E363 + 1;
      E365 := E365 + 1;
      PSL.QM'ELAB_SPEC;
      E367 := E367 + 1;
      PSL.BUILD'ELAB_BODY;
      E357 := E357 + 1;
      E369 := E369 + 1;
      E347 := E347 + 1;
      E428 := E428 + 1;
      E420 := E420 + 1;
      E418 := E418 + 1;
      Netlists'Elab_Body;
      E415 := E415 + 1;
      E426 := E426 + 1;
      E441 := E441 + 1;
      E447 := E447 + 1;
      E459 := E459 + 1;
      Netlists.Locations'Elab_Body;
      E424 := E424 + 1;
      E492 := E492 + 1;
      E488 := E488 + 1;
      E445 := E445 + 1;
      E422 := E422 + 1;
      E439 := E439 + 1;
      E443 := E443 + 1;
      E449 := E449 + 1;
      E490 := E490 + 1;
      E437 := E437 + 1;
      E406 := E406 + 1;
      E457 := E457 + 1;
      E455 := E455 + 1;
      E119 := E119 + 1;
      Vhdl.Flists'Elab_Body;
      E225 := E225 + 1;
      Vhdl.Lists'Elab_Body;
      E212 := E212 + 1;
      E216 := E216 + 1;
      Vhdl.Nodes'Elab_Body;
      E210 := E210 + 1;
      Elab.Vhdl_Objtypes'Elab_Spec;
      E200 := E200 + 1;
      E391 := E391 + 1;
      Elab.Vhdl_Heap'Elab_Body;
      E413 := E413 + 1;
      E404 := E404 + 1;
      Synth.Ieee.Numeric_Std'Elab_Body;
      E472 := E472 + 1;
      Trans'Elab_Spec;
      Trans'Elab_Body;
      E318 := E318 + 1;
      E296 := E296 + 1;
      Vhdl.Elocations'Elab_Body;
      E294 := E294 + 1;
      E284 := E284 + 1;
      E260 := E260 + 1;
      E286 := E286 + 1;
      E282 := E282 + 1;
      E288 := E288 + 1;
      E258 := E258 + 1;
      E290 := E290 + 1;
      E463 := E463 + 1;
      Vhdl.Annotations'Elab_Body;
      E400 := E400 + 1;
      Elab.Vhdl_Context'Elab_Spec;
      Elab.Vhdl_Context'Elab_Body;
      E389 := E389 + 1;
      E402 := E402 + 1;
      E246 := E246 + 1;
      E494 := E494 + 1;
      Vhdl.Ieee.Numeric'Elab_Body;
      E500 := E500 + 1;
      Vhdl.Ieee.Numeric_Std_Unsigned'Elab_Body;
      E514 := E514 + 1;
      Vhdl.Ieee.Std_Logic_Arith'Elab_Body;
      E516 := E516 + 1;
      Vhdl.Ieee.Std_Logic_Misc'Elab_Body;
      E518 := E518 + 1;
      Vhdl.Ieee.Std_Logic_Unsigned'Elab_Body;
      E520 := E520 + 1;
      E538 := E538 + 1;
      Vhdl.Prints'Elab_Spec;
      Vhdl.Prints'Elab_Body;
      E530 := E530 + 1;
      E553 := E553 + 1;
      Vhdl.Scanner'Elab_Body;
      E301 := E301 + 1;
      Vhdl.Sem_Inst'Elab_Body;
      E504 := E504 + 1;
      Vhdl.Sem_Scopes'Elab_Body;
      E524 := E524 + 1;
      E528 := E528 + 1;
      Vhdl.Xrefs'Elab_Body;
      E305 := E305 + 1;
      E292 := E292 + 1;
      E298 := E298 + 1;
      Options'Elab_Spec;
      Libraries'Elab_Body;
      E195 := E195 + 1;
      E197 := E197 + 1;
      E510 := E510 + 1;
      Vhdl.Configuration'Elab_Spec;
      Synthesis'Elab_Spec;
      Elab.Vhdl_Files'Elab_Spec;
      Elab.Debugger'Elab_Body;
      E387 := E387 + 1;
      E375 := E375 + 1;
      Synth.Vhdl_Environment'Elab_Spec;
      E522 := E522 + 1;
      E512 := E512 + 1;
      E508 := E508 + 1;
      Vhdl.Configuration'Elab_Body;
      E547 := E547 + 1;
      E543 := E543 + 1;
      E545 := E545 + 1;
      E381 := E381 + 1;
      E486 := E486 + 1;
      E465 := E465 + 1;
      E385 := E385 + 1;
      E467 := E467 + 1;
      E496 := E496 + 1;
      E379 := E379 + 1;
      E377 := E377 + 1;
      E536 := E536 + 1;
      E534 := E534 + 1;
      E532 := E532 + 1;
      E506 := E506 + 1;
      E526 := E526 + 1;
      E502 := E502 + 1;
      E498 := E498 + 1;
      E408 := E408 + 1;
      E373 := E373 + 1;
      E355 := E355 + 1;
      E371 := E371 + 1;
      E435 := E435 + 1;
      E482 := E482 + 1;
      Synth.Vhdl_Insts'Elab_Body;
      E484 := E484 + 1;
      Synth.Vhdl_Stmts'Elab_Body;
      E480 := E480 + 1;
      E461 := E461 + 1;
      E411 := E411 + 1;
      E430 := E430 + 1;
      Synth.Vhdl_Context'Elab_Body;
      E432 := E432 + 1;
      Trans.Chap8'Elab_Spec;
      Trans.Rtis'Elab_Spec;
      E351 := E351 + 1;
      E555 := E555 + 1;
      E311 := E311 + 1;
      Trans.Chap8'Elab_Body;
      E349 := E349 + 1;
      E341 := E341 + 1;
      E339 := E339 + 1;
      E337 := E337 + 1;
      Trans.Chap7'Elab_Body;
      E335 := E335 + 1;
      Trans.Rtis'Elab_Body;
      E551 := E551 + 1;
      E331 := E331 + 1;
      E333 := E333 + 1;
      E353 := E353 + 1;
      E329 := E329 + 1;
      E327 := E327 + 1;
      E325 := E325 + 1;
      E309 := E309 + 1;
      Ortho_Front'Elab_Body;
      E117 := E117 + 1;
   end adainit;

   procedure Ada_Main_Program;
   pragma Import (Ada, Ada_Main_Program, "_ada_ortho_code_main");

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
   --   ./psl.o
   --   ./synth.o
   --   ./synth-ieee.o
   --   ./types.o
   --   ./vhdl.o
   --   ./vhdl-nodes_priv.o
   --   ./vhdl-tokens.o
   --   ./vhdl-types.o
   --   ./areapools.o
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
   --   ./ortho_ident.o
   --   ./ortho_llvm.o
   --   ./ortho_nodes.o
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
   --   ./psl-nodes.o
   --   ./psl-nodes_meta.o
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
   --   ./trans_decls.o
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
   --   ./trans.o
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
   --   ./trans_analyzes.o
   --   ./vhdl-scanner.o
   --   ./vhdl-sem_inst.o
   --   ./vhdl-sem_scopes.o
   --   ./vhdl-std_env.o
   --   ./vhdl-xrefs.o
   --   ./vhdl-parse.o
   --   ./vhdl-parse_psl.o
   --   ./libraries.o
   --   ./options.o
   --   ./vhdl-nodes_gc.o
   --   ./elab-debugger.o
   --   ./vhdl-sem_assocs.o
   --   ./vhdl-ieee-vital_timing.o
   --   ./vhdl-post_sems.o
   --   ./vhdl-sem_lib.o
   --   ./vhdl-configuration.o
   --   ./elab-vhdl_insts.o
   --   ./elab-vhdl_stmts.o
   --   ./elab-vhdl_decls.o
   --   ./synthesis.o
   --   ./synth-vhdl_eval.o
   --   ./elab-vhdl_expr.o
   --   ./elab-vhdl_files.o
   --   ./synth-vhdl_static_proc.o
   --   ./elab-vhdl_types.o
   --   ./vhdl-evaluation.o
   --   ./vhdl-sem_types.o
   --   ./vhdl-sem_stmts.o
   --   ./vhdl-sem_psl.o
   --   ./vhdl-sem_names.o
   --   ./vhdl-sem_specs.o
   --   ./vhdl-sem_decls.o
   --   ./vhdl-sem_expr.o
   --   ./elab-vhdl_debug.o
   --   ./vhdl-sem.o
   --   ./vhdl-canon.o
   --   ./vhdl-canon_psl.o
   --   ./synth-vhdl_environment.o
   --   ./synth-vhdl_decls.o
   --   ./synth-vhdl_insts.o
   --   ./synth-vhdl_stmts.o
   --   ./synth-vhdl_oper.o
   --   ./synth-vhdl_expr.o
   --   ./synth-vhdl_aggr.o
   --   ./synth-vhdl_context.o
   --   ./trans-foreach_non_composite.o
   --   ./trans-chap12.o
   --   ./translation.o
   --   ./trans-chap8.o
   --   ./trans-chap9.o
   --   ./trans-chap5.o
   --   ./trans-chap4.o
   --   ./trans-chap7.o
   --   ./trans-rtis.o
   --   ./trans-chap14.o
   --   ./trans-chap6.o
   --   ./trans-helpers2.o
   --   ./trans-chap3.o
   --   ./trans-chap2.o
   --   ./trans-chap1.o
   --   ./trans_be.o
   --   ./ortho_front.o
   --   ./ortho_code_main.o
   --   -L./
   --   -L/usr/lib/gcc/x86_64-linux-gnu/9/adalib/
   --   -shared
   --   -lm
   --   -lgnat-9
   --   -ldl
--  END Object file/option list   

end ada_main;
