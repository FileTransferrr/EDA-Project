/*
  Test intent: A parameterized UVM test can be registered with the UVM factory
               by the macro `uvm_component_utils using the default specialization 
               of the class, as long as the parameters are not type parameters

  Test scope : UVM 1.1b or 1.2

  Keywords   : UVM, parameter, class, uvm_test

  References : N/A
*/

import uvm_pkg::*;
`include "uvm_macros.svh"

class basic_test #(DATA_WIDTH=16) extends uvm_test;
   
  `uvm_component_utils(basic_test)
   
  function new(string name = "basic_test", uvm_component parent=null);
     super.new(name, parent);
  endfunction // new

  virtual task run_phase(uvm_phase phase);
     phase.raise_objection(this);
     $display("RUNNING basic_test...");
     super.run_phase(phase);
     $display("DATA_WIDTH = %0d", DATA_WIDTH);
     $display("basic_test COMPLETED.");
     phase.drop_objection(this);
  endtask

endclass // basic_test

module testbench;

   initial begin
      run_test("basic_test");
   end
endmodule // testbench

        
   