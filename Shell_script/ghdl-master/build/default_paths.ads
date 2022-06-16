--  GHDL driver paths  -*- ada -*-.
--  Copyright (C) 2002, 2003, 2004, 2005 Tristan Gingold
--
--  This program is free software: you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation, either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program.  If not, see <gnu.org/licenses>.

package Default_Paths is

   --  Accept long lines.
   pragma Style_Checks ("M999");

   Install_Prefix : constant String :=
     "/opt/ghdl";
   LibDir_Suffix : constant String :=
     "lib";
   LibGhdlDir_Suffix : constant String :=
     "lib/ghdl";
   IncDir_Suffix : constant String :=
     "include";

   Compiler_Gcc : constant String :=
     "ghdl1-gcc";
   Compiler_Mcode : constant String :=
     "ghdl1-mcode";
   Compiler_Llvm : constant String :=
     "ghdl1-llvm";
   Compiler_Debug : constant String :=
     "ghdl1-debug";
   Post_Processor : constant String :=
     "oread-llvm";

   Shared_Library_Extension : constant String := ".so";
   Default_Pie : constant Boolean := "true" = String'("true");
end Default_Paths;
