library work;
use work.uLBC_pkg.all;

library ieee;
use ieee.std_logic_1164.all;

entity PosPerm is
    port (
        State_DI: in std_logic_vector(N - 1 downto 0);
        State_DO: out std_logic_vector(N - 1 downto 0)
    );
end entity;

architecture Behavioral of PosPerm is
begin
    State_DO(127 downto 124) <= State_DI(127 downto 124);
    State_DO(123 downto 120) <= State_DI(27 downto 24);
    State_DO(119 downto 116) <= State_DI(55 downto 52);
    State_DO(115 downto 112) <= State_DI(67 downto 64);
    State_DO(111 downto 108) <= State_DI(111 downto 108);
    State_DO(107 downto 104) <= State_DI(123 downto 120);
    State_DO(103 downto 100) <= State_DI(7 downto 4);
    State_DO(99 downto 96) <= State_DI(51 downto 48);
    State_DO(95 downto 92) <= State_DI(95 downto 92);
    State_DO(91 downto 88) <= State_DI(11 downto 8);
    State_DO(87 downto 84) <= State_DI(39 downto 36);
    State_DO(83 downto 80) <= State_DI(99 downto 96);
    State_DO(79 downto 76) <= State_DI(79 downto 76);
    State_DO(75 downto 72) <= State_DI(91 downto 88);
    State_DO(71 downto 68) <= State_DI(23 downto 20);
    State_DO(67 downto 64) <= State_DI(35 downto 32);
    State_DO(63 downto 60) <= State_DI(47 downto 44);
    State_DO(59 downto 56) <= State_DI(107 downto 104);
    State_DO(55 downto 52) <= State_DI(87 downto 84);
    State_DO(51 downto 48) <= State_DI(3 downto 0);
    State_DO(47 downto 44) <= State_DI(63 downto 60);
    State_DO(43 downto 40) <= State_DI(75 downto 72);
    State_DO(39 downto 36) <= State_DI(119 downto 116);
    State_DO(35 downto 32) <= State_DI(19 downto 16);
    State_DO(31 downto 28) <= State_DI(15 downto 12);
    State_DO(27 downto 24) <= State_DI(59 downto 56);
    State_DO(23 downto 20) <= State_DI(103 downto 100);
    State_DO(19 downto 16) <= State_DI(115 downto 112);
    State_DO(15 downto 12) <= State_DI(31 downto 28);
    State_DO(11 downto 8) <= State_DI(43 downto 40);
    State_DO(7 downto 4) <= State_DI(71 downto 68);
    State_DO(3 downto 0) <= State_DI(83 downto 80);
end Behavioral;