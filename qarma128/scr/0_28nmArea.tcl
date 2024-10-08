sh rm -rf WORK/*
remove_design -all
define_design_lib WORK -path ./WORK

set PDKDIR [format "%s%s"  [getenv "TSMC28"] "/TSMCHOME/digital/Front_End/timing_power_noise/CCS/tcbn28hpcplusbwp12t30p140_180a"]
set search_path [concat  $search_path $PDKDIR]
set alib_library_analysis_path $PDKDIR
set libname "tcbn28hpcplusbwp12t30p140ffg1p05vm40c_ccs.db"
set link_library [set target_library [concat  [list $libname] [list dw_foundation.sldb]]]
set target_library $libname



analyze -library WORK -format sverilog -autoread {../verilog}
elaborate cipher_top -library WORK

#set_max_delay -from Plaintext -to Ciphertext  0
#set_max_delay -from Key -to Ciphertext  0

create_clock -name "clk" -period 100 -waveform { 0 50  }  { clk  }

set_max_area 0

compile_ultra

change_selection -name global -replace [get_timing_paths -delay_type max -nworst 1 -max_paths 1 -include_hierarchical_pins]

uplevel #0 { report_timing -path full -delay max -nworst 40 -max_paths 40 -significant_digits 2 -sort_by group > 0_28nm_minarea_timing_lp.txt}

uplevel #0 { report_area -hierarchy > 0_28nm_minarea_area_lp.txt}
 
write -hierarchy -format verilog -output dc-syn.v 

write_sdf dc-syn.sdf  

write -hierarchy -format ddc -output dc.ddc

write -hierarchy -format vhdl -output dc.vhdl

write_sdc -nosplit dc.sdc

exit