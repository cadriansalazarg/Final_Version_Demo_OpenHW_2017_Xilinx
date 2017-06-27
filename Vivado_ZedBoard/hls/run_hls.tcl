
# WRAPPED AXI4-STREAM MATRIX MULTIPLIER HLS PROJECT
open_project hls_wrapped_mmult_prj
set_top HLS_accel


#open_project Borrar
add_files infoli.cpp  
add_files dma_interface.cpp  
add_files infoli_wrapped.cpp  
add_files -tb infoli_tb.cpp  -cflags  "-lm" 
 
open_solution "solution0"
set_part {xc7z020clg484-1} -tool vivado
create_clock -period 10 -name default
csim_design -clean
csim_design 
csynth_design
#the next line must be uncommented to do the cosimulation
#cosim_design  
export_design -format ip_catalog
close_project
