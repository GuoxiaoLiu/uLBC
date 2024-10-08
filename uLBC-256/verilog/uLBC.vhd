library work;
use work.uLBC_pkg.all;

library ieee;
use ieee.std_logic_1164.all;

entity cipher_top is
    Port ( Plaintext : in STD_LOGIC_VECTOR (N - 1 downto 0);
           Key : in STD_LOGIC_VECTOR (K - 1 downto 0);
           Ciphertext : out STD_LOGIC_VECTOR (N - 1 downto 0));
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
    signal RS0, RS1, RKXOR : RS1_ARR;
    signal RS2, RS3 : RS2_ARR;
    

begin

    -- Key Schedule
    RK(0) <= Key;
    KEY_SCHEDULE : for i in 0 to (R-1) generate
      RK(i+1) <= RK(i)(223 downto 220) & RK(i)(159 downto 156) & RK(i)(143 downto 140) & RK(i)(163 downto 160) & RK(i)(219 downto 216) & RK(i)(131 downto 128) & RK(i)(199 downto 196) & RK(i)(187 downto 184) & RK(i)(175 downto 172) & RK(i)(179 downto 176) & RK(i)(151 downto 148) & RK(i)(235 downto 232) & RK(i)(183 downto 180) & RK(i)(247 downto 244) & RK(i)(231 downto 228) & RK(i)(135 downto 132) & RK(i)(239 downto 236) & RK(i)(195 downto 192) & RK(i)(211 downto 208) & RK(i)(155 downto 152) & RK(i)(215 downto 212) & RK(i)(139 downto 136) & RK(i)(207 downto 204) & RK(i)(243 downto 240) & RK(i)(171 downto 168) & RK(i)(191 downto 188) & RK(i)(227 downto 224) & RK(i)(255 downto 252) & RK(i)(147 downto 144) & RK(i)(203 downto 200) & RK(i)(167 downto 164) & RK(i)(251 downto 248)
               & (RK(i)(95) xor RK(i)(92)) & RK(i)(95 downto 93) & (RK(i)(31) xor RK(i)(28)) & RK(i)(31 downto 29) & (RK(i)(15) xor RK(i)(12)) & RK(i)(15 downto 13) & (RK(i)(35) xor RK(i)(32)) & RK(i)(35 downto 33) & (RK(i)(91) xor RK(i)(88)) & RK(i)(91 downto 89) & (RK(i)(3) xor RK(i)(0)) & RK(i)(3 downto 1) & (RK(i)(71) xor RK(i)(68)) & RK(i)(71 downto 69) & (RK(i)(59) xor RK(i)(56)) & RK(i)(59 downto 57) & (RK(i)(47) xor RK(i)(44)) & RK(i)(47 downto 45) & (RK(i)(51) xor RK(i)(48)) & RK(i)(51 downto 49) & (RK(i)(23) xor RK(i)(20)) & RK(i)(23 downto 21) & (RK(i)(107) xor RK(i)(104)) & RK(i)(107 downto 105) & (RK(i)(55) xor RK(i)(52)) & RK(i)(55 downto 53) & (RK(i)(119) xor RK(i)(116)) & RK(i)(119 downto 117) & (RK(i)(103) xor RK(i)(100)) & RK(i)(103 downto 101) & (RK(i)(7) xor RK(i)(4)) & RK(i)(7 downto 5) & (RK(i)(111) xor RK(i)(108)) & RK(i)(111 downto 109) & (RK(i)(67) xor RK(i)(64)) & RK(i)(67 downto 65) & (RK(i)(83) xor RK(i)(80)) & RK(i)(83 downto 81) & (RK(i)(27) xor RK(i)(24)) & RK(i)(27 downto 25) & (RK(i)(87) xor RK(i)(84)) & RK(i)(87 downto 85) & (RK(i)(11) xor RK(i)(8)) & RK(i)(11 downto 9) & (RK(i)(79) xor RK(i)(76)) & RK(i)(79 downto 77) & (RK(i)(115) xor RK(i)(112)) & RK(i)(115 downto 113) & (RK(i)(43) xor RK(i)(40)) & RK(i)(43 downto 41) & (RK(i)(63) xor RK(i)(60)) & RK(i)(63 downto 61) & (RK(i)(99) xor RK(i)(96)) & RK(i)(99 downto 97) & (RK(i)(127) xor RK(i)(124)) & RK(i)(127 downto 125) & (RK(i)(19) xor RK(i)(16)) & RK(i)(19 downto 17) & (RK(i)(75) xor RK(i)(72)) & RK(i)(75 downto 73) & (RK(i)(39) xor RK(i)(36)) & RK(i)(39 downto 37) & (RK(i)(123) xor RK(i)(120)) & RK(i)(123 downto 121);
      RKXOR(i) <= RK(i+1)(K - 1 downto K - N) xor RK(i+1)(K - N - 1 downto 0);
    end generate;


    RS0(0) <= Plaintext xor RK(0)(K - 1 downto K - N) xor RK(0)(K - N - 1 downto 0);

    -- Round Function
    ROUND_FUNCTION : for i in 0 to (R-2) generate
      SubNib_i : SubNib port map(State_DI => RS0(i), State_DO => RS1(i));
      -- AddConst
      RS2(i) <= (RS1(i)(N - 1 downto N - 8) xor lfsr(i)) & (RS1(i)(N - 9 downto N - 24) xor x"3c3c") & (RS1(i)(N - 25 downto N - 32) xor pi(i)) & RS1(i)(N - 33 downto 0);
      PosPerm_i : PosPerm port map(State_DI => RS2(i), State_DO => RS3(i));
      -- MixCol and AddRoundKey
      MixColAddKey_i : MixColAddKey port map(State_DI => RS3(i), Key_DI => RKXOR(i), State_DO => RS0(i+1));
    end generate;

    -- Last Round Function
    SubNib_R : SubNib port map(State_DI => RS0(R-1), State_DO => RS1(R-1));
    -- AddRoundKey
    Ciphertext <= RS1(R-1) xor RKXOR(R-1);

end Behavioral;