library work;
use work.uLBC_pkg.all;

library ieee;
use ieee.std_logic_1164.all;

entity cipher_top is
    Port ( Ciphertext : in STD_LOGIC_VECTOR (N - 1 downto 0);
           Key : in STD_LOGIC_VECTOR (K - 1 downto 0);
           Plaintext : out STD_LOGIC_VECTOR (N - 1 downto 0));
end cipher_top;

architecture Behavioral of cipher_top is

    component SubNib
      port(
        -- Input signals
        signal State_DI : in std_logic_vector(N - 1 downto 0);
        -- Output signals
        signal State_DO : out std_logic_vector(N - 1 downto 0)
      );
    end component;
  
    component PosPerm is
      port (
          State_DI: in std_logic_vector(N - 1 downto 0);
          State_DO: out std_logic_vector(N - 1 downto 0)
      );
    end component;
  
    component MixColAddKey is
      port (
          State_DI: in std_logic_vector(N - 1 downto 0);
          Key_DI: in std_logic_vector(N - 1 downto 0);
          State_DO: out std_logic_vector(N - 1 downto 0)
      );
    end component;


    signal RK : RK_ARR;
    signal RS0, RS1 : RS1_ARR;
    signal RS2, RS3 : RS2_ARR;
    

begin

    -- key Schedule
    RK(0) <= Key;
    KEY_SCHEDULE : for i in 0 to (R-1) generate
      RK(i+1) <= RK(i)(95 downto 92) & RK(i)(31 downto 28) & RK(i)(15 downto 12) & RK(i)(35 downto 32) & RK(i)(91 downto 88) & RK(i)(3 downto 0) & RK(i)(71 downto 68) & RK(i)(59 downto 56) & RK(i)(47 downto 44) & RK(i)(51 downto 48) & RK(i)(23 downto 20) & RK(i)(107 downto 104) & RK(i)(55 downto 52) & RK(i)(119 downto 116) & RK(i)(103 downto 100) & RK(i)(7 downto 4) & RK(i)(111 downto 108) & RK(i)(67 downto 64) & RK(i)(83 downto 80) & RK(i)(27 downto 24) & RK(i)(87 downto 84) & RK(i)(11 downto 8) & RK(i)(79 downto 76) & RK(i)(115 downto 112) & RK(i)(43 downto 40) & RK(i)(63 downto 60) & RK(i)(99 downto 96) & RK(i)(127 downto 124) & RK(i)(19 downto 16) & RK(i)(75 downto 72) & RK(i)(39 downto 36) & RK(i)(123 downto 120);
    end generate;


    -- RS0(0) <= plaintext xor RK(0);

    -- -- Round Function
    -- ROUND_FUNCTION : for i in 0 to (R-2) generate
    --   SubNib_i : SubNib port map(State_DI => RS0(i), State_DO => RS1(i));
    --   -- AddConst
    --   RS2(i) <= (RS1(i)(N - 1 downto N - 8) xor lfsr(i)) & (RS1(i)(N - 9 downto N - 24) xor x"c5c5") & (RS1(i)(N - 25 downto N - 32) xor pi(i)) & RS1(i)(N - 33 downto 0);
    --   PosPerm_i : PosPerm port map(State_DI => RS2(i), State_DO => RS3(i));
    --   -- MixCol and AddRoundKey
    --   MixColAddKey_i : MixColAddKey port map(State_DI => RS3(i), Key_DI => RK(i+1), State_DO => RS0(i+1));
    -- end generate;

    -- -- Last Round Function
    -- SubNib_R : SubNib port map(State_DI => RS0(R-1), State_DO => RS1(R-1));
    -- -- AddRoundKey
    -- Ciphertext <= RS1(R-1) xor RK(R);


    -- decryption
    -- AddRoundKey
    RS1(R-1) <= Ciphertext xor RK(R);
    -- First Round Function
    SubNib_0 : SubNib port map(State_DI => RS1(R-1), State_DO => RS0(R-1));

    -- Round Function
    ROUND_FUNCTION : for i in (R-2) downto 0 generate
      -- MixCol and AddRoundKey
      MixColAddKey_i : MixColAddKey port map(State_DI => RS0(i+1), Key_DI => RK(i+1), State_DO => RS3(i));
      -- PosPerm
      PosPerm_i : PosPerm port map(State_DI => RS3(i), State_DO => RS2(i));
      -- AddConst
      RS1(i) <= (RS2(i)(N - 1 downto N - 8) xor lfsr(i)) & (RS2(i)(N - 9 downto N - 24) xor x"c5c5") & (RS2(i)(N - 25 downto N - 32) xor pi(i)) & RS2(i)(N - 33 downto 0);
      -- SubNib
      SubNib_i : SubNib port map(State_DI => RS1(i), State_DO => RS0(i));
    end generate;
    
    Plaintext <= RS0(0) xor RK(0);

end Behavioral;