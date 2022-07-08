library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity csr is 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        CSR_WADR_SM         : in std_logic_vector(11 downto 0);
        CSR_WDATA_SM        : in std_logic_vector(31 downto 0);
        CSR_ENABLE_SM       : in std_logic;

        EXCEPTION_SM        : in std_logic;
        MSTATUS_WDATA_SM    : in std_logic_vector(31 downto 0);
        MIP_WDATA_SM        : in std_logic_vector(31 downto 0);
        MEPC_WDATA_SM       : in std_logic_vector(31 downto 0);
        MCAUSE_WDATA_SM     : in std_logic_vector(31 downto 0);
        MTVAL_WDATA_SM      : in std_logic_vector(31 downto 0);

        MEPC_SC             : out std_logic_vector(31 downto 0);
        MSTATUS_RC          : out std_logic_vector(31 downto 0);
        MTVEC_VALUE_RC      : out std_logic_vector(31 downto 0);
        MIP_VALUE_RC        : out std_logic_vector(31 downto 0);
        MIE_VALUE_RC        : out std_logic_vector(31 downto 0);
        MCAUSE_SC           : out std_logic_vector(31 downto 0);
    
        CSR_RADR_SD         : in std_logic_vector(11 downto 0);
        CSR_RDATA_SC        : out std_logic_vector(31 downto 0)
    );
end csr; 

architecture archi of csr is 

-- CSR registers 
signal reg_mvendorid    : std_logic_vector(31 downto 0);
signal reg_marchid      : std_logic_vector(31 downto 0);
signal reg_mimpid       : std_logic_vector(31 downto 0);
signal reg_mstatus      : std_logic_vector(31 downto 0);
signal reg_misa         : std_logic_vector(31 downto 0);
signal reg_mie          : std_logic_vector(31 downto 0);
signal reg_mtvec        : std_logic_vector(31 downto 0);
signal reg_mstatush     : std_logic_vector(31 downto 0);
signal reg_mepc         : std_logic_vector(31 downto 0);
signal reg_mcause       : std_logic_vector(31 downto 0);
signal reg_mtval        : std_logic_vector(31 downto 0);
signal reg_mip          : std_logic_vector(31 downto 0);
signal reg_mscratch     : std_logic_vector(31 downto 0);

-- CSR adress 
constant adr_mvendorid    : std_logic_vector(11 downto 0) := x"F11";
constant adr_marchid      : std_logic_vector(11 downto 0) := x"F12";
constant adr_mimpid       : std_logic_vector(11 downto 0) := x"F13";
constant adr_mstatus      : std_logic_vector(11 downto 0) := x"300";
constant adr_misa         : std_logic_vector(11 downto 0) := x"301";
constant adr_mie          : std_logic_vector(11 downto 0) := x"304";
constant adr_mtvec        : std_logic_vector(11 downto 0) := x"305";
constant adr_mstatush     : std_logic_vector(11 downto 0) := x"310";
constant adr_mepc         : std_logic_vector(11 downto 0) := x"341";
constant adr_mcause       : std_logic_vector(11 downto 0) := x"342";
constant adr_mtval        : std_logic_vector(11 downto 0) := x"343";
constant adr_mip          : std_logic_vector(11 downto 0) := x"344";
constant adr_mscratch     : std_logic_vector(11 downto 0) := x"340";

begin 

csr_write : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        reg_mvendorid   <= x"00000000";       
        reg_marchid     <= x"00000000";    
        reg_mimpid      <= x"00000000";    
        reg_mstatus     <= x"00000000";    
        reg_misa        <= x"40100100";    
        reg_mie         <= x"00000000";    
        reg_mtvec       <= x"00000000";    
        reg_mstatush    <= x"00000000";    
        reg_mepc        <= x"00000000";    
        reg_mcause      <= x"00000000";    
        reg_mtval       <= x"00000000";    
        reg_mip         <= x"00000000";    
        reg_mscratch    <= x"00000000";  
    
    elsif rising_edge(clk) then 
        if EXCEPTION_SM = '1' then 
            reg_mstatus      <= MSTATUS_WDATA_SM;
            reg_mepc         <= MEPC_WDATA_SM;
            reg_mcause       <= MCAUSE_WDATA_SM;
            reg_mtval        <= MTVAL_WDATA_SM;
            reg_mip          <= MIP_WDATA_SM;
        elsif CSR_ENABLE_SM = '1' then 
            case (CSR_WADR_SM) is 
                when adr_mstatus        =>  reg_mstatus     <= CSR_WDATA_SM;
                when adr_mie            =>  reg_mie         <= CSR_WDATA_SM;
                when adr_mtvec          =>  reg_mtvec       <= CSR_WDATA_SM;
                when adr_mepc           =>  reg_mepc        <= CSR_WDATA_SM;
                when adr_mcause         =>  reg_mcause      <= CSR_WDATA_SM;
                when adr_mtval          =>  reg_mtval       <= CSR_WDATA_SM;
                when adr_mip            =>  reg_mip         <= CSR_WDATA_SM;
                when adr_mscratch       =>  reg_mscratch    <= CSR_WDATA_SM;
                when others             =>  NULL;
            end case; 
        end if; 
    end if; 
end process; 

-- Read
with CSR_RADR_SD select CSR_RDATA_SC <= 
    reg_mvendorid    when adr_mvendorid,    
    reg_marchid      when adr_marchid,    
    reg_mimpid       when adr_mimpid,    
    reg_mstatus      when adr_mstatus,    
    reg_misa         when adr_misa,    
    reg_mie          when adr_mie,    
    reg_mtvec        when adr_mtvec,    
    reg_mstatush     when adr_mstatush,    
    reg_mepc         when adr_mepc,    
    reg_mcause       when adr_mcause,    
    reg_mtval        when adr_mtval,    
    reg_mip          when adr_mip,    
    reg_mscratch     when adr_mscratch,
    x"00000000"      when others;  

MEPC_SC         <= reg_mepc;
MSTATUS_RC      <= reg_mstatus;
MTVEC_VALUE_RC  <= reg_mtvec;
MIP_VALUE_RC    <= reg_mip;
MIE_VALUE_RC    <= reg_mie; 
MCAUSE_SC       <= reg_mcause; 

end archi;