library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
entity SB is
	Port (  input : in STD_LOGIC_VECTOR (3 downto 0);
			output : out STD_LOGIC_VECTOR (3 downto 0));
end SB;
architecture Behavioral of SB is
begin
    with input select
    output <= "1000" when "0000",
              "0000" when "0001",
              "0001" when "0010",
              "0101" when "0011",
              "1100" when "0100",
              "0111" when "0101",
              "0100" when "0110",
              "0110" when "0111",
              "0010" when "1000",
              "1010" when "1001",
              "0011" when "1010",
              "1101" when "1011",
              "1110" when "1100",
              "1111" when "1101",
              "1011" when "1110",
              "1001" when others;
end Behavioral;