
# Tensix ISS Wormhole B0 ISA coverage

Legend: feature support status marks

```
[+]    Supported and at least all known LLK use cases are covered
       (fully supported if mark % is absent)
[%]    Partially supported because of insufficient documentation or missing use cases
       (will be documented separately; for instructions details are commented in the code)
[0]    Recognized but does not affect simulator state
[C]    CFG instruction: always reads or writes configuration regster values but
       affects simulator state only for supported configuration registers 
       (see the separate list below)
[T]    Support delegated to Thread layer
```

Note that the ISS does not aim at detailed modeling of Tensix RTL therefore some
features that are not required for correct instruction execution are silently ignored
(e.g., most of operand variants in STALLWAIT).

## Instructions

```
    ADDDMAREG        [+]
    ADDRCRXY         [+]
    ADDRCRZW         [+]
    APOOL3S1
    APOOL3S2
    ATCAS
    ATGETM           [+]
    ATINCGET
    ATINCGETPTR
    ATRELM           [+]
    ATSWAP
    BITWOPDMAREG     [+]
    CLEARDVALID      [+]
    CLREXPHIST
    CMPDMAREG        [+]
    CONV3S1
    CONV3S2
    DMANOP           [+]
    DOTPV
    ELWADD           [+]
    ELWMUL           [+]
    ELWSUB           [+]
    FLUSHDMA         [+0]
    GAPOOL           [+%]
    GATESRCRST       [+0]
    GMPOOL           [+%]
    INCADCXY         [+]
    INCADCZW         [+]
    INCRWC           [+]
    LOADIND
    LOADREG
    MFCONV3S1
    MOP              [+T]
    MOP_CFG          [+T]
    MOVA2D           [+%]
    MOVB2A           [+]
    MOVB2D           [+%]
    MOVD2A           [+%]
    MOVD2B           [+%]
    MOVDBGA2D
    MPOOL3S1
    MPOOL3S2
    MULDMAREG        [+]
    MVMUL            [+]
    NOP              [+]
    PACR             [+%]
    PACR_SETREG
    RAREB
    RDCFG            [+C]
    REG2FLOP         [+]
    REPLAY           [+T]
    RMWCIB0          [+C]
    RMWCIB1          [+C]
    RMWCIB2          [+C]
    RMWCIB3          [+C]
    RSTDMA           [+0]
    SEMGET           [+]
    SEMINIT          [+]
    SEMPOST          [+]
    SEMWAIT          [+T]
    SETADC           [+]
    SETADCXX         [+]
    SETADCXY         [+]
    SETADCZW         [+]
    SETASHRMH
    SETASHRMH0
    SETASHRMH1
    SETASHRMV
    SETC16           [+C]
    SETDMAREG        [+]
    SETDVALID        [+]
    SETIBRWC
    SETPKEDGOF
    SETRWC           [+]
    SFPABS           [+]
    SFPADD           [+%]
    SFPADDI          [+%]
    SFPAND           [+]
    SFPCAST          [+%]
    SFPCOMPC         [+]
    SFPCONFIG        [+%]
    SFPDIVP2         [+]
    SFPENCC          [+]
    SFPEXEXP         [+]
    SFPEXMAN         [+]
    SFPIADD          [+]
    SFPLOAD          [+%]
    SFPLOADI         [+]
    SFPLOADMACRO
    SFPLUT           [+%]
    SFPLUTFP32       [+]
    SFPLZ            [+]
    SFPMAD           [+]
    SFPMOV           [+%]
    SFPMUL           [+%]
    SFPMULI          [+%]
    SFPNOP           [+]
    SFPNOT           [+]
    SFPOR            [+]
    SFPPOPC          [+]
    SFPPUSHC         [+]
    SFPSETCC         [+]
    SFPSETEXP        [+]
    SFPSETMAN        [+]
    SFPSETSGN        [+]
    SFPSHFT          [+]
    SFPSHFT2         [+]
    SFPSTORE         [+%]
    SFPSWAP          [+]
    SFPTRANSP        [+]
    SFPXOR           [+]
    SFP_STOCH_RND    [+%]
    SHIFTDMAREG      [+]
    SHIFTXA
    SHIFTXB
    STALLWAIT        [+T]
    STOREIND         [+]
    STOREREG
    SUBDMAREG        [+]
    TBUFCMD
    TRNSPSRCA
    TRNSPSRCB        [+]
    UNPACR           [+%]
    UNPACR_NOP       [+%]
    WRCFG            [+C]
    XMOV
    ZEROACC          [+]
    ZEROSRC          [+]
```

## Registers

```
// Registers for THREAD

//    THREAD_CFGREG_BASE_ADDR32                     0

    CFG_STATE_ID_StateID_ADDR32                   0
    DEST_TARGET_REG_CFG_MATH_Offset_ADDR32        1        [+]
    ADDR_MOD_SET_Base_ADDR32                      2        [+]
    SRCA_SET_Base_ADDR32                          3
    SRCA_SET_SetOvrdWithAddr_ADDR32               3
    SRCB_SET_Base_ADDR32                          4
    CLR_DVALID_SrcA_Disable_ADDR32                5
    CLR_DVALID_SrcB_Disable_ADDR32                5
    FIDELITY_BASE_Phase_ADDR32                    6
    ADDR_MOD_AB_SEC0_SrcAIncr_ADDR32              7        [+]
    ADDR_MOD_AB_SEC0_SrcACR_ADDR32                7        [+]
    ADDR_MOD_AB_SEC0_SrcAClear_ADDR32             7        [+]
    ADDR_MOD_AB_SEC0_SrcBIncr_ADDR32              7        [+]
    ADDR_MOD_AB_SEC0_SrcBCR_ADDR32                7        [+]
    ADDR_MOD_AB_SEC0_SrcBClear_ADDR32             7        [+]
    ADDR_MOD_AB_SEC0_BiasCntIncr_ADDR32           8
    ADDR_MOD_AB_SEC0_BiasCntClear_ADDR32          8
    ADDR_MOD_AB_SEC1_SrcAIncr_ADDR32              9        [+]
    ADDR_MOD_AB_SEC1_SrcACR_ADDR32                9        [+]
    ADDR_MOD_AB_SEC1_SrcAClear_ADDR32             9        [+]
    ADDR_MOD_AB_SEC1_SrcBIncr_ADDR32              9        [+]
    ADDR_MOD_AB_SEC1_SrcBCR_ADDR32                9        [+]
    ADDR_MOD_AB_SEC1_SrcBClear_ADDR32             9        [+]
    ADDR_MOD_AB_SEC1_BiasCntIncr_ADDR32          10
    ADDR_MOD_AB_SEC1_BiasCntClear_ADDR32         10
    ADDR_MOD_AB_SEC2_SrcAIncr_ADDR32             11        [+]
    ADDR_MOD_AB_SEC2_SrcACR_ADDR32               11        [+]
    ADDR_MOD_AB_SEC2_SrcAClear_ADDR32            11        [+]
    ADDR_MOD_AB_SEC2_SrcBIncr_ADDR32             11        [+]
    ADDR_MOD_AB_SEC2_SrcBCR_ADDR32               11        [+]
    ADDR_MOD_AB_SEC2_SrcBClear_ADDR32            11        [+]
    ADDR_MOD_AB_SEC2_BiasCntIncr_ADDR32          12
    ADDR_MOD_AB_SEC2_BiasCntClear_ADDR32         12
    ADDR_MOD_AB_SEC3_SrcAIncr_ADDR32             13        [+]
    ADDR_MOD_AB_SEC3_SrcACR_ADDR32               13        [+]
    ADDR_MOD_AB_SEC3_SrcAClear_ADDR32            13        [+]
    ADDR_MOD_AB_SEC3_SrcBIncr_ADDR32             13        [+]
    ADDR_MOD_AB_SEC3_SrcBCR_ADDR32               13        [+]
    ADDR_MOD_AB_SEC3_SrcBClear_ADDR32            13        [+]
    ADDR_MOD_AB_SEC3_BiasCntIncr_ADDR32          14
    ADDR_MOD_AB_SEC3_BiasCntClear_ADDR32         14
    ADDR_MOD_AB_SEC4_SrcAIncr_ADDR32             15        [+]
    ADDR_MOD_AB_SEC4_SrcACR_ADDR32               15        [+]
    ADDR_MOD_AB_SEC4_SrcAClear_ADDR32            15        [+]
    ADDR_MOD_AB_SEC4_SrcBIncr_ADDR32             15        [+]
    ADDR_MOD_AB_SEC4_SrcBCR_ADDR32               15        [+]
    ADDR_MOD_AB_SEC4_SrcBClear_ADDR32            15        [+]
    ADDR_MOD_AB_SEC4_BiasCntIncr_ADDR32          16
    ADDR_MOD_AB_SEC4_BiasCntClear_ADDR32         16
    ADDR_MOD_AB_SEC5_SrcAIncr_ADDR32             17        [+]
    ADDR_MOD_AB_SEC5_SrcACR_ADDR32               17        [+]
    ADDR_MOD_AB_SEC5_SrcAClear_ADDR32            17        [+]
    ADDR_MOD_AB_SEC5_SrcBIncr_ADDR32             17        [+]
    ADDR_MOD_AB_SEC5_SrcBCR_ADDR32               17        [+]
    ADDR_MOD_AB_SEC5_SrcBClear_ADDR32            17        [+]
    ADDR_MOD_AB_SEC5_BiasCntIncr_ADDR32          18
    ADDR_MOD_AB_SEC5_BiasCntClear_ADDR32         18
    ADDR_MOD_AB_SEC6_SrcAIncr_ADDR32             19        [+]
    ADDR_MOD_AB_SEC6_SrcACR_ADDR32               19        [+]
    ADDR_MOD_AB_SEC6_SrcAClear_ADDR32            19        [+]
    ADDR_MOD_AB_SEC6_SrcBIncr_ADDR32             19        [+]
    ADDR_MOD_AB_SEC6_SrcBCR_ADDR32               19        [+]
    ADDR_MOD_AB_SEC6_SrcBClear_ADDR32            19        [+]
    ADDR_MOD_AB_SEC6_BiasCntIncr_ADDR32          20
    ADDR_MOD_AB_SEC6_BiasCntClear_ADDR32         20
    ADDR_MOD_AB_SEC7_SrcAIncr_ADDR32             21        [+]
    ADDR_MOD_AB_SEC7_SrcACR_ADDR32               21        [+]
    ADDR_MOD_AB_SEC7_SrcAClear_ADDR32            21        [+]
    ADDR_MOD_AB_SEC7_SrcBIncr_ADDR32             21        [+]
    ADDR_MOD_AB_SEC7_SrcBCR_ADDR32               21        [+]
    ADDR_MOD_AB_SEC7_SrcBClear_ADDR32            21        [+]
    ADDR_MOD_AB_SEC7_BiasCntIncr_ADDR32          22
    ADDR_MOD_AB_SEC7_BiasCntClear_ADDR32         22
    ADDR_MOD_DST_SEC0_DestIncr_ADDR32            23        [+]
    ADDR_MOD_DST_SEC0_DestCR_ADDR32              23        [+]
    ADDR_MOD_DST_SEC0_DestClear_ADDR32           23        [+]
    ADDR_MOD_DST_SEC0_DestCToCR_ADDR32           23        [+]
    ADDR_MOD_DST_SEC0_FidelityIncr_ADDR32        23        [+]
    ADDR_MOD_DST_SEC0_FidelityClear_ADDR32       23        [+]
    ADDR_MOD_DST_SEC1_DestIncr_ADDR32            24        [+]
    ADDR_MOD_DST_SEC1_DestCR_ADDR32              24        [+]
    ADDR_MOD_DST_SEC1_DestClear_ADDR32           24        [+]
    ADDR_MOD_DST_SEC1_DestCToCR_ADDR32           24        [+]
    ADDR_MOD_DST_SEC1_FidelityIncr_ADDR32        24        [+]
    ADDR_MOD_DST_SEC1_FidelityClear_ADDR32       24        [+]
    ADDR_MOD_DST_SEC2_DestIncr_ADDR32            25        [+]
    ADDR_MOD_DST_SEC2_DestCR_ADDR32              25        [+]
    ADDR_MOD_DST_SEC2_DestClear_ADDR32           25        [+]
    ADDR_MOD_DST_SEC2_DestCToCR_ADDR32           25        [+]
    ADDR_MOD_DST_SEC2_FidelityIncr_ADDR32        25        [+]
    ADDR_MOD_DST_SEC2_FidelityClear_ADDR32       25        [+]
    ADDR_MOD_DST_SEC3_DestIncr_ADDR32            26        [+]
    ADDR_MOD_DST_SEC3_DestCR_ADDR32              26        [+]
    ADDR_MOD_DST_SEC3_DestClear_ADDR32           26        [+]
    ADDR_MOD_DST_SEC3_DestCToCR_ADDR32           26        [+]
    ADDR_MOD_DST_SEC3_FidelityIncr_ADDR32        26        [+]
    ADDR_MOD_DST_SEC3_FidelityClear_ADDR32       26        [+]
    ADDR_MOD_DST_SEC4_DestIncr_ADDR32            27        [+]
    ADDR_MOD_DST_SEC4_DestCR_ADDR32              27        [+]
    ADDR_MOD_DST_SEC4_DestClear_ADDR32           27        [+]
    ADDR_MOD_DST_SEC4_DestCToCR_ADDR32           27        [+]
    ADDR_MOD_DST_SEC4_FidelityIncr_ADDR32        27        [+]
    ADDR_MOD_DST_SEC4_FidelityClear_ADDR32       27        [+]
    ADDR_MOD_DST_SEC5_DestIncr_ADDR32            28        [+]
    ADDR_MOD_DST_SEC5_DestCR_ADDR32              28        [+]
    ADDR_MOD_DST_SEC5_DestClear_ADDR32           28        [+]
    ADDR_MOD_DST_SEC5_DestCToCR_ADDR32           28        [+]
    ADDR_MOD_DST_SEC5_FidelityIncr_ADDR32        28        [+]
    ADDR_MOD_DST_SEC5_FidelityClear_ADDR32       28        [+]
    ADDR_MOD_DST_SEC6_DestIncr_ADDR32            29        [+]
    ADDR_MOD_DST_SEC6_DestCR_ADDR32              29        [+]
    ADDR_MOD_DST_SEC6_DestClear_ADDR32           29        [+]
    ADDR_MOD_DST_SEC6_DestCToCR_ADDR32           29        [+]
    ADDR_MOD_DST_SEC6_FidelityIncr_ADDR32        29        [+]
    ADDR_MOD_DST_SEC6_FidelityClear_ADDR32       29        [+]
    ADDR_MOD_DST_SEC7_DestIncr_ADDR32            30        [+]
    ADDR_MOD_DST_SEC7_DestCR_ADDR32              30        [+]
    ADDR_MOD_DST_SEC7_DestClear_ADDR32           30        [+]
    ADDR_MOD_DST_SEC7_DestCToCR_ADDR32           30        [+]
    ADDR_MOD_DST_SEC7_FidelityIncr_ADDR32        30        [+]
    ADDR_MOD_DST_SEC7_FidelityClear_ADDR32       30        [+]
    ADDR_MOD_PACK_SEC0_YsrcIncr_ADDR32           31        [+]
    ADDR_MOD_PACK_SEC0_YsrcCR_ADDR32             31        [+]
    ADDR_MOD_PACK_SEC0_YsrcClear_ADDR32          31        [+]
    ADDR_MOD_PACK_SEC0_YdstIncr_ADDR32           31        [+]
    ADDR_MOD_PACK_SEC0_YdstCR_ADDR32             31        [+]
    ADDR_MOD_PACK_SEC0_YdstClear_ADDR32          31        [+]
    ADDR_MOD_PACK_SEC0_ZsrcIncr_ADDR32           31        [+]
    ADDR_MOD_PACK_SEC0_ZsrcClear_ADDR32          31        [+]
    ADDR_MOD_PACK_SEC0_ZdstIncr_ADDR32           31        [+]
    ADDR_MOD_PACK_SEC0_ZdstClear_ADDR32          31        [+]
    ADDR_MOD_PACK_SEC1_YsrcIncr_ADDR32           32        [+]
    ADDR_MOD_PACK_SEC1_YsrcCR_ADDR32             32        [+]
    ADDR_MOD_PACK_SEC1_YsrcClear_ADDR32          32        [+]
    ADDR_MOD_PACK_SEC1_YdstIncr_ADDR32           32        [+]
    ADDR_MOD_PACK_SEC1_YdstCR_ADDR32             32        [+]
    ADDR_MOD_PACK_SEC1_YdstClear_ADDR32          32        [+]
    ADDR_MOD_PACK_SEC1_ZsrcIncr_ADDR32           32        [+]
    ADDR_MOD_PACK_SEC1_ZsrcClear_ADDR32          32        [+]
    ADDR_MOD_PACK_SEC1_ZdstIncr_ADDR32           32        [+]
    ADDR_MOD_PACK_SEC1_ZdstClear_ADDR32          32        [+]
    ADDR_MOD_PACK_SEC2_YsrcIncr_ADDR32           33        [+]
    ADDR_MOD_PACK_SEC2_YsrcCR_ADDR32             33        [+]
    ADDR_MOD_PACK_SEC2_YsrcClear_ADDR32          33        [+]
    ADDR_MOD_PACK_SEC2_YdstIncr_ADDR32           33        [+]
    ADDR_MOD_PACK_SEC2_YdstCR_ADDR32             33        [+]
    ADDR_MOD_PACK_SEC2_YdstClear_ADDR32          33        [+]
    ADDR_MOD_PACK_SEC2_ZsrcIncr_ADDR32           33        [+]
    ADDR_MOD_PACK_SEC2_ZsrcClear_ADDR32          33        [+]
    ADDR_MOD_PACK_SEC2_ZdstIncr_ADDR32           33        [+]
    ADDR_MOD_PACK_SEC2_ZdstClear_ADDR32          33        [+]
    ADDR_MOD_PACK_SEC3_YsrcIncr_ADDR32           34        [+]
    ADDR_MOD_PACK_SEC3_YsrcCR_ADDR32             34        [+]
    ADDR_MOD_PACK_SEC3_YsrcClear_ADDR32          34        [+]
    ADDR_MOD_PACK_SEC3_YdstIncr_ADDR32           34        [+]
    ADDR_MOD_PACK_SEC3_YdstCR_ADDR32             34        [+]
    ADDR_MOD_PACK_SEC3_YdstClear_ADDR32          34        [+]
    ADDR_MOD_PACK_SEC3_ZsrcIncr_ADDR32           34        [+]
    ADDR_MOD_PACK_SEC3_ZsrcClear_ADDR32          34        [+]
    ADDR_MOD_PACK_SEC3_ZdstIncr_ADDR32           34        [+]
    ADDR_MOD_PACK_SEC3_ZdstClear_ADDR32          34        [+]
    ADDR_MOD_PACK_SEC4_YsrcIncr_ADDR32           35        [+]
    ADDR_MOD_PACK_SEC4_YsrcCR_ADDR32             35        [+]
    ADDR_MOD_PACK_SEC4_YsrcClear_ADDR32          35        [+]
    ADDR_MOD_PACK_SEC4_YdstIncr_ADDR32           35        [+]
    ADDR_MOD_PACK_SEC4_YdstCR_ADDR32             35        [+]
    ADDR_MOD_PACK_SEC4_YdstClear_ADDR32          35        [+]
    ADDR_MOD_PACK_SEC4_ZsrcIncr_ADDR32           35        [+]
    ADDR_MOD_PACK_SEC4_ZsrcClear_ADDR32          35        [+]
    ADDR_MOD_PACK_SEC4_ZdstIncr_ADDR32           35        [+]
    ADDR_MOD_PACK_SEC4_ZdstClear_ADDR32          35        [+]
    ADDR_MOD_PACK_SEC5_YsrcIncr_ADDR32           36        [+]
    ADDR_MOD_PACK_SEC5_YsrcCR_ADDR32             36        [+]
    ADDR_MOD_PACK_SEC5_YsrcClear_ADDR32          36        [+]
    ADDR_MOD_PACK_SEC5_YdstIncr_ADDR32           36        [+]
    ADDR_MOD_PACK_SEC5_YdstCR_ADDR32             36        [+]
    ADDR_MOD_PACK_SEC5_YdstClear_ADDR32          36        [+]
    ADDR_MOD_PACK_SEC5_ZsrcIncr_ADDR32           36        [+]
    ADDR_MOD_PACK_SEC5_ZsrcClear_ADDR32          36        [+]
    ADDR_MOD_PACK_SEC5_ZdstIncr_ADDR32           36        [+]
    ADDR_MOD_PACK_SEC5_ZdstClear_ADDR32          36        [+]
    ADDR_MOD_PACK_SEC6_YsrcIncr_ADDR32           37        [+]
    ADDR_MOD_PACK_SEC6_YsrcCR_ADDR32             37        [+]
    ADDR_MOD_PACK_SEC6_YsrcClear_ADDR32          37        [+]
    ADDR_MOD_PACK_SEC6_YdstIncr_ADDR32           37        [+]
    ADDR_MOD_PACK_SEC6_YdstCR_ADDR32             37        [+]
    ADDR_MOD_PACK_SEC6_YdstClear_ADDR32          37        [+]
    ADDR_MOD_PACK_SEC6_ZsrcIncr_ADDR32           37        [+]
    ADDR_MOD_PACK_SEC6_ZsrcClear_ADDR32          37        [+]
    ADDR_MOD_PACK_SEC6_ZdstIncr_ADDR32           37        [+]
    ADDR_MOD_PACK_SEC6_ZdstClear_ADDR32          37        [+]
    ADDR_MOD_PACK_SEC7_YsrcIncr_ADDR32           38        [+]
    ADDR_MOD_PACK_SEC7_YsrcCR_ADDR32             38        [+]
    ADDR_MOD_PACK_SEC7_YsrcClear_ADDR32          38        [+]
    ADDR_MOD_PACK_SEC7_YdstIncr_ADDR32           38        [+]
    ADDR_MOD_PACK_SEC7_YdstCR_ADDR32             38        [+]
    ADDR_MOD_PACK_SEC7_YdstClear_ADDR32          38        [+]
    ADDR_MOD_PACK_SEC7_ZsrcIncr_ADDR32           38        [+]
    ADDR_MOD_PACK_SEC7_ZsrcClear_ADDR32          38        [+]
    ADDR_MOD_PACK_SEC7_ZdstIncr_ADDR32           38        [+]
    ADDR_MOD_PACK_SEC7_ZdstClear_ADDR32          38        [+]
    UNPACK_MISC_CFG_CfgContextOffset_0_ADDR32    39        [+]
    UNPACK_MISC_CFG_CfgContextCntReset_0_ADDR32  39
    UNPACK_MISC_CFG_CfgContextCntInc_0_ADDR32    39
    UNPACK_MISC_CFG_CfgContextOffset_1_ADDR32    39        [+]
    UNPACK_MISC_CFG_CfgContextCntReset_1_ADDR32  39
    UNPACK_MISC_CFG_CfgContextCntInc_1_ADDR32    39
    NOC_OVERLAY_MSG_CLEAR_StreamId_0_ADDR32      40
    NOC_OVERLAY_MSG_CLEAR_MsgNum_0_ADDR32        40
    NOC_OVERLAY_MSG_CLEAR_StreamId_1_ADDR32      41
    NOC_OVERLAY_MSG_CLEAR_MsgNum_1_ADDR32        41
    CG_CTRL_EN_Regblocks_ADDR32                  42
    CG_CTRL_EN_Fpu_ADDR32                        42
    CG_CTRL_EN_Fpu_mtile_ADDR32                  42
    CG_CTRL_EN_Fpu_sfpu_ADDR32                   42
    CG_CTRL_EN_Move_ADDR32                       42
    CG_CTRL_EN_Packer_ADDR32                     42
    CG_CTRL_EN_Unpacker_ADDR32                   42
    CG_CTRL_EN_Xsearch_ADDR32                    42
    CG_CTRL_EN_Thcon_ADDR32                      42
    CG_CTRL_EN_Trisc_ADDR32                      42
    CG_CTRL_EN_Retmux_ADDR32                     42
    CG_CTRL_EN_Ithread_ADDR32                    42
    CG_CTRL_EN_Hyst_ADDR32                       43
    CG_CTRL_KICK_Regblocks_ADDR32                44
    CG_CTRL_KICK_Fpu_ADDR32                      44
    CG_CTRL_KICK_Move_ADDR32                     44
    CG_CTRL_KICK_Packer_ADDR32                   44
    CG_CTRL_KICK_Unpacker_ADDR32                 44
    CG_CTRL_KICK_Xsearch_ADDR32                  44
    CG_CTRL_KICK_Thcon_ADDR32                    44
    CG_CTRL_KICK_Trisc_ADDR32                    44
    CG_CTRL_KICK_Retmux_ADDR32                   44
    CG_CTRL_KICK_Ithread_ADDR32                  44
    PERF_CNT_CMD_Cmd0Start_ADDR32                45
    PERF_CNT_CMD_Cmd0Stop_ADDR32                 45
    PERF_CNT_CMD_Cmd1Start_ADDR32                45
    PERF_CNT_CMD_Cmd1Stop_ADDR32                 45
    PERF_CNT_CMD_Cmd2Start_ADDR32                45
    PERF_CNT_CMD_Cmd2Stop_ADDR32                 45
    PERF_CNT_CMD_Cmd3Start_ADDR32                45
    PERF_CNT_CMD_Cmd3Stop_ADDR32                 45
    ENABLE_ACC_STATS_Enable_ADDR32               46
    FPU_BIAS_SEL_Pointer_ADDR32                  47
    ADDR_MOD_BIAS_SEC0_BiasIncr_ADDR32           48        [+]
    ADDR_MOD_BIAS_SEC0_BiasClear_ADDR32          48        [+]
    ADDR_MOD_BIAS_SEC1_BiasIncr_ADDR32           49        [+]
    ADDR_MOD_BIAS_SEC1_BiasClear_ADDR32          49        [+]
    ADDR_MOD_BIAS_SEC2_BiasIncr_ADDR32           50        [+]
    ADDR_MOD_BIAS_SEC2_BiasClear_ADDR32          50        [+]
    ADDR_MOD_BIAS_SEC3_BiasIncr_ADDR32           51        [+]
    ADDR_MOD_BIAS_SEC3_BiasClear_ADDR32          51        [+]
    ADDR_MOD_BIAS_SEC4_BiasIncr_ADDR32           52        [+]
    ADDR_MOD_BIAS_SEC4_BiasClear_ADDR32          52        [+]
    ADDR_MOD_BIAS_SEC5_BiasIncr_ADDR32           53        [+]
    ADDR_MOD_BIAS_SEC5_BiasClear_ADDR32          53        [+]
    ADDR_MOD_BIAS_SEC6_BiasIncr_ADDR32           54        [+]
    ADDR_MOD_BIAS_SEC6_BiasClear_ADDR32          54        [+]
    ADDR_MOD_BIAS_SEC7_BiasIncr_ADDR32           55        [+]
    ADDR_MOD_BIAS_SEC7_BiasClear_ADDR32          55        [+]
    FP16A_FORCE_Enable_ADDR32                    56

// Registers for ALU

//    ALU_CFGREG_BASE_ADDR32                            0

    ALU_FORMAT_SPEC_REG_SrcA_val_ADDR32               0
    ALU_FORMAT_SPEC_REG_SrcA_override_ADDR32          0
    ALU_FORMAT_SPEC_REG_SrcB_val_ADDR32               0
    ALU_FORMAT_SPEC_REG_SrcB_override_ADDR32          0
    ALU_FORMAT_SPEC_REG_Dstacc_val_ADDR32             0
    ALU_FORMAT_SPEC_REG_Dstacc_override_ADDR32        0
    ALU_ROUNDING_MODE_Fpu_srnd_en_ADDR32              1
    ALU_ROUNDING_MODE_Gasket_srnd_en_ADDR32           1
    ALU_ROUNDING_MODE_Packer_srnd_en_ADDR32           1
    ALU_ROUNDING_MODE_Padding_ADDR32                  1
    ALU_ROUNDING_MODE_GS_LF_ADDR32                    1
    ALU_ROUNDING_MODE_Bfp8_HF_ADDR32                  1
    ALU_FORMAT_SPEC_REG0_SrcAUnsigned_ADDR32          1
    ALU_FORMAT_SPEC_REG0_SrcBUnsigned_ADDR32          1
    ALU_FORMAT_SPEC_REG0_SrcA_ADDR32                  1
    ALU_FORMAT_SPEC_REG1_SrcB_ADDR32                  1
    ALU_FORMAT_SPEC_REG2_Dstacc_ADDR32                1
    ALU_ACC_CTRL_Fp32_enabled_ADDR32                  1
    ALU_ACC_CTRL_SFPU_Fp32_enabled_ADDR32             1
    ALU_ACC_CTRL_INT8_math_enabled_ADDR32             1
    ALU_ACC_CTRL_Zero_Flag_disabled_src_ADDR32        2
    ALU_ACC_CTRL_Zero_Flag_disabled_dst_ADDR32        2
    STACC_RELU_ApplyRelu_ADDR32                       2        [+]
    STACC_RELU_ReluThreshold_ADDR32                   2        [+]
    DISABLE_RISC_BP_Disable_main_ADDR32               2
    DISABLE_RISC_BP_Disable_trisc_ADDR32              2
    DISABLE_RISC_BP_Disable_ncrisc_ADDR32             2
    DISABLE_RISC_BP_Disable_bmp_clear_main_ADDR32     2
    DISABLE_RISC_BP_Disable_bmp_clear_trisc_ADDR32    2
    DISABLE_RISC_BP_Disable_bmp_clear_ncrisc_ADDR32   2
    ECC_SCRUBBER_Enable_ADDR32                        3
    ECC_SCRUBBER_Scrub_On_Error_ADDR32                3
    ECC_SCRUBBER_Scrub_On_Error_Immediately_ADDR32    3
    ECC_SCRUBBER_Delay_ADDR32                         3
    STATE_RESET_EN_ADDR32                             4
    DEST_OFFSET_Enable_ADDR32                         5
    DEST_REGW_BASE_Base_ADDR32                        6
    INT_DESCALE_Enable_ADDR32                         7
    INT_DESCALE_Mode_ADDR32                           7

// Registers for PACK0

//    PACK0_CFGREG_BASE_ADDR32                             8

    PCK0_ADDR_CTRL_XY_REG_0_Xstride_ADDR32               8        [+]
    PCK0_ADDR_CTRL_XY_REG_0_Ystride_ADDR32               8        [+]
    PCK0_ADDR_CTRL_ZW_REG_0_Zstride_ADDR32               9        [+]
    PCK0_ADDR_CTRL_ZW_REG_0_Wstride_ADDR32               9        [+]
    PCK0_ADDR_CTRL_XY_REG_1_Xstride_ADDR32              10
    PCK0_ADDR_CTRL_XY_REG_1_Ystride_ADDR32              10
    PCK0_ADDR_CTRL_ZW_REG_1_Zstride_ADDR32              11
    PCK0_ADDR_CTRL_ZW_REG_1_Wstride_ADDR32              11
    PCK0_ADDR_BASE_REG_0_Base_ADDR32                    12
    PCK0_ADDR_BASE_REG_1_Base_ADDR32                    13
    PCK_DEST_RD_CTRL_Read_32b_data_ADDR32               14        [+]
    PCK_DEST_RD_CTRL_Read_unsigned_ADDR32               14
    PCK_DEST_RD_CTRL_Read_int8_ADDR32                   14
    PCK_DEST_RD_CTRL_Round_10b_mant_ADDR32              14
    PCK_EDGE_TILE_FACE_SET_SELECT_select_ADDR32         15
    PCK_EDGE_TILE_FACE_SET_SELECT_enable_ADDR32         15
    TILE_ROW_SET_MAPPING_0_row_set_mapping_0_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_1_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_2_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_3_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_4_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_5_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_6_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_7_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_8_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_9_ADDR32     16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_10_ADDR32    16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_11_ADDR32    16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_12_ADDR32    16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_13_ADDR32    16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_14_ADDR32    16        [+]
    TILE_ROW_SET_MAPPING_0_row_set_mapping_15_ADDR32    16        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_1_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_2_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_3_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_4_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_5_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_6_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_7_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_8_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_9_ADDR32     17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_10_ADDR32    17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_11_ADDR32    17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_12_ADDR32    17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_13_ADDR32    17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_14_ADDR32    17        [+]
    TILE_ROW_SET_MAPPING_1_row_set_mapping_15_ADDR32    17        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_0_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_1_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_2_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_3_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_4_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_5_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_6_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_7_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_8_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_9_ADDR32     18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_10_ADDR32    18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_11_ADDR32    18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_12_ADDR32    18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_13_ADDR32    18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_14_ADDR32    18        [+]
    TILE_ROW_SET_MAPPING_2_row_set_mapping_15_ADDR32    18        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_0_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_1_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_2_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_3_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_4_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_5_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_6_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_7_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_8_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_9_ADDR32     19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_10_ADDR32    19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_11_ADDR32    19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_12_ADDR32    19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_13_ADDR32    19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_14_ADDR32    19        [+]
    TILE_ROW_SET_MAPPING_3_row_set_mapping_15_ADDR32    19        [+]
    PCK_EDGE_OFFSET_SEC0_mask_ADDR32                    20        [+]
    PCK_EDGE_MODE_mode_ADDR32                           20
    PCK_EDGE_TILE_ROW_SET_SELECT_select_ADDR32          20        [+]
    PCK_EDGE_OFFSET_SEC1_mask_ADDR32                    21        [+]
    PCK_EDGE_OFFSET_SEC2_mask_ADDR32                    22        [+]
    PCK_EDGE_OFFSET_SEC3_mask_ADDR32                    23        [+]
    PACK_COUNTERS_SEC0_pack_per_xy_plane_ADDR32         24
    PACK_COUNTERS_SEC0_pack_reads_per_xy_plane_ADDR32   24
    PACK_COUNTERS_SEC0_pack_xys_per_tile_ADDR32         24
    PACK_COUNTERS_SEC0_pack_yz_transposed_ADDR32        24
    PACK_COUNTERS_SEC0_pack_per_xy_plane_offset_ADDR32  24
    PACK_COUNTERS_SEC1_pack_per_xy_plane_ADDR32         25
    PACK_COUNTERS_SEC1_pack_reads_per_xy_plane_ADDR32   25
    PACK_COUNTERS_SEC1_pack_xys_per_tile_ADDR32         25
    PACK_COUNTERS_SEC1_pack_yz_transposed_ADDR32        25
    PACK_COUNTERS_SEC1_pack_per_xy_plane_offset_ADDR32  25
    PACK_COUNTERS_SEC2_pack_per_xy_plane_ADDR32         26
    PACK_COUNTERS_SEC2_pack_reads_per_xy_plane_ADDR32   26
    PACK_COUNTERS_SEC2_pack_xys_per_tile_ADDR32         26
    PACK_COUNTERS_SEC2_pack_yz_transposed_ADDR32        26
    PACK_COUNTERS_SEC2_pack_per_xy_plane_offset_ADDR32  26
    PACK_COUNTERS_SEC3_pack_per_xy_plane_ADDR32         27
    PACK_COUNTERS_SEC3_pack_reads_per_xy_plane_ADDR32   27
    PACK_COUNTERS_SEC3_pack_xys_per_tile_ADDR32         27
    PACK_COUNTERS_SEC3_pack_yz_transposed_ADDR32        27
    PACK_COUNTERS_SEC3_pack_per_xy_plane_offset_ADDR32  27
    PACK_CONCAT_MASK_SEC0_pack_concat_mask_ADDR32       28
    PACK_CONCAT_MASK_SEC1_pack_concat_mask_ADDR32       29
    PACK_CONCAT_MASK_SEC2_pack_concat_mask_ADDR32       30
    PACK_CONCAT_MASK_SEC3_pack_concat_mask_ADDR32       31
    TILE_FACE_SET_MAPPING_0_face_set_mapping_0_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_1_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_2_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_3_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_4_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_5_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_6_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_7_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_8_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_9_ADDR32   32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_10_ADDR32  32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_11_ADDR32  32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_12_ADDR32  32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_13_ADDR32  32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_14_ADDR32  32
    TILE_FACE_SET_MAPPING_0_face_set_mapping_15_ADDR32  32
    TILE_FACE_SET_MAPPING_1_face_set_mapping_0_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_1_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_2_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_3_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_4_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_5_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_6_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_7_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_8_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_9_ADDR32   33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_10_ADDR32  33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_11_ADDR32  33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_12_ADDR32  33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_13_ADDR32  33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_14_ADDR32  33
    TILE_FACE_SET_MAPPING_1_face_set_mapping_15_ADDR32  33
    TILE_FACE_SET_MAPPING_2_face_set_mapping_0_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_1_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_2_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_3_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_4_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_5_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_6_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_7_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_8_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_9_ADDR32   34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_10_ADDR32  34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_11_ADDR32  34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_12_ADDR32  34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_13_ADDR32  34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_14_ADDR32  34
    TILE_FACE_SET_MAPPING_2_face_set_mapping_15_ADDR32  34
    TILE_FACE_SET_MAPPING_3_face_set_mapping_0_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_1_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_2_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_3_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_4_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_5_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_6_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_7_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_8_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_9_ADDR32   35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_10_ADDR32  35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_11_ADDR32  35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_12_ADDR32  35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_13_ADDR32  35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_14_ADDR32  35
    TILE_FACE_SET_MAPPING_3_face_set_mapping_15_ADDR32  35

// Registers for UNPACK0

//    UNPACK0_CFGREG_BASE_ADDR32                         36

    UNP0_ADDR_CTRL_XY_REG_0_Xstride_ADDR32             36
    UNP0_ADDR_CTRL_XY_REG_0_Ystride_ADDR32             36
    UNP0_ADDR_CTRL_ZW_REG_0_Zstride_ADDR32             37
    UNP0_ADDR_CTRL_ZW_REG_0_Wstride_ADDR32             37
    UNP1_ADDR_CTRL_XY_REG_0_Xstride_ADDR32             38
    UNP1_ADDR_CTRL_XY_REG_0_Ystride_ADDR32             38
    UNP1_ADDR_CTRL_ZW_REG_0_Zstride_ADDR32             39
    UNP1_ADDR_CTRL_ZW_REG_0_Wstride_ADDR32             39
    UNP0_ADDR_BASE_REG_0_Base_ADDR32                   40
    UNP0_ADDR_BASE_REG_1_Base_ADDR32                   40
    UNP0_FORCED_SHARED_EXP_shared_exp_ADDR32           41
    UNP0_ADD_DEST_ADDR_CNTR_add_dest_addr_cntr_ADDR32  41
    UNP0_BLOBS_Y_START_CNTX_01_blobs_y_start_ADDR32    42
    UNP0_BLOBS_Y_START_CNTX_23_blobs_y_start_ADDR32    43

// Registers for UNPACK1

//    UNPACK1_CFGREG_BASE_ADDR32                         44

    UNP0_ADDR_CTRL_XY_REG_1_Xstride_ADDR32             44
    UNP0_ADDR_CTRL_XY_REG_1_Ystride_ADDR32             44
    UNP0_ADDR_CTRL_ZW_REG_1_Zstride_ADDR32             45
    UNP0_ADDR_CTRL_ZW_REG_1_Wstride_ADDR32             45
    UNP1_ADDR_CTRL_XY_REG_1_Xstride_ADDR32             46
    UNP1_ADDR_CTRL_XY_REG_1_Ystride_ADDR32             46
    UNP1_ADDR_CTRL_ZW_REG_1_Zstride_ADDR32             47
    UNP1_ADDR_CTRL_ZW_REG_1_Wstride_ADDR32             47
    UNP1_ADDR_BASE_REG_0_Base_ADDR32                   48
    UNP1_ADDR_BASE_REG_1_Base_ADDR32                   48
    UNP1_FORCED_SHARED_EXP_shared_exp_ADDR32           49
    UNP1_ADD_DEST_ADDR_CNTR_add_dest_addr_cntr_ADDR32  49

// Registers for THCON

//    THCON_CFGREG_BASE_ADDR32                                    52

    THCON_SEC0_REG0_TileDescriptor_ADDR32                       52        [+%]
    THCON_SEC0_REG1_Row_start_section_size_ADDR32               56
    THCON_SEC0_REG1_Exp_section_size_ADDR32                     56
    THCON_SEC0_REG1_L1_Dest_addr_ADDR32                         57        [+]
    THCON_SEC0_REG1_Disable_zero_compress_ADDR32                58
    THCON_SEC0_REG1_Add_l1_dest_addr_offset_ADDR32              58
    THCON_SEC0_REG1_Addr_cnt_context_ADDR32                     58
    THCON_SEC0_REG1_Out_data_format_ADDR32                      58        [+]
    THCON_SEC0_REG1_In_data_format_ADDR32                       58        [+]
    THCON_SEC0_REG1_Dis_shared_exp_assembler_ADDR32             58
    THCON_SEC0_REG1_Force_pack_per_max_xy_plane_ADDR32          58
    THCON_SEC0_REG1_Enable_out_fifo_ADDR32                      58
    THCON_SEC0_REG1_Sub_l1_tile_header_size_ADDR32              58
    THCON_SEC0_REG1_Source_interface_selection_ADDR32           58
    THCON_SEC0_REG1_All_pack_disable_zero_compress_ADDR32       58
    THCON_SEC0_REG1_All_pack_disable_zero_compress_ovrd_ADDR32  58
    THCON_SEC0_REG1_Add_tile_header_size_ADDR32                 58
    THCON_SEC0_REG1_L1_source_addr_ADDR32                       58
    THCON_SEC0_REG1_Downsample_mask_ADDR32                      59
    THCON_SEC0_REG1_Downsample_rate_ADDR32                      59
    THCON_SEC0_REG1_Read_mode_ADDR32                            59
    THCON_SEC0_REG1_Exp_threshold_en_ADDR32                     59
    THCON_SEC0_REG1_Pack_L1_Acc_ADDR32                          59
    THCON_SEC0_REG1_Disable_pack_zero_flags_ADDR32              59
    THCON_SEC0_REG1_Exp_threshold_ADDR32                        59
    THCON_SEC0_REG2_Out_data_format_ADDR32                      60
    THCON_SEC0_REG2_Throttle_mode_ADDR32                        60
    THCON_SEC0_REG2_Context_count_ADDR32                        60
    THCON_SEC0_REG2_Haloize_mode_ADDR32                         60        [+]
    THCON_SEC0_REG2_Tileize_mode_ADDR32                         60        [+]
    THCON_SEC0_REG2_Unpack_Src_Reg_Set_Upd_ADDR32               60
    THCON_SEC0_REG2_Unpack_If_Sel_ADDR32                        60
    THCON_SEC0_REG2_Upsample_rate_ADDR32                        60
    THCON_SEC0_REG2_Ovrd_data_format_ADDR32                     60
    THCON_SEC0_REG2_Upsample_and_interleave_ADDR32              60
    THCON_SEC0_REG2_Shift_amount_cntx0_ADDR32                   60        [+]
    THCON_SEC0_REG2_Shift_amount_cntx1_ADDR32                   60
    THCON_SEC0_REG2_Shift_amount_cntx2_ADDR32                   60
    THCON_SEC0_REG2_Shift_amount_cntx3_ADDR32                   60
    THCON_SEC0_REG2_Disable_zero_compress_cntx0_ADDR32          61
    THCON_SEC0_REG2_Disable_zero_compress_cntx1_ADDR32          61
    THCON_SEC0_REG2_Disable_zero_compress_cntx2_ADDR32          61
    THCON_SEC0_REG2_Disable_zero_compress_cntx3_ADDR32          61
    THCON_SEC0_REG2_Unpack_if_sel_cntx0_ADDR32                  61
    THCON_SEC0_REG2_Unpack_if_sel_cntx1_ADDR32                  61
    THCON_SEC0_REG2_Unpack_if_sel_cntx2_ADDR32                  61
    THCON_SEC0_REG2_Unpack_if_sel_cntx3_ADDR32                  61
    THCON_SEC0_REG2_Force_shared_exp_ADDR32                     61
    THCON_SEC0_REG2_Disable_zero_compress_cntx4_ADDR32          61
    THCON_SEC0_REG2_Disable_zero_compress_cntx5_ADDR32          61
    THCON_SEC0_REG2_Disable_zero_compress_cntx6_ADDR32          61
    THCON_SEC0_REG2_Disable_zero_compress_cntx7_ADDR32          61
    THCON_SEC0_REG2_Unpack_if_sel_cntx4_ADDR32                  61
    THCON_SEC0_REG2_Unpack_if_sel_cntx5_ADDR32                  61
    THCON_SEC0_REG2_Unpack_if_sel_cntx6_ADDR32                  61
    THCON_SEC0_REG2_Unpack_if_sel_cntx7_ADDR32                  61
    THCON_SEC0_REG2_Unpack_limit_address_ADDR32                 62
    THCON_SEC0_REG2_Unpack_fifo_size_ADDR32                     63
    THCON_SEC0_REG3_Base_address_ADDR32                         64        [+]
    THCON_SEC0_REG3_Base_cntx1_address_ADDR32                   65        [+]
    THCON_SEC0_REG3_Base_cntx2_address_ADDR32                   66        [+]
    THCON_SEC0_REG3_Base_cntx3_address_ADDR32                   67        [+]
    THCON_SEC0_REG4_Base_cntx4_address_ADDR32                   68        [+]
    THCON_SEC0_REG4_Base_cntx5_address_ADDR32                   69        [+]
    THCON_SEC0_REG4_Base_cntx6_address_ADDR32                   70        [+]
    THCON_SEC0_REG4_Base_cntx7_address_ADDR32                   71        [+]
    THCON_SEC0_REG5_Dest_cntx0_address_ADDR32                   72
    THCON_SEC0_REG5_Dest_cntx1_address_ADDR32                   72
    THCON_SEC0_REG5_Dest_cntx2_address_ADDR32                   73
    THCON_SEC0_REG5_Dest_cntx3_address_ADDR32                   73
    THCON_SEC0_REG5_Tile_x_dim_cntx0_ADDR32                     74        [+]
    THCON_SEC0_REG5_Tile_x_dim_cntx1_ADDR32                     74        [+]
    THCON_SEC0_REG5_Tile_x_dim_cntx2_ADDR32                     75
    THCON_SEC0_REG5_Tile_x_dim_cntx3_ADDR32                     75
    THCON_SEC0_REG6_Source_address_ADDR32                       76
    THCON_SEC0_REG6_Destination_address_ADDR32                  77
    THCON_SEC0_REG6_Buffer_size_ADDR32                          78
    THCON_SEC0_REG6_Transfer_direction_ADDR32                   78
    THCON_SEC0_REG7_Offset_address_ADDR32                       80        [+]
    THCON_SEC0_REG7_Unpack_data_format_cntx0_ADDR32             80
    THCON_SEC0_REG7_Unpack_out_data_format_cntx0_ADDR32         80
    THCON_SEC0_REG7_Unpack_data_format_cntx4_ADDR32             80
    THCON_SEC0_REG7_Unpack_out_data_format_cntx4_ADDR32         80
    THCON_SEC0_REG7_Offset_cntx1_address_ADDR32                 81        [+]
    THCON_SEC0_REG7_Unpack_data_format_cntx1_ADDR32             81
    THCON_SEC0_REG7_Unpack_out_data_format_cntx1_ADDR32         81
    THCON_SEC0_REG7_Unpack_data_format_cntx5_ADDR32             81
    THCON_SEC0_REG7_Unpack_out_data_format_cntx5_ADDR32         81
    THCON_SEC0_REG7_Offset_cntx2_address_ADDR32                 82        [+]
    THCON_SEC0_REG7_Unpack_data_format_cntx2_ADDR32             82
    THCON_SEC0_REG7_Unpack_out_data_format_cntx2_ADDR32         82
    THCON_SEC0_REG7_Unpack_data_format_cntx6_ADDR32             82
    THCON_SEC0_REG7_Unpack_out_data_format_cntx6_ADDR32         82
    THCON_SEC0_REG7_Offset_cntx3_address_ADDR32                 83        [+]
    THCON_SEC0_REG7_Unpack_data_format_cntx3_ADDR32             83
    THCON_SEC0_REG7_Unpack_out_data_format_cntx3_ADDR32         83
    THCON_SEC0_REG7_Unpack_data_format_cntx7_ADDR32             83
    THCON_SEC0_REG7_Unpack_out_data_format_cntx7_ADDR32         83
    THCON_SEC0_REG8_Row_start_section_size_ADDR32               84
    THCON_SEC0_REG8_Exp_section_size_ADDR32                     84
    THCON_SEC0_REG8_L1_Dest_addr_ADDR32                         85        [+]
    THCON_SEC0_REG8_Disable_zero_compress_ADDR32                86
    THCON_SEC0_REG8_Add_l1_dest_addr_offset_ADDR32              86
    THCON_SEC0_REG8_Addr_cnt_context_ADDR32                     86
    THCON_SEC0_REG8_Out_data_format_ADDR32                      86
    THCON_SEC0_REG8_In_data_format_ADDR32                       86
    THCON_SEC0_REG8_Dis_shared_exp_assembler_ADDR32             86
    THCON_SEC0_REG8_Force_pack_per_max_xy_plane_ADDR32          86
    THCON_SEC0_REG8_Enable_out_fifo_ADDR32                      86
    THCON_SEC0_REG8_Sub_l1_tile_header_size_ADDR32              86
    THCON_SEC0_REG8_Source_interface_selection_ADDR32           86
    THCON_SEC0_REG8_Add_tile_header_size_ADDR32                 86
    THCON_SEC0_REG8_L1_source_addr_ADDR32                       86
    THCON_SEC0_REG8_Downsample_mask_ADDR32                      87
    THCON_SEC0_REG8_Downsample_rate_ADDR32                      87
    THCON_SEC0_REG8_Read_mode_ADDR32                            87
    THCON_SEC0_REG8_Exp_threshold_en_ADDR32                     87
    THCON_SEC0_REG8_Pack_L1_Acc_ADDR32                          87
    THCON_SEC0_REG8_Disable_pack_zero_flags_ADDR32              87
    THCON_SEC0_REG8_Exp_threshold_ADDR32                        87
    THCON_SEC0_REG9_Pack_0_2_limit_address_ADDR32               88
    THCON_SEC0_REG9_Pack_0_2_fifo_size_ADDR32                   89
    THCON_SEC0_REG9_Pack_1_3_limit_address_ADDR32               90
    THCON_SEC0_REG9_Pack_1_3_fifo_size_ADDR32                   91
    THCON_SEC1_REG0_TileDescriptor_ADDR32                       92        [+%]
    THCON_SEC1_REG1_Row_start_section_size_ADDR32               96
    THCON_SEC1_REG1_Exp_section_size_ADDR32                     96
    THCON_SEC1_REG1_L1_Dest_addr_ADDR32                         97        [+0]
    THCON_SEC1_REG1_Disable_zero_compress_ADDR32                98
    THCON_SEC1_REG1_Add_l1_dest_addr_offset_ADDR32              98
    THCON_SEC1_REG1_Addr_cnt_context_ADDR32                     98
    THCON_SEC1_REG1_Out_data_format_ADDR32                      98
    THCON_SEC1_REG1_In_data_format_ADDR32                       98
    THCON_SEC1_REG1_Dis_shared_exp_assembler_ADDR32             98
    THCON_SEC1_REG1_Force_pack_per_max_xy_plane_ADDR32          98
    THCON_SEC1_REG1_Enable_out_fifo_ADDR32                      98
    THCON_SEC1_REG1_Sub_l1_tile_header_size_ADDR32              98
    THCON_SEC1_REG1_Source_interface_selection_ADDR32           98
    THCON_SEC1_REG1_All_pack_disable_zero_compress_ADDR32       98
    THCON_SEC1_REG1_All_pack_disable_zero_compress_ovrd_ADDR32  98
    THCON_SEC1_REG1_Add_tile_header_size_ADDR32                 98
    THCON_SEC1_REG1_L1_source_addr_ADDR32                       98
    THCON_SEC1_REG1_Downsample_mask_ADDR32                      99
    THCON_SEC1_REG1_Downsample_rate_ADDR32                      99
    THCON_SEC1_REG1_Read_mode_ADDR32                            99
    THCON_SEC1_REG1_Exp_threshold_en_ADDR32                     99
    THCON_SEC1_REG1_Pack_L1_Acc_ADDR32                          99
    THCON_SEC1_REG1_Disable_pack_zero_flags_ADDR32              99
    THCON_SEC1_REG1_Exp_threshold_ADDR32                        99
    THCON_SEC1_REG2_Out_data_format_ADDR32                     100
    THCON_SEC1_REG2_Throttle_mode_ADDR32                       100
    THCON_SEC1_REG2_Context_count_ADDR32                       100
    THCON_SEC1_REG2_Haloize_mode_ADDR32                        100
    THCON_SEC1_REG2_Tileize_mode_ADDR32                        100
    THCON_SEC1_REG2_Unpack_Src_Reg_Set_Upd_ADDR32              100
    THCON_SEC1_REG2_Unpack_If_Sel_ADDR32                       100
    THCON_SEC1_REG2_Upsample_rate_ADDR32                       100
    THCON_SEC1_REG2_Ovrd_data_format_ADDR32                    100
    THCON_SEC1_REG2_Upsample_and_interleave_ADDR32             100
    THCON_SEC1_REG2_Shift_amount_cntx0_ADDR32                  100
    THCON_SEC1_REG2_Shift_amount_cntx1_ADDR32                  100
    THCON_SEC1_REG2_Shift_amount_cntx2_ADDR32                  100
    THCON_SEC1_REG2_Shift_amount_cntx3_ADDR32                  100
    THCON_SEC1_REG2_Disable_zero_compress_cntx0_ADDR32         101
    THCON_SEC1_REG2_Disable_zero_compress_cntx1_ADDR32         101
    THCON_SEC1_REG2_Disable_zero_compress_cntx2_ADDR32         101
    THCON_SEC1_REG2_Disable_zero_compress_cntx3_ADDR32         101
    THCON_SEC1_REG2_Unpack_if_sel_cntx0_ADDR32                 101
    THCON_SEC1_REG2_Unpack_if_sel_cntx1_ADDR32                 101
    THCON_SEC1_REG2_Unpack_if_sel_cntx2_ADDR32                 101
    THCON_SEC1_REG2_Unpack_if_sel_cntx3_ADDR32                 101
    THCON_SEC1_REG2_Force_shared_exp_ADDR32                    101
    THCON_SEC1_REG2_Disable_zero_compress_cntx4_ADDR32         101
    THCON_SEC1_REG2_Disable_zero_compress_cntx5_ADDR32         101
    THCON_SEC1_REG2_Disable_zero_compress_cntx6_ADDR32         101
    THCON_SEC1_REG2_Disable_zero_compress_cntx7_ADDR32         101
    THCON_SEC1_REG2_Unpack_if_sel_cntx4_ADDR32                 101
    THCON_SEC1_REG2_Unpack_if_sel_cntx5_ADDR32                 101
    THCON_SEC1_REG2_Unpack_if_sel_cntx6_ADDR32                 101
    THCON_SEC1_REG2_Unpack_if_sel_cntx7_ADDR32                 101
    THCON_SEC1_REG2_Unpack_limit_address_ADDR32                102
    THCON_SEC1_REG2_Unpack_fifo_size_ADDR32                    103
    THCON_SEC1_REG3_Base_address_ADDR32                        104        [+]
    THCON_SEC1_REG3_Base_cntx1_address_ADDR32                  105        [+]
    THCON_SEC1_REG3_Base_cntx2_address_ADDR32                  106        [+]
    THCON_SEC1_REG3_Base_cntx3_address_ADDR32                  107        [+]
    THCON_SEC1_REG4_Base_cntx4_address_ADDR32                  108        [+]
    THCON_SEC1_REG4_Base_cntx5_address_ADDR32                  109        [+]
    THCON_SEC1_REG4_Base_cntx6_address_ADDR32                  110        [+]
    THCON_SEC1_REG4_Base_cntx7_address_ADDR32                  111        [+]
    THCON_SEC1_REG5_Dest_cntx0_address_ADDR32                  112
    THCON_SEC1_REG5_Dest_cntx1_address_ADDR32                  112
    THCON_SEC1_REG5_Dest_cntx2_address_ADDR32                  113
    THCON_SEC1_REG5_Dest_cntx3_address_ADDR32                  113
    THCON_SEC1_REG5_Tile_x_dim_cntx0_ADDR32                    114
    THCON_SEC1_REG5_Tile_x_dim_cntx1_ADDR32                    114
    THCON_SEC1_REG5_Tile_x_dim_cntx2_ADDR32                    115
    THCON_SEC1_REG5_Tile_x_dim_cntx3_ADDR32                    115
    THCON_SEC1_REG6_Source_address_ADDR32                      116
    THCON_SEC1_REG6_Destination_address_ADDR32                 117
    THCON_SEC1_REG6_Buffer_size_ADDR32                         118
    THCON_SEC1_REG6_Transfer_direction_ADDR32                  118
    THCON_SEC1_REG7_Offset_address_ADDR32                      120        [+]
    THCON_SEC1_REG7_Unpack_data_format_cntx0_ADDR32            120
    THCON_SEC1_REG7_Unpack_out_data_format_cntx0_ADDR32        120
    THCON_SEC1_REG7_Unpack_data_format_cntx4_ADDR32            120
    THCON_SEC1_REG7_Unpack_out_data_format_cntx4_ADDR32        120
    THCON_SEC1_REG7_Offset_cntx1_address_ADDR32                121        [+]
    THCON_SEC1_REG7_Unpack_data_format_cntx1_ADDR32            121
    THCON_SEC1_REG7_Unpack_out_data_format_cntx1_ADDR32        121
    THCON_SEC1_REG7_Unpack_data_format_cntx5_ADDR32            121
    THCON_SEC1_REG7_Unpack_out_data_format_cntx5_ADDR32        121
    THCON_SEC1_REG7_Offset_cntx2_address_ADDR32                122        [+]
    THCON_SEC1_REG7_Unpack_data_format_cntx2_ADDR32            122
    THCON_SEC1_REG7_Unpack_out_data_format_cntx2_ADDR32        122
    THCON_SEC1_REG7_Unpack_data_format_cntx6_ADDR32            122
    THCON_SEC1_REG7_Unpack_out_data_format_cntx6_ADDR32        122
    THCON_SEC1_REG7_Offset_cntx3_address_ADDR32                123        [+]
    THCON_SEC1_REG7_Unpack_data_format_cntx3_ADDR32            123
    THCON_SEC1_REG7_Unpack_out_data_format_cntx3_ADDR32        123
    THCON_SEC1_REG7_Unpack_data_format_cntx7_ADDR32            123
    THCON_SEC1_REG7_Unpack_out_data_format_cntx7_ADDR32        123
    THCON_SEC1_REG8_Row_start_section_size_ADDR32              124
    THCON_SEC1_REG8_Exp_section_size_ADDR32                    124
    THCON_SEC1_REG8_L1_Dest_addr_ADDR32                        125        [+0]
    THCON_SEC1_REG8_Disable_zero_compress_ADDR32               126
    THCON_SEC1_REG8_Add_l1_dest_addr_offset_ADDR32             126
    THCON_SEC1_REG8_Addr_cnt_context_ADDR32                    126
    THCON_SEC1_REG8_Out_data_format_ADDR32                     126
    THCON_SEC1_REG8_In_data_format_ADDR32                      126
    THCON_SEC1_REG8_Dis_shared_exp_assembler_ADDR32            126
    THCON_SEC1_REG8_Force_pack_per_max_xy_plane_ADDR32         126
    THCON_SEC1_REG8_Enable_out_fifo_ADDR32                     126
    THCON_SEC1_REG8_Sub_l1_tile_header_size_ADDR32             126
    THCON_SEC1_REG8_Source_interface_selection_ADDR32          126
    THCON_SEC1_REG8_Add_tile_header_size_ADDR32                126
    THCON_SEC1_REG8_L1_source_addr_ADDR32                      126
    THCON_SEC1_REG8_Downsample_mask_ADDR32                     127
    THCON_SEC1_REG8_Downsample_rate_ADDR32                     127
    THCON_SEC1_REG8_Read_mode_ADDR32                           127
    THCON_SEC1_REG8_Exp_threshold_en_ADDR32                    127
    THCON_SEC1_REG8_Pack_L1_Acc_ADDR32                         127
    THCON_SEC1_REG8_Disable_pack_zero_flags_ADDR32             127
    THCON_SEC1_REG8_Exp_threshold_ADDR32                       127
    THCON_SEC1_REG9_Pack_0_2_limit_address_ADDR32              128
    THCON_SEC1_REG9_Pack_0_2_fifo_size_ADDR32                  129
    THCON_SEC1_REG9_Pack_1_3_limit_address_ADDR32              130
    THCON_SEC1_REG9_Pack_1_3_fifo_size_ADDR32                  131

// Registers for GLOBAL

//    GLOBAL_CFGREG_BASE_ADDR32                            152

    DEST_TARGET_REG_CFG_PACK_SEC0_Offset_ADDR32          152        [+]
    DEST_TARGET_REG_CFG_PACK_SEC0_ZOffset_ADDR32         152
    DEST_TARGET_REG_CFG_PACK_SEC1_Offset_ADDR32          153        [+]
    DEST_TARGET_REG_CFG_PACK_SEC1_ZOffset_ADDR32         153
    DEST_TARGET_REG_CFG_PACK_SEC2_Offset_ADDR32          154        [+]
    DEST_TARGET_REG_CFG_PACK_SEC2_ZOffset_ADDR32         154
    DEST_TARGET_REG_CFG_PACK_SEC3_Offset_ADDR32          155        [+]
    DEST_TARGET_REG_CFG_PACK_SEC3_ZOffset_ADDR32         155
    CG_SRC_PIPELINE_GateSrcAPipeEn_ADDR32                156
    CG_SRC_PIPELINE_GateSrcBPipeEn_ADDR32                156
    RISCV_IC_INVALIDATE_InvalidateAll_ADDR32             157
    TRISC_RESET_PC_SEC0_PC_ADDR32                        158
    TRISC_RESET_PC_SEC1_PC_ADDR32                        159
    TRISC_RESET_PC_SEC2_PC_ADDR32                        160
    TRISC_RESET_PC_OVERRIDE_Reset_PC_Override_en_ADDR32  161
    NCRISC_RESET_PC_PC_ADDR32                            162
    PRNG_SEED_Seed_Val_ADDR32                            164
    INT_DESCALE_VALUES_SEC0_Value_ADDR32                 165
    INT_DESCALE_VALUES_SEC1_Value_ADDR32                 166
    INT_DESCALE_VALUES_SEC2_Value_ADDR32                 167
    INT_DESCALE_VALUES_SEC3_Value_ADDR32                 168
    INT_DESCALE_VALUES_SEC4_Value_ADDR32                 169
    INT_DESCALE_VALUES_SEC5_Value_ADDR32                 170
    INT_DESCALE_VALUES_SEC6_Value_ADDR32                 171
    INT_DESCALE_VALUES_SEC7_Value_ADDR32                 172
    INT_DESCALE_VALUES_SEC8_Value_ADDR32                 173
    INT_DESCALE_VALUES_SEC9_Value_ADDR32                 174
    INT_DESCALE_VALUES_SEC10_Value_ADDR32                175
    INT_DESCALE_VALUES_SEC11_Value_ADDR32                176
    INT_DESCALE_VALUES_SEC12_Value_ADDR32                177
    INT_DESCALE_VALUES_SEC13_Value_ADDR32                178
    INT_DESCALE_VALUES_SEC14_Value_ADDR32                179
    INT_DESCALE_VALUES_SEC15_Value_ADDR32                180
    TRISC_END_PC_SEC0_PC_ADDR32                          181
    TRISC_END_PC_SEC1_PC_ADDR32                          182
    TRISC_END_PC_SEC2_PC_ADDR32                          183
    BRISC_END_PC_PC_ADDR32                               184
    NOC_RISC_END_PC_PC_ADDR32                            185
    RISC_PREFETCH_CTRL_Enable_Trisc_ADDR32               186
    RISC_PREFETCH_CTRL_Enable_Brisc_ADDR32               186
    RISC_PREFETCH_CTRL_Enable_NocRisc_ADDR32             186
    RISC_PREFETCH_CTRL_Max_Req_Count_ADDR32              186
```



