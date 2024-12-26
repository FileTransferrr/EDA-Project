#!/usr/bin/env perl
# **************************************************************************************
# * NOTICE OF COPYRIGHT AND OWNERSHIP OF SOFTWARE:
# *
# * Copyright (c) 2022 Primarius Technologies Co., Ltd.  All Rights Reserved.
# *
# * This computer program is the property of Primarius Technologies Co., Ltd. 9/F,
# * Building 4, No.26 Qiuyue Road, Zhangjiang, Pudong District, Shanghai, China
# * 
# * Any use, copy, publication, distribution, display, modification, or transmission
# * of this computer program in whole or in part in any form or by any means without
# * the prior express written permission of Primarius Technologies Co., Ltd. is
# * strictly prohibited.
# *
# * Except when expressly provided by Primarius Technologies Co., Ltd. in writing,
# * possession of this computer program shall not be construed to confer any license
# * or rights under any of Primarius Technologies Co., Ltd.'s intellectual property
# * rights, whether by estoppel, implication, or otherwise.
# *
# * ALL COPIES OF THIS PROGRAM MUST DISPLAY THIS NOTICE OF COPYRIGHT AND OWNERSHIP IN FULL.
# *****************************************************************************************/
#

open(OUTF,">dsim_vpi_pkg.sv");
print OUTF <<HEAD;
package dsim_vpi_pkg;
typedef chandle vpiHandle;
HEAD

open(INF,"vpi_user.h") || die("open vpi_user.h");
&extract();
close(INF);
open(INF,"sv_vpi_user.h") || die("open sv_vpi_user.h");
&extract();
close(INF);

print OUTF <<TAIL;
import "DPI-C" function vpiHandle vpi_handle_by_name(string name, vpiHandle scope);
import "DPI-C" function vpiHandle vpi_handle_by_index(string name, vpiHandle indx);
import "DPI-C" function vpiHandle vpi_handle(int arg, vpiHandle handle);
import "DPI-C" function vpiHandle vpi_iterate(int arg, vpiHandle handle);
import "DPI-C" function vpiHandle vpi_scan(vpiHandle handle);
import "DPI-C" function int vpi_get(int prop, vpiHandle obj);
import "DPI-C" function string vpi_get_str(int prop, vpiHandle obj);
import "DPI-C" function int vpi_release_handle(vpiHandle obj);

//import "DPI-C" function void vpi_get_value_4s(vpiHandle h, output reg [] value);
//import "DPI-C" function void vpi_put_value_4s(vpiHandle h, reg [] value, int flags);

endpackage
TAIL

close(OUTF);

sub extract {
    while (<INF>) {
        if (/#define +([a-zA-Z0-9]+) +([0-9]+)/) {
            print OUTF "localparam $1 = $2;\n";
        }
    }
}
