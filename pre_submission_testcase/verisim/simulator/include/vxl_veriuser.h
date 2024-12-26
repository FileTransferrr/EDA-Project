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
 *****************************************************************************************/

#ifndef VXL_VERIUSER_H
#define VXL_VERIUSER_H

/*------------- values for component 'type' in tfcell structure -------------*/
#define usertask                  1
#define USERTASK                  usertask
#define userfunction              2
#define USERFUNCTION              userfunction
#define userrealfunction          3
#define USERREALFUNCTION          userrealfunction


#if defined(__STDC__) || defined(__cplusplus)

#ifndef PROTO_PARAMS
#define PROTO_PARAMS(params) params
#define DEFINED_PROTO_PARAMS
#endif

#else

#ifndef PROTO_PARAMS
#define PROTO_PARAMS(params) (/* nothing */)
#define DEFINED_PROTO_PARAMS
#endif

#endif /* __STDC__ */


typedef struct t_tfcell
{
    short type;
    short data;
    int (*checktf) PROTO_PARAMS((int data, int reason));
    int (*sizetf) PROTO_PARAMS((int data, int reason));
    int (*calltf) PROTO_PARAMS((int data, int reason));
    int (*misctf) PROTO_PARAMS((int data, int reason, int paramvc));
    char *tfname;
    int forwref;
    char *tferrmessage;

    int hash;
    struct tfcell_t *left_p;
    struct tfcell_t *right_p;
    char *namecell_p;
    int warning_printed;
    char *user_data;
} s_tfcell, *p_tfcell;


#ifdef DEFINED_PROTO_PARAMS
#undef DEFINED_PROTO_PARAMS
#undef PROTO_PARAMS
#endif


#endif /* VXL_VERIUSER_H */
