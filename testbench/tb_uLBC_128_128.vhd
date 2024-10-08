library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity TB_uLBC is
end TB_uLBC;

architecture Behavioral of TB_uLBC is

    component cipher_top is
        Port ( Plaintext : in STD_LOGIC_VECTOR (127 downto 0);
               Key : in STD_LOGIC_VECTOR (127 downto 0);
               Ciphertext : out STD_LOGIC_VECTOR (127 downto 0));
    end component;
           
    constant clk_period : time := 10 ns;
    
    signal clk : STD_LOGIC;
    signal Key : STD_LOGIC_VECTOR(127 downto 0);
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

        Plaintext <= x"2d33f9e1_4c935dc3_150e07ef_14909c24";
        Key <= x"2f5a39f6_5ef0d47b_4c2ae6b5_21fb0a08";

        wait for 3*clk_period;

        if (Ciphertext = x"5384bf47_150a1f05_059c1e70_4894ca21") then
            report "SUCCESS";
        else
            report "FAILURE";
        end if;

        Plaintext <= x"00000000_00000000_00000000_00000000";
        Key <= x"00000000_00000000_00000000_00000000";

        wait for 3*clk_period;

        if (Ciphertext = x"e6b6b249_c25994a4_25c480b8_52f4899a") then
            report "SUCCESS";
        else
            report "FAILURE";
        end if;

        wait;
    end process;

end Behavioral;