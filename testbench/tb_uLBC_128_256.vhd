library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity TB_uLBC is
end TB_uLBC;

architecture Behavioral of TB_uLBC is

    component cipher_top is
        Port ( Plaintext : in STD_LOGIC_VECTOR (127 downto 0);
               Key : in STD_LOGIC_VECTOR (255 downto 0);
               Ciphertext : out STD_LOGIC_VECTOR (127 downto 0));
    end component;
           
    constant clk_period : time := 10 ns;
    
    signal clk : STD_LOGIC;
    signal Key : STD_LOGIC_VECTOR(255 downto 0);
    signal Plaintext, Ciphertext : STD_LOGIC_VECTOR(127 downto 0);
    
begin

    UUT : cipher_top Port Map (Plaintext, Key, Ciphertext);

    -- Clock Process
    clk_proc : process
    begin
        clk <= '1';
        wait for clk_period/2;
        clk <= '0';
        wait for clk_period/2;
    end process;

    -- Stimulation Process
    stim_proc : process
    begin

        Plaintext <= x"2d643085_0cad830b_53764dad_7be5c3a9";
        Key <= x"2990cbcb_613391be_00a582cd_722e610f_42e65a59_2135ab5f_4a85fb01_222224e0";

        wait for 3*clk_period;

        if (Ciphertext = x"b6cfa6a5_90dba5ea_a5a7004e_5a8339f4") then
            report "SUCCESS";
        else
            report "FAILURE";
        end if;

        Plaintext <= x"00000000_00000000_00000000_00000000";
        Key <= x"00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000";

        wait for 3*clk_period;

        if (Ciphertext = x"fb0be725_0cfd59a9_d2712fb1_899ff396") then
            report "SUCCESS";
        else
            report "FAILURE";
        end if;
        
        wait;
    end process;

end Behavioral;