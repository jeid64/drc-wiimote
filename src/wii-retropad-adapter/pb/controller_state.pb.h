/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.3 at Tue Jun  2 18:41:14 2015. */

#ifndef PB_CONTROLLER_STATE_PB_H_INCLUDED
#define PB_CONTROLLER_STATE_PB_H_INCLUDED
#include "pb.h"

#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
/* Struct definitions */
typedef struct _ControllerState {
    int32_t controller_id;
    int32_t bdl;
    int32_t bdr;
    int32_t bdu;
    int32_t bdd;
    int32_t ba;
    int32_t bb;
    int32_t bx;
    int32_t by;
    int32_t bl;
    int32_t br;
    int32_t bm;
    int32_t bp;
    int32_t bhome;
    int32_t bzl;
    int32_t bzr;
    int32_t lt;
    int32_t rt;
    int32_t lx;
    int32_t ly;
    int32_t rx;
    int32_t ry;
} ControllerState;

/* Default values for struct fields */

/* Initializer values for message structs */
#define ControllerState_init_default             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define ControllerState_init_zero                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define ControllerState_controller_id_tag        1
#define ControllerState_bdl_tag                  2
#define ControllerState_bdr_tag                  3
#define ControllerState_bdu_tag                  4
#define ControllerState_bdd_tag                  5
#define ControllerState_ba_tag                   6
#define ControllerState_bb_tag                   7
#define ControllerState_bx_tag                   8
#define ControllerState_by_tag                   9
#define ControllerState_bl_tag                   10
#define ControllerState_br_tag                   11
#define ControllerState_bm_tag                   12
#define ControllerState_bp_tag                   13
#define ControllerState_bhome_tag                14
#define ControllerState_bzl_tag                  15
#define ControllerState_bzr_tag                  16
#define ControllerState_lt_tag                   17
#define ControllerState_rt_tag                   18
#define ControllerState_lx_tag                   19
#define ControllerState_ly_tag                   20
#define ControllerState_rx_tag                   21
#define ControllerState_ry_tag                   22

/* Struct field encoding specification for nanopb */
extern const pb_field_t ControllerState_fields[23];

/* Maximum encoded size of messages (where known) */
#define ControllerState_size                     249

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define CONTROLLER_STATE_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
