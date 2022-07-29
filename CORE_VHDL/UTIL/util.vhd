library ieee; 
use ieee.std_logic_1164.all;

package util is 
    constant PRED_REG_SIZE          :   integer :=  128;
    constant RET_PRED_REG_SIZE      :   integer :=  16;
    constant PRED_POINTER_SIZE      :   integer :=  7;
    constant RET_PRED_POINTER_SIZE  :   integer :=  4;
    constant RET_STACK_SIZE         :   integer :=  16;

    constant kernel_adr : std_logic_vector(31 downto 0) := x"F0000000"; 
    constant one_ext_32 : std_logic_vector(31 downto 0) := x"00000001";

    constant zero_ext_pred_size     :   std_logic_vector(PRED_POINTER_SIZE-1 downto 0) := (others => '0');
    constant zero_ext_ret_size      :   std_logic_vector(RET_PRED_POINTER_SIZE-1 downto 0) := (others => '0');
    constant zero_ret_stack_size    :   std_logic_vector(RET_STACK_SIZE-1 downto 0) := (others => '0');
    
    constant one_ext_pred_size      :   std_logic_vector(PRED_POINTER_SIZE-1 downto 0) := zero_ext_pred_size(PRED_POINTER_SIZE-1 downto 1) & '1';
    constant one_ext_ret_size       :   std_logic_vector(RET_PRED_POINTER_SIZE-1 downto 0) := zero_ext_pred_size(RET_PRED_POINTER_SIZE-1 downto 1) & '1';
    constant one_ext_ret_stack_size :   std_logic_vector(RET_STACK_SIZE-1 downto 0) := zero_ret_stack_size(RET_STACK_SIZE-1 downto 1) & '1'; 
    

    type tab_32_t   is array(0 to 31) of std_logic_vector(63 downto 0);
    type tab_20_t   is array(0 to 19) of std_logic_vector(63 downto 0);
    type tab_14_t   is array(0 to 13) of std_logic_vector(63 downto 0);
    type tab_10_t   is array(0 to 9)  of std_logic_vector(63 downto 0);
    type tab_6_t    is array(0 to 5)  of std_logic_vector(63 downto 0);
    type tab_5_t    is array(0 to 5)  of std_logic_vector(63 downto 0);
    type tab_4_t    is array(0 to 3)  of std_logic_vector(63 downto 0);
    type tab_2_t    is array(0 to 1)  of std_logic_vector(63 downto 0);

end util; 

package body util is 
    
end util; 