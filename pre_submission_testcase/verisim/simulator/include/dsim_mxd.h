/**************************************************************************************
 * NOTICE OF COPYRIGHT AND OWNERSHIP OF SOFTWARE:
 *
 * Copyright (c) 2022 Primarius Technologies Co., Ltd.  All Rights Reserved.
 *
 * This computer program is the property of Primarius Technologies Co., Ltd. 9/F,
 * Building 4, No.26 Qiuyue Road, Zhangjiang, Pudong District, Shanghai, China
 * 
 * Any use, copy, publication, distribution, display, modification, or transmission
 * of this computer program in whole or in part in any form or by any means without
 * the prior express written permission of Primarius Technologies Co., Ltd. is
 * strictly prohibited.
 *
 * Except when expressly provided by Primarius Technologies Co., Ltd. in writing,
 * possession of this computer program shall not be construed to confer any license
 * or rights under any of Primarius Technologies Co., Ltd.'s intellectual property
 * rights, whether by estoppel, implication, or otherwise.
 *
 * ALL COPIES OF THIS PROGRAM MUST DISPLAY THIS NOTICE OF COPYRIGHT AND OWNERSHIP IN FULL.
 ******************************************************************************************/

#ifndef DSIM_MXD
#define DSIM_MXD
/***************************************************************************
* dsim_mxd.h
*
* MXD extensions.
*
* This file contains the API to allow customized control of the MXD files
* 
*
**************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "mxd.h"

void dsim_mxd_register_chunk_size_callback(mxd_file_default_chunk_size_func func);
void dsim_mxd_register_sub_chunk_size_callback(mxd_file_default_sub_chunk_size_func func);
void dsim_mxd_register_buffer_size_callback(mxd_vstbuffer_size_func func);
void dsim_mxd_register_compression_callback(mxd_chunk_compression_func func);

#ifdef __cplusplus
}
#endif

#endif // DSIM_MXD
