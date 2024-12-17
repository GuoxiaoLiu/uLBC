library work;
use work.uLBC_pkg.all;

library ieee;
use ieee.std_logic_1164.all;

entity MixColAddKey is
    port (
        State_DI: in std_logic_vector(N - 1 downto 0);
        Key_DI: in std_logic_vector(N - 1 downto 0);
        State_DO: out std_logic_vector(N - 1 downto 0)
    );
end entity;

architecture Behavioral of MixColAddKey is
    signal tmp, tmp0, tmp1 : std_logic_vector(N - 1 downto 0);
begin
    -- State_DO(127 downto 124) <= State_DI(123 downto 120) xor State_DI(119 downto 116) xor State_DI(115 downto 112);
    -- State_DO(123 downto 120) <= State_DI(127 downto 124) xor State_DI(119 downto 116) xor State_DI(115 downto 112);
    -- State_DO(119 downto 116) <= State_DI(127 downto 124) xor State_DI(123 downto 120) xor State_DI(115 downto 112);
    -- State_DO(115 downto 112) <= State_DI(127 downto 124) xor State_DI(123 downto 120) xor State_DI(119 downto 116);
    -- State_DO(111 downto 108) <= State_DI(107 downto 104) xor State_DI(103 downto 100) xor State_DI(99 downto 96);
    -- State_DO(107 downto 104) <= State_DI(111 downto 108) xor State_DI(103 downto 100) xor State_DI(99 downto 96);
    -- State_DO(103 downto 100) <= State_DI(111 downto 108) xor State_DI(107 downto 104) xor State_DI(99 downto 96);
    -- State_DO(99 downto 96) <= State_DI(111 downto 108) xor State_DI(107 downto 104) xor State_DI(103 downto 100);
    -- State_DO(95 downto 92) <= State_DI(91 downto 88) xor State_DI(87 downto 84) xor State_DI(83 downto 80);
    -- State_DO(91 downto 88) <= State_DI(95 downto 92) xor State_DI(87 downto 84) xor State_DI(83 downto 80);
    -- State_DO(87 downto 84) <= State_DI(95 downto 92) xor State_DI(91 downto 88) xor State_DI(83 downto 80);
    -- State_DO(83 downto 80) <= State_DI(95 downto 92) xor State_DI(91 downto 88) xor State_DI(87 downto 84);
    -- State_DO(79 downto 76) <= State_DI(75 downto 72) xor State_DI(71 downto 68) xor State_DI(67 downto 64);
    -- State_DO(75 downto 72) <= State_DI(79 downto 76) xor State_DI(71 downto 68) xor State_DI(67 downto 64);
    -- State_DO(71 downto 68) <= State_DI(79 downto 76) xor State_DI(75 downto 72) xor State_DI(67 downto 64);
    -- State_DO(67 downto 64) <= State_DI(79 downto 76) xor State_DI(75 downto 72) xor State_DI(71 downto 68);
    -- State_DO(63 downto 60) <= State_DI(59 downto 56) xor State_DI(55 downto 52) xor State_DI(51 downto 48);
    -- State_DO(59 downto 56) <= State_DI(63 downto 60) xor State_DI(55 downto 52) xor State_DI(51 downto 48);
    -- State_DO(55 downto 52) <= State_DI(63 downto 60) xor State_DI(59 downto 56) xor State_DI(51 downto 48);
    -- State_DO(51 downto 48) <= State_DI(63 downto 60) xor State_DI(59 downto 56) xor State_DI(55 downto 52);
    -- State_DO(47 downto 44) <= State_DI(43 downto 40) xor State_DI(39 downto 36) xor State_DI(35 downto 32);
    -- State_DO(43 downto 40) <= State_DI(47 downto 44) xor State_DI(39 downto 36) xor State_DI(35 downto 32);
    -- State_DO(39 downto 36) <= State_DI(47 downto 44) xor State_DI(43 downto 40) xor State_DI(35 downto 32);
    -- State_DO(35 downto 32) <= State_DI(47 downto 44) xor State_DI(43 downto 40) xor State_DI(39 downto 36);
    -- State_DO(31 downto 28) <= State_DI(27 downto 24) xor State_DI(23 downto 20) xor State_DI(19 downto 16);
    -- State_DO(27 downto 24) <= State_DI(31 downto 28) xor State_DI(23 downto 20) xor State_DI(19 downto 16);
    -- State_DO(23 downto 20) <= State_DI(31 downto 28) xor State_DI(27 downto 24) xor State_DI(19 downto 16);
    -- State_DO(19 downto 16) <= State_DI(31 downto 28) xor State_DI(27 downto 24) xor State_DI(23 downto 20);
    -- State_DO(15 downto 12) <= State_DI(11 downto 8) xor State_DI(7 downto 4) xor State_DI(3 downto 0);
    -- State_DO(11 downto 8) <= State_DI(15 downto 12) xor State_DI(7 downto 4) xor State_DI(3 downto 0);
    -- State_DO(7 downto 4) <= State_DI(15 downto 12) xor State_DI(11 downto 8) xor State_DI(3 downto 0);
    -- State_DO(3 downto 0) <= State_DI(15 downto 12) xor State_DI(11 downto 8) xor State_DI(7 downto 4);
    tmp <= State_DI xor Key_DI;
    
    tmp0(127 downto 124) <= tmp(123 downto 120) xor tmp(119 downto 116);
    tmp1(127 downto 124) <= tmp(115 downto 112);
    
    tmp0(123 downto 120) <= tmp(127 downto 124) xor tmp(119 downto 116);
    tmp1(123 downto 120) <= tmp(115 downto 112);
    
    tmp0(119 downto 116) <= tmp(127 downto 124) xor tmp(123 downto 120);
    tmp1(119 downto 116) <= tmp(115 downto 112);

    tmp0(115 downto 112) <= tmp(127 downto 124) xor tmp(123 downto 120);
    tmp1(115 downto 112) <= tmp(119 downto 116);
    
    tmp0(111 downto 108) <= tmp(107 downto 104) xor tmp(103 downto 100);
    tmp1(111 downto 108) <= tmp(99 downto 96);
    
    tmp0(107 downto 104) <= tmp(111 downto 108) xor tmp(103 downto 100);
    tmp1(107 downto 104) <= tmp(99 downto 96);
    
    tmp0(103 downto 100) <= tmp(111 downto 108) xor tmp(107 downto 104);
    tmp1(103 downto 100) <= tmp(99 downto 96);
    
    tmp0(99 downto 96) <= tmp(111 downto 108) xor tmp(107 downto 104);
    tmp1(99 downto 96) <= tmp(103 downto 100);
    
    tmp0(95 downto 92) <= tmp(91 downto 88) xor tmp(87 downto 84);
    tmp1(95 downto 92) <= tmp(83 downto 80);
    
    tmp0(91 downto 88) <= tmp(95 downto 92) xor tmp(87 downto 84);
    tmp1(91 downto 88) <= tmp(83 downto 80);
    
    tmp0(87 downto 84) <= tmp(95 downto 92) xor tmp(91 downto 88);
    tmp1(87 downto 84) <= tmp(83 downto 80);
    
    tmp0(83 downto 80) <= tmp(95 downto 92) xor tmp(91 downto 88);
    tmp1(83 downto 80) <= tmp(87 downto 84);
    
    tmp0(79 downto 76) <= tmp(75 downto 72) xor tmp(71 downto 68);
    tmp1(79 downto 76) <= tmp(67 downto 64);
    
    tmp0(75 downto 72) <= tmp(79 downto 76) xor tmp(71 downto 68);
    tmp1(75 downto 72) <= tmp(67 downto 64);
    
    tmp0(71 downto 68) <= tmp(79 downto 76) xor tmp(75 downto 72);
    tmp1(71 downto 68) <= tmp(67 downto 64);
    
    tmp0(67 downto 64) <= tmp(79 downto 76) xor tmp(75 downto 72);
    tmp1(67 downto 64) <= tmp(71 downto 68);
    
    tmp0(63 downto 60) <= tmp(59 downto 56) xor tmp(55 downto 52);
    tmp1(63 downto 60) <= tmp(51 downto 48);
    
    tmp0(59 downto 56) <= tmp(63 downto 60) xor tmp(55 downto 52);
    tmp1(59 downto 56) <= tmp(51 downto 48);
    
    tmp0(55 downto 52) <= tmp(63 downto 60) xor tmp(59 downto 56);
    tmp1(55 downto 52) <= tmp(51 downto 48);
    
    tmp0(51 downto 48) <= tmp(63 downto 60) xor tmp(59 downto 56);
    tmp1(51 downto 48) <= tmp(55 downto 52);
    
    tmp0(47 downto 44) <= tmp(43 downto 40) xor tmp(39 downto 36);
    tmp1(47 downto 44) <= tmp(35 downto 32);
    
    tmp0(43 downto 40) <= tmp(47 downto 44) xor tmp(39 downto 36);
    tmp1(43 downto 40) <= tmp(35 downto 32);
    
    tmp0(39 downto 36) <= tmp(47 downto 44) xor tmp(43 downto 40);
    tmp1(39 downto 36) <= tmp(35 downto 32);
    
    tmp0(35 downto 32) <= tmp(47 downto 44) xor tmp(43 downto 40);
    tmp1(35 downto 32) <= tmp(39 downto 36);
    
    tmp0(31 downto 28) <= tmp(27 downto 24) xor tmp(23 downto 20);
    tmp1(31 downto 28) <= tmp(19 downto 16);
    
    tmp0(27 downto 24) <= tmp(31 downto 28) xor tmp(23 downto 20);
    tmp1(27 downto 24) <= tmp(19 downto 16);
    
    tmp0(23 downto 20) <= tmp(31 downto 28) xor tmp(27 downto 24);
    tmp1(23 downto 20) <= tmp(19 downto 16);
    
    tmp0(19 downto 16) <= tmp(31 downto 28) xor tmp(27 downto 24);
    tmp1(19 downto 16) <= tmp(23 downto 20);
    
    tmp0(15 downto 12) <= tmp(11 downto 8) xor tmp(7 downto 4);
    tmp1(15 downto 12) <= tmp(3 downto 0);
    
    tmp0(11 downto 8) <= tmp(15 downto 12) xor tmp(7 downto 4);
    tmp1(11 downto 8) <= tmp(3 downto 0);
    
    tmp0(7 downto 4) <= tmp(15 downto 12) xor tmp(11 downto 8);
    tmp1(7 downto 4) <= tmp(3 downto 0);
    
    tmp0(3 downto 0) <= tmp(15 downto 12) xor tmp(11 downto 8);
    tmp1(3 downto 0) <= tmp(7 downto 4);
    
    State_DO <= tmp0 xor tmp1;

end architecture Behavioral;