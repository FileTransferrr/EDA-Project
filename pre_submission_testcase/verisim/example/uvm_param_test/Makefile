

UVM_HOME=../../simulator/uvm-1.2
all:
	verisim -j 8 -linebuf -all-class-spec -sv_lib ${UVM_HOME}/src/dpi/libuvm_dpi.so +incdir+${UVM_HOME}/src ${UVM_HOME}/src/uvm_pkg.sv test.sv

compile:
	verisim -genimage image -j 8 -linebuf -all-class-spec +incdir+${UVM_HOME}/src ${UVM_HOME}/src/uvm_pkg.sv test.sv

run:
	verisim -image image -sv_lib ${UVM_HOME}/src/dpi/libuvm_dpi.so 


clean:
	rm -rf verisim*
