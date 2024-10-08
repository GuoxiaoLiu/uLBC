library work;
use work.uLBC_pkg.all;

library ieee;
use ieee.std_logic_1164.all;

entity SubNib is
  port(
    -- Input signals
    signal State_DI : in std_logic_vector(N - 1 downto 0);
    -- Output signals
    signal State_DO : out std_logic_vector(N - 1 downto 0)
  );
end entity;


architecture behavorial of SubNib is
  component SB
      port(
        -- Input signals
        signal input : in std_logic_vector(4 - 1 downto 0);
        -- Output signals
        signal output : out std_logic_vector(4 - 1 downto 0)
      );
    end component;
  
begin

  
  
  SBOX_GEN : for i in 0 to N / 4 - 1 generate
    SBOX_I : SB
    port map(
        input => State_DI(N - i * 4 - 1 downto N - i * 4 - 4),
        output => State_DO(N - i * 4 - 1 downto N - i * 4 - 4)
      );
  end generate SBOX_GEN;
  
end behavorial;