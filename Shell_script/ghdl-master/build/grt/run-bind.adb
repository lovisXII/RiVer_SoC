pragma Warnings (Off);
pragma Ada_95;
pragma Source_File_Name (grt_main, Spec_File_Name => "run-bind.ads");
pragma Source_File_Name (grt_main, Body_File_Name => "run-bind.adb");
pragma Suppress (Overflow_Check);

package body grt_main is

   E019 : Short_Integer; pragma Import (Ada, E019, "grt__hooks_E");
   E211 : Short_Integer; pragma Import (Ada, E211, "grt__algos_E");
   E213 : Short_Integer; pragma Import (Ada, E213, "grt__avls_E");
   E049 : Short_Integer; pragma Import (Ada, E049, "grt__rtis_addr_E");
   E076 : Short_Integer; pragma Import (Ada, E076, "grt__stack2_E");
   E039 : Short_Integer; pragma Import (Ada, E039, "grt__to_strings_E");
   E043 : Short_Integer; pragma Import (Ada, E043, "grt__vstrings_io_E");
   E037 : Short_Integer; pragma Import (Ada, E037, "grt__rtis_utils_E");
   E035 : Short_Integer; pragma Import (Ada, E035, "grt__avhpi_E");
   E052 : Short_Integer; pragma Import (Ada, E052, "grt__avhpi_utils_E");
   E193 : Short_Integer; pragma Import (Ada, E193, "grt__psl_E");
   E080 : Short_Integer; pragma Import (Ada, E080, "grt__stats_E");
   E067 : Short_Integer; pragma Import (Ada, E067, "grt__signals_E");
   E074 : Short_Integer; pragma Import (Ada, E074, "grt__unithread_E");
   E061 : Short_Integer; pragma Import (Ada, E061, "grt__errors_exec_E");
   E056 : Short_Integer; pragma Import (Ada, E056, "grt__backtraces_E");
   E059 : Short_Integer; pragma Import (Ada, E059, "grt__backtraces__jit_E");
   E065 : Short_Integer; pragma Import (Ada, E065, "grt__disp_E");
   E063 : Short_Integer; pragma Import (Ada, E063, "grt__processes_E");
   E069 : Short_Integer; pragma Import (Ada, E069, "grt__disp_signals_E");
   E090 : Short_Integer; pragma Import (Ada, E090, "grt__disp_rti_E");
   E092 : Short_Integer; pragma Import (Ada, E092, "grt__disp_tree_E");
   E084 : Short_Integer; pragma Import (Ada, E084, "grt__files_E");
   E086 : Short_Integer; pragma Import (Ada, E086, "grt__images_E");
   E054 : Short_Integer; pragma Import (Ada, E054, "grt__lib_E");
   E216 : Short_Integer; pragma Import (Ada, E216, "grt__names_E");
   E187 : Short_Integer; pragma Import (Ada, E187, "grt__rtis_types_E");
   E202 : Short_Integer; pragma Import (Ada, E202, "grt__sdf_E");
   E200 : Short_Integer; pragma Import (Ada, E200, "grt__vital_annotate_E");
   E206 : Short_Integer; pragma Import (Ada, E206, "grt__std_logic_1164_E");
   E082 : Short_Integer; pragma Import (Ada, E082, "grt__values_E");
   E033 : Short_Integer; pragma Import (Ada, E033, "grt__change_generics_E");
   E189 : Short_Integer; pragma Import (Ada, E189, "grt__vcd_E");
   E094 : Short_Integer; pragma Import (Ada, E094, "grt__fst_E");
   E198 : Short_Integer; pragma Import (Ada, E198, "grt__vhpi_E");
   E209 : Short_Integer; pragma Import (Ada, E209, "grt__waves_E");
   E195 : Short_Integer; pragma Import (Ada, E195, "grt__vcdz_E");
   E204 : Short_Integer; pragma Import (Ada, E204, "grt__vpi_E");
   E088 : Short_Integer; pragma Import (Ada, E088, "grt__modules_E");
   E031 : Short_Integer; pragma Import (Ada, E031, "grt__main_E");
   E002 : Short_Integer; pragma Import (Ada, E002, "ghdl_main_E");

   Sec_Default_Sized_Stacks : array (1 .. 1) of aliased System.Secondary_Stack.SS_Stack (System.Parameters.Runtime_Default_Sec_Stack_Size);


   procedure grt_init is
      Binder_Sec_Stacks_Count : Natural;
      pragma Import (Ada, Binder_Sec_Stacks_Count, "__gnat_binder_ss_count");

      Default_Secondary_Stack_Size : System.Parameters.Size_Type;
      pragma Import (C, Default_Secondary_Stack_Size, "__gnat_default_ss_size");
      Default_Sized_SS_Pool : System.Address;
      pragma Import (Ada, Default_Sized_SS_Pool, "__gnat_default_ss_pool");

   begin
      null;

      grt_main'Elab_Body;
      Default_Secondary_Stack_Size := System.Parameters.Runtime_Default_Sec_Stack_Size;
      Binder_Sec_Stacks_Count := 1;
      Default_Sized_SS_Pool := Sec_Default_Sized_Stacks'Address;


      if E019 = 0 then
         Grt.Hooks'Elab_Spec;
      end if;
      E019 := E019 + 1;
      E211 := E211 + 1;
      E213 := E213 + 1;
      E049 := E049 + 1;
      E076 := E076 + 1;
      E039 := E039 + 1;
      E043 := E043 + 1;
      E037 := E037 + 1;
      E035 := E035 + 1;
      E052 := E052 + 1;
      E193 := E193 + 1;
      if E067 = 0 then
         Grt.Signals'Elab_Spec;
      end if;
      if E074 = 0 then
         Grt.Unithread'Elab_Body;
      end if;
      E074 := E074 + 1;
      E059 := E059 + 1;
      E056 := E056 + 1;
      E065 := E065 + 1;
      if E067 = 0 then
         Grt.Signals'Elab_Body;
      end if;
      E067 := E067 + 1;
      E061 := E061 + 1;
      if E080 = 0 then
         Grt.Stats'Elab_Body;
      end if;
      E080 := E080 + 1;
      if E063 = 0 then
         Grt.Processes'Elab_Body;
      end if;
      E063 := E063 + 1;
      E069 := E069 + 1;
      E090 := E090 + 1;
      E092 := E092 + 1;
      if E084 = 0 then
         Grt.Files'Elab_Body;
      end if;
      E084 := E084 + 1;
      E086 := E086 + 1;
      E054 := E054 + 1;
      E216 := E216 + 1;
      E187 := E187 + 1;
      if E200 = 0 then
         Grt.Vital_Annotate'Elab_Body;
      end if;
      E200 := E200 + 1;
      E202 := E202 + 1;
      E206 := E206 + 1;
      E082 := E082 + 1;
      E033 := E033 + 1;
      if E189 = 0 then
         Grt.Vcd'Elab_Body;
      end if;
      E189 := E189 + 1;
      if E094 = 0 then
         Grt.Fst'Elab_Body;
      end if;
      E094 := E094 + 1;
      if E198 = 0 then
         Grt.Vhpi'Elab_Body;
      end if;
      E198 := E198 + 1;
      if E209 = 0 then
         Grt.Waves'Elab_Body;
      end if;
      E209 := E209 + 1;
      E195 := E195 + 1;
      if E204 = 0 then
         Grt.Vpi'Elab_Body;
      end if;
      E204 := E204 + 1;
      E088 := E088 + 1;
      E031 := E031 + 1;
      E002 := E002 + 1;
   end grt_init;

--  BEGIN Object file/option list
   --   ./grt.o
   --   ./grt-c.o
   --   ./grt-fcvt.o
   --   ./grt-severity.o
   --   ./grt-stdio.o
   --   ./grt-strings.o
   --   ./grt-types.o
   --   ./grt-astdio.o
   --   ./grt-rtis.o
   --   ./grt-rtis_binding.o
   --   ./grt-vhdl_types.o
   --   ./grt-callbacks.o
   --   ./grt-hooks.o
   --   ./grt-astdio-vhdl.o
   --   ./grt-options.o
   --   ./grt-errors.o
   --   ./grt-wave_opt.o
   --   ./grt-wave_opt-file.o
   --   ./grt-shadow_ieee.o
   --   ./grt-vstrings.o
   --   ./grt-wave_opt-design.o
   --   ./grt-algos.o
   --   ./grt-avls.o
   --   ./grt-fst_api.o
   --   ./grt-ghw.o
   --   ./grt-rtis_addr.o
   --   ./grt-stack2.o
   --   ./grt-table.o
   --   ./grt-to_strings.o
   --   ./grt-vstrings_io.o
   --   ./grt-rtis_utils.o
   --   ./grt-avhpi.o
   --   ./grt-avhpi_utils.o
   --   ./grt-psl.o
   --   ./grt-unithread.o
   --   ./grt-threads.o
   --   ./grt-backtraces-jit.o
   --   ./grt-backtraces-impl.o
   --   ./grt-backtraces.o
   --   ./grt-disp.o
   --   ./grt-signals.o
   --   ./grt-errors_exec.o
   --   ./grt-stats.o
   --   ./grt-processes.o
   --   ./grt-disp_signals.o
   --   ./grt-disp_rti.o
   --   ./grt-disp_tree.o
   --   ./grt-files.o
   --   ./grt-images.o
   --   ./grt-lib.o
   --   ./grt-names.o
   --   ./grt-rtis_types.o
   --   ./grt-vital_annotate.o
   --   ./grt-sdf.o
   --   ./grt-std_logic_1164.o
   --   ./grt-values.o
   --   ./grt-change_generics.o
   --   ./grt-vcd.o
   --   ./grt-fst.o
   --   ./grt-vhpi.o
   --   ./grt-waves.o
   --   ./grt-zlib.o
   --   ./grt-vcdz.o
   --   ./version.o
   --   ./grt-vpi.o
   --   ./grt-modules.o
   --   ./grt-main.o
   --   ./ghdl_main.o
   --   -L./
   --   -L/usr/lib/gcc/x86_64-linux-gnu/9/adalib/
   --   -lz
   --   -ldl
--  END Object file/option list   

end grt_main;
