library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity TB_uLBC is
end TB_uLBC;

architecture Behavioral of TB_uLBC is

    component cipher_top is
        Port ( Plaintext : in STD_LOGIC_VECTOR (127 downto 0);
               Key : in STD_LOGIC_VECTOR (383 downto 0);
               Ciphertext : out STD_LOGIC_VECTOR (127 downto 0));
    end component;
           
    constant clk_period : time := 10 ns;
    
    signal clk : STD_LOGIC;
    signal Key : STD_LOGIC_VECTOR(383 downto 0);
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

        Plaintext <= x"2d7d941a_0f864e4b_3a7e25e3_2fe97815";
        Key <= x"08cac346_37da2d2c_54cbc059_0cc13e8d_62f1a185_65a35582_4642fdef_582d79f2_11a12c18_6c55e4b2_7b1999ae_4db8ada5";

        wait for 3*clk_period;

        if (Ciphertext = x"6bfe00ba_63397825_379e397e_fa11d15a") then
            report "SUCCESS";
        else
            report "FAILURE";
        end if;

        Plaintext <= x"00000000_00000000_00000000_00000000";
        Key <= x"00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000";

        wait for 3*clk_period;

        if (Ciphertext = x"64adeb02_32294d9c_1e268365_764e674a") then
            report "SUCCESS";
        else
            report "FAILURE";
        end if;

        wait;
    end process;

end Behavioral;