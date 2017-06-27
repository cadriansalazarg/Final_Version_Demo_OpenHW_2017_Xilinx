
#Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.
#Copyright <YEAR> <COPYRIGHT HOLDER>
#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

set design_name system

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "ERROR: Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      puts "INFO: Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   puts "INFO: Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   puts "INFO: Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   puts "INFO: Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}


puts "INFO: Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   puts $errMsg
   return $nRet
}


# Create interface ports
set DDR [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 DDR ]
set FIXED_IO [ create_bd_intf_port -mode Master -vlnv xilinx.com:display_processing_system7:fixedio_rtl:1.0 FIXED_IO ]

# Create ports

# Create instance: HLS_accel_1, and set properties
set HLS_accel_1 [ create_bd_cell -type ip -vlnv xilinx.com:hls:HLS_accel:1.0 HLS_accel_1 ]

# Create instance: axi_dma_1, and set properties
set axi_dma_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.1 axi_dma_1 ]
set_property -dict [ list CONFIG.c_include_mm2s_dre {1} CONFIG.c_include_s2mm_dre {1} CONFIG.c_include_sg {0} CONFIG.c_mm2s_burst_size {256} CONFIG.c_s2mm_burst_size {256} CONFIG.c_sg_use_stsapp_length {0}  ] $axi_dma_1

# Create instance: axi_interconnect_0, and set properties
set axi_interconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_interconnect_0 ]
set_property -dict [ list CONFIG.NUM_MI {1} CONFIG.NUM_SI {2}  ] $axi_interconnect_0

# Create instance: axi_interconnect_1, and set properties
set axi_interconnect_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_interconnect_1 ]
set_property -dict [ list CONFIG.NUM_MI {3}  ] $axi_interconnect_1

# Create instance: axi_timer_1, and set properties
set axi_timer_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:2.0 axi_timer_1 ]

# Create instance: proc_sys_reset, and set properties
set proc_sys_reset [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 proc_sys_reset ]

# Create instance: processing_system7_1, and set properties 
set processing_system7_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_1 ]
set_property -dict [ list  CONFIG.PCW_CAN0_PERIPHERAL_ENABLE {0} CONFIG.PCW_ENET0_PERIPHERAL_ENABLE {1} CONFIG.PCW_ENET0_ENET0_IO {MIO 16 .. 27} CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {100} CONFIG.PCW_GPIO_MIO_GPIO_ENABLE {0} CONFIG.PCW_I2C0_PERIPHERAL_ENABLE {0} CONFIG.PCW_IMPORT_BOARD_PRESET {None} CONFIG.PCW_IRQ_F2P_INTR {1} CONFIG.PCW_QSPI_GRP_SINGLE_SS_ENABLE {1} CONFIG.PCW_QSPI_PERIPHERAL_ENABLE {1} CONFIG.PCW_SD0_PERIPHERAL_ENABLE {0} CONFIG.PCW_USB0_PERIPHERAL_ENABLE {0} CONFIG.PCW_USE_FABRIC_INTERRUPT {1} CONFIG.PCW_USE_S_AXI_ACP {1} CONFIG.preset {ZC702}  ] $processing_system7_1

# Create instance: xlconcat_1, and set properties
set xlconcat_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_1 ]

# Create interface connections
connect_bd_intf_net -intf_net HLS_accel_1_output_stream [get_bd_intf_pins HLS_accel_1/OUTPUT_STREAM] [get_bd_intf_pins axi_dma_1/S_AXIS_S2MM]
connect_bd_intf_net -intf_net axi_dma_1_m_axi_mm2s [get_bd_intf_pins axi_dma_1/M_AXI_MM2S] [get_bd_intf_pins axi_interconnect_0/S00_AXI]
connect_bd_intf_net -intf_net axi_dma_1_m_axi_s2mm [get_bd_intf_pins axi_dma_1/M_AXI_S2MM] [get_bd_intf_pins axi_interconnect_0/S01_AXI]
connect_bd_intf_net -intf_net axi_dma_1_m_axis_mm2s [get_bd_intf_pins HLS_accel_1/INPUT_STREAM] [get_bd_intf_pins axi_dma_1/M_AXIS_MM2S]
connect_bd_intf_net -intf_net axi_interconnect_0_M00_AXI [get_bd_intf_pins axi_interconnect_0/M00_AXI] [get_bd_intf_pins processing_system7_1/S_AXI_ACP]
connect_bd_intf_net -intf_net axi_interconnect_1_m01_axi [get_bd_intf_pins HLS_accel_1/s_axi_CONTROL_BUS] [get_bd_intf_pins axi_interconnect_1/M01_AXI]
connect_bd_intf_net -intf_net axi_interconnect_1_m02_axi [get_bd_intf_pins axi_dma_1/S_AXI_LITE] [get_bd_intf_pins axi_interconnect_1/M02_AXI]
connect_bd_intf_net -intf_net processing_system7_1_axi_periph_m00_axi [get_bd_intf_pins axi_interconnect_1/M00_AXI] [get_bd_intf_pins axi_timer_1/S_AXI]
connect_bd_intf_net -intf_net processing_system7_1_ddr [get_bd_intf_ports DDR] [get_bd_intf_pins processing_system7_1/DDR]
connect_bd_intf_net -intf_net processing_system7_1_fixed_io [get_bd_intf_ports FIXED_IO] [get_bd_intf_pins processing_system7_1/FIXED_IO]
connect_bd_intf_net -intf_net processing_system7_1_m_axi_gp0 [get_bd_intf_pins axi_interconnect_1/S00_AXI] [get_bd_intf_pins processing_system7_1/M_AXI_GP0]

# Create port connections
connect_bd_net -net HLS_accel_1_interrupt [get_bd_pins HLS_accel_1/interrupt] [get_bd_pins xlconcat_1/In1]
connect_bd_net -net axi_timer_1_interrupt [get_bd_pins axi_timer_1/interrupt] [get_bd_pins xlconcat_1/In0]
connect_bd_net -net proc_sys_reset_interconnect_aresetn [get_bd_pins axi_interconnect_0/ARESETN] [get_bd_pins axi_interconnect_1/ARESETN] [get_bd_pins proc_sys_reset/interconnect_aresetn]
connect_bd_net -net proc_sys_reset_peripheral_aresetn [get_bd_pins HLS_accel_1/ap_rst_n] [get_bd_pins axi_dma_1/axi_resetn] [get_bd_pins axi_interconnect_0/M00_ARESETN] [get_bd_pins axi_interconnect_0/S00_ARESETN] [get_bd_pins axi_interconnect_0/S01_ARESETN] [get_bd_pins axi_interconnect_1/M00_ARESETN] [get_bd_pins axi_interconnect_1/M01_ARESETN] [get_bd_pins axi_interconnect_1/M02_ARESETN] [get_bd_pins axi_interconnect_1/S00_ARESETN] [get_bd_pins axi_timer_1/s_axi_aresetn] [get_bd_pins proc_sys_reset/peripheral_aresetn]
connect_bd_net -net processing_system7_1_fclk_clk0 [get_bd_pins HLS_accel_1/ap_clk] [get_bd_pins axi_dma_1/m_axi_mm2s_aclk] [get_bd_pins axi_dma_1/m_axi_s2mm_aclk] [get_bd_pins axi_dma_1/s_axi_lite_aclk] [get_bd_pins axi_interconnect_0/ACLK] [get_bd_pins axi_interconnect_0/M00_ACLK] [get_bd_pins axi_interconnect_0/S00_ACLK] [get_bd_pins axi_interconnect_0/S01_ACLK] [get_bd_pins axi_interconnect_1/ACLK] [get_bd_pins axi_interconnect_1/M00_ACLK] [get_bd_pins axi_interconnect_1/M01_ACLK] [get_bd_pins axi_interconnect_1/M02_ACLK] [get_bd_pins axi_interconnect_1/S00_ACLK] [get_bd_pins axi_timer_1/s_axi_aclk] [get_bd_pins proc_sys_reset/slowest_sync_clk] [get_bd_pins processing_system7_1/FCLK_CLK0] [get_bd_pins processing_system7_1/M_AXI_GP0_ACLK] [get_bd_pins processing_system7_1/S_AXI_ACP_ACLK]
connect_bd_net -net processing_system7_1_fclk_reset0_n [get_bd_pins proc_sys_reset/ext_reset_in] [get_bd_pins processing_system7_1/FCLK_RESET0_N]
connect_bd_net -net xlconcat_1_dout [get_bd_pins processing_system7_1/IRQ_F2P] [get_bd_pins xlconcat_1/dout]

# Create address segments
create_bd_addr_seg -range 0x400000 -offset 0xE0000000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs processing_system7_1/S_AXI_ACP/ACP_IOP] SEG1
create_bd_addr_seg -range 0x40000000 -offset 0x0 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs processing_system7_1/S_AXI_ACP/ACP_DDR_LOWOCM] SEG2
create_bd_addr_seg -range 0x40000000 -offset 0x0 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs processing_system7_1/S_AXI_ACP/ACP_DDR_LOWOCM] SEG_processing_system7_1_ACP_DDR_LOWOCM
create_bd_addr_seg -range 0x400000 -offset 0xE0000000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs processing_system7_1/S_AXI_ACP/ACP_IOP] SEG_processing_system7_1_ACP_IOP
create_bd_addr_seg -range 0x40000000 -offset 0x40000000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs processing_system7_1/S_AXI_ACP/ACP_M_AXI_GP0] SEG_processing_system7_1_ACP_M_AXI_GP0
create_bd_addr_seg -range 0x40000000 -offset 0x40000000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs processing_system7_1/S_AXI_ACP/ACP_M_AXI_GP0] SEG_processing_system7_1_ACP_M_AXI_GP0
create_bd_addr_seg -range 0x10000 -offset 0x43C00000 [get_bd_addr_spaces processing_system7_1/Data] [get_bd_addr_segs HLS_accel_1/s_axi_CONTROL_BUS/Reg] SEG_HLS_accel_1_Reg
create_bd_addr_seg -range 0x10000 -offset 0x40400000 [get_bd_addr_spaces processing_system7_1/Data] [get_bd_addr_segs axi_dma_1/S_AXI_LITE/Reg] SEG_axi_dma_1_Reg
create_bd_addr_seg -range 0x10000 -offset 0x42800000 [get_bd_addr_spaces processing_system7_1/Data] [get_bd_addr_segs axi_timer_1/S_AXI/Reg] SEG_axi_timer_1_Reg

#New commands
#Create port connection AXI -LITE
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_1/M_AXI_GP0" Clk "Auto" }  [get_bd_intf_pins HLS_accel_1/s_axi_bus]

#Validate the design
validate_bd_design

save_bd_design

