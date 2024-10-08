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
      RK(i+1) <= RK(i)(351 downto 348) & RK(i)(287 downto 284) & RK(i)(271 downto 268) & RK(i)(291 downto 288) & RK(i)(347 downto 344) & RK(i)(259 downto 256) & RK(i)(327 downto 324) & RK(i)(315 downto 312) & RK(i)(303 downto 300) & RK(i)(307 downto 304) & RK(i)(279 downto 276) & RK(i)(363 downto 360) & RK(i)(311 downto 308) & RK(i)(375 downto 372) & RK(i)(359 downto 356) & RK(i)(263 downto 260) & RK(i)(367 downto 364) & RK(i)(323 downto 320) & RK(i)(339 downto 336) & RK(i)(283 downto 280) & RK(i)(343 downto 340) & RK(i)(267 downto 264) & RK(i)(335 downto 332) & RK(i)(371 downto 368) & RK(i)(299 downto 296) & RK(i)(319 downto 316) & RK(i)(355 downto 352) & RK(i)(383 downto 380) & RK(i)(275 downto 272) & RK(i)(331 downto 328) & RK(i)(295 downto 292) & RK(i)(379 downto 376) & (RK(i)(223) xor RK(i)(220)) & RK(i)(223 downto 221) & (RK(i)(159) xor RK(i)(156)) & RK(i)(159 downto 157) & (RK(i)(143) xor RK(i)(140)) & RK(i)(143 downto 141) & (RK(i)(163) xor RK(i)(160)) & RK(i)(163 downto 161) & (RK(i)(219) xor RK(i)(216)) & RK(i)(219 downto 217) & (RK(i)(131) xor RK(i)(128)) & RK(i)(131 downto 129) & (RK(i)(199) xor RK(i)(196)) & RK(i)(199 downto 197) & (RK(i)(187) xor RK(i)(184)) & RK(i)(187 downto 185) & (RK(i)(175) xor RK(i)(172)) & RK(i)(175 downto 173) & (RK(i)(179) xor RK(i)(176)) & RK(i)(179 downto 177) & (RK(i)(151) xor RK(i)(148)) & RK(i)(151 downto 149) & (RK(i)(235) xor RK(i)(232)) & RK(i)(235 downto 233) & (RK(i)(183) xor RK(i)(180)) & RK(i)(183 downto 181) & (RK(i)(247) xor RK(i)(244)) & RK(i)(247 downto 245) & (RK(i)(231) xor RK(i)(228)) & RK(i)(231 downto 229) & (RK(i)(135) xor RK(i)(132)) & RK(i)(135 downto 133) & (RK(i)(239) xor RK(i)(236)) & RK(i)(239 downto 237) & (RK(i)(195) xor RK(i)(192)) & RK(i)(195 downto 193) & (RK(i)(211) xor RK(i)(208)) & RK(i)(211 downto 209) & (RK(i)(155) xor RK(i)(152)) & RK(i)(155 downto 153) & (RK(i)(215) xor RK(i)(212)) & RK(i)(215 downto 213) & (RK(i)(139) xor RK(i)(136)) & RK(i)(139 downto 137) & (RK(i)(207) xor RK(i)(204)) & RK(i)(207 downto 205) & (RK(i)(243) xor RK(i)(240)) & RK(i)(243 downto 241) & (RK(i)(171) xor RK(i)(168)) & RK(i)(171 downto 169) & (RK(i)(191) xor RK(i)(188)) & RK(i)(191 downto 189) & (RK(i)(227) xor RK(i)(224)) & RK(i)(227 downto 225) & (RK(i)(255) xor RK(i)(252)) & RK(i)(255 downto 253) & (RK(i)(147) xor RK(i)(144)) & RK(i)(147 downto 145) & (RK(i)(203) xor RK(i)(200)) & RK(i)(203 downto 201) & (RK(i)(167) xor RK(i)(164)) & RK(i)(167 downto 165) & (RK(i)(251) xor RK(i)(248)) & RK(i)(251 downto 249) & RK(i)(94 downto 92) & (RK(i)(95) xor RK(i)(94)) & RK(i)(30 downto 28) & (RK(i)(31) xor RK(i)(30)) & RK(i)(14 downto 12) & (RK(i)(15) xor RK(i)(14)) & RK(i)(34 downto 32) & (RK(i)(35) xor RK(i)(34)) & RK(i)(90 downto 88) & (RK(i)(91) xor RK(i)(90)) & RK(i)(2 downto 0) & (RK(i)(3) xor RK(i)(2)) & RK(i)(70 downto 68) & (RK(i)(71) xor RK(i)(70)) & RK(i)(58 downto 56) & (RK(i)(59) xor RK(i)(58)) & RK(i)(46 downto 44) & (RK(i)(47) xor RK(i)(46)) & RK(i)(50 downto 48) & (RK(i)(51) xor RK(i)(50)) & RK(i)(22 downto 20) & (RK(i)(23) xor RK(i)(22)) & RK(i)(106 downto 104) & (RK(i)(107) xor RK(i)(106)) & RK(i)(54 downto 52) & (RK(i)(55) xor RK(i)(54)) & RK(i)(118 downto 116) & (RK(i)(119) xor RK(i)(118)) & RK(i)(102 downto 100) & (RK(i)(103) xor RK(i)(102)) & RK(i)(6 downto 4) & (RK(i)(7) xor RK(i)(6)) & RK(i)(110 downto 108) & (RK(i)(111) xor RK(i)(110)) & RK(i)(66 downto 64) & (RK(i)(67) xor RK(i)(66)) & RK(i)(82 downto 80) & (RK(i)(83) xor RK(i)(82)) & RK(i)(26 downto 24) & (RK(i)(27) xor RK(i)(26)) & RK(i)(86 downto 84) & (RK(i)(87) xor RK(i)(86)) & RK(i)(10 downto 8) & (RK(i)(11) xor RK(i)(10)) & RK(i)(78 downto 76) & (RK(i)(79) xor RK(i)(78)) & RK(i)(114 downto 112) & (RK(i)(115) xor RK(i)(114)) & RK(i)(42 downto 40) & (RK(i)(43) xor RK(i)(42)) & RK(i)(62 downto 60) & (RK(i)(63) xor RK(i)(62)) & RK(i)(98 downto 96) & (RK(i)(99) xor RK(i)(98)) & RK(i)(126 downto 124) & (RK(i)(127) xor RK(i)(126)) & RK(i)(18 downto 16) & (RK(i)(19) xor RK(i)(18)) & RK(i)(74 downto 72) & (RK(i)(75) xor RK(i)(74)) & RK(i)(38 downto 36) & (RK(i)(39) xor RK(i)(38)) & RK(i)(122 downto 120) & (RK(i)(123) xor RK(i)(122));
      RKXOR(i) <= RK(i+1)(K - 1 downto K - N) xor RK(i+1)(K - N - 1 downto K - N - N) xor RK(i+1)(K - N - N - 1 downto 0);
    end generate;


    RS0(0) <= Plaintext xor RK(0)(K - 1 downto K - N) xor RK(0)(K - N - 1 downto K - N - N) xor RK(0)(K - N - N - 1 downto 0);

    -- Round Function
    ROUND_FUNCTION : for i in 0 to (R-2) generate
      SubNib_i : SubNib port map(State_DI => RS0(i), State_DO => RS1(i));
      -- AddConst
      RS2(i) <= (RS1(i)(N - 1 downto N - 8) xor lfsr(i)) & (RS1(i)(N - 9 downto N - 24) xor x"5a5a") & (RS1(i)(N - 25 downto N - 32) xor pi(i)) & RS1(i)(N - 33 downto 0);
      PosPerm_i : PosPerm port map(State_DI => RS2(i), State_DO => RS3(i));
      -- MixCol and AddRoundKey
      MixColAddKey_i : MixColAddKey port map(State_DI => RS3(i), Key_DI => RKXOR(i), State_DO => RS0(i+1));
    end generate;

    -- Last Round Function
    SubNib_R : SubNib port map(State_DI => RS0(R-1), State_DO => RS1(R-1));
    -- AddRoundKey
    Ciphertext <= RS1(R-1) xor RKXOR(R-1);

end Behavioral;