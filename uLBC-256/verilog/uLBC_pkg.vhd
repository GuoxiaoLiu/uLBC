library ieee;
use ieee.std_logic_1164.all;

library work;

package uLBC_pkg is
  -- uLBC parameter
  constant N : integer := 128; -- block length
  constant K : integer := 256; -- key length
  constant R : integer := 22; -- round number
  type RC_ARR is array (0 to R - 1) of std_logic_vector(7 downto 0); -- round constant
  type RK_ARR is array (0 to R) of std_logic_vector(K - 1 downto 0); -- round key
  type RS2_ARR is array (0 to R - 2) of std_logic_vector(N - 1 downto 0); -- round state
  type RS1_ARR is array (0 to R - 1) of std_logic_vector(N - 1 downto 0); -- round state
  --pi = x"a0ac9329ac4bc991c2313219c193ca814420cb8b49cc9ba882c104ba4a22";
  constant pi : RC_ARR := 
    (x"a0",
    x"ac",
    x"93",
    x"29",
    x"ac",
    x"4b",
    x"c9",
    x"91",
    x"c2",
    x"31",
    x"32",
    x"19",
    x"c1",
    x"93",
    x"ca",
    x"81",
    x"44",
    x"20",
    x"cb",
    x"8b",
    x"49",
    x"cc");
  
    --pi = x"a0ac9329ac4bc991c2313219c193ca814420cb8b49cc9ba882c104ba4a22";
    constant lfsr : RC_ARR := 
    (x"80",
    x"c0",
    x"e0",
    x"f0",
    x"f8",
    x"7c",
    x"bc",
    x"dc",
    x"ec",
    x"f4",
    x"78",
    x"3c",
    x"9c",
    x"cc",
    x"e4",
    x"70",
    x"b8",
    x"5c",
    x"ac",
    x"d4",
    x"68",
    x"34");
  

end uLBC_pkg;