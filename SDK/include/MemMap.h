/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2007 - 2023 by Vector Informatik GmbH.                                           All rights reserved.
 *
 *                Please note, that this file contains example configuration used by the
 *                MICROSAR BSW. This code may influence the behaviour of the MICROSAR BSW
 *                in principle. Therefore, great care must be taken to verify
 *                the correctness of the implementation.
 *
 *                The contents of the originally delivered files are only examples respectively
 *                implementation proposals. With regard to the fact that these functions
 *                are meant for demonstration purposes only, the liability of Vector Informatik
 *                shall be expressly excluded in cases of ordinary negligence,
 *                to the extent admissible by law or statute.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  _MemMap.h
 *    Component:  -
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  This File is a template for the MemMap.h
 *                This file has to be extended with the memory section defined for all BSW modules
 *                which are used.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  MISRA VIOLATIONS
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Joachim Kalmbach              Jk            Vector Informatik GmbH
 *  Heike Honert                  Ht            Vector Informatik GmbH
 *  Eugen Stripling               Seu           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2007-08-01  Jk                    Initial creation
 *  01.01.00  2007-12-14  Jk                    Component specific defines filtering added
 *  01.01.02  2008-11-04  Jk                    Component specific defines filtering added
 *  01.01.03  2008-12-17  Ht                    Improve list of components  (Tp_AsrTpCan,Tp_AsrTpFr,DrvMcu,DrvIcu added)
 *  01.01.04  2009-04-27  Ht                    improve list of components  (Cp_XcpOnCanAsr, Il_AsrIpduM, If_VxFblDcm,
 *                                              If_VxFblVpm_Volvo_ab, DrvFls added)
 *  01.01.05  2009-04-24  Msr                   Renamed J1939_AsrBase as TpJ1939_AsrBase
 *  01.01.06  2009-06-03  Ht                    Improve list of components (Adc, Dio, Gpt, Pwm, Spi, Wdg, Fls, Port, Fim)
 *  01.02.00  2009-08-01  Ht                    Improve list of components (Fee_30_Inst2, Can, ...Sub)
 *                                              Support filtering for RTE
 *  01.02.01  2009-08-18  HH                    replaced C++ comment by C comment
 *  01.02.02  2009-09-02  Lo                    add external Flash driver support
 *  01.02.03  2009-09-12  Lo                    add DrvFls_Mcs12xFslftm01ExtVx
 *                        Ht                    Improve list of components (CanTrcv_30_Tja1040dio,
 *                                                Eth, EthTrcv, EthIf, SoAd, TcpIp, EthSM)
 *  01.03.00  2009-10-30  Ht                    support R8: change EthTrcv to EthTrcv_30_Canoeemu
 *                                              support EthTrcv_30_Dp83848
 *                                              change CanTrcv_30_Xdio to CanTrcv_30___Your_Trcv__
 *                                              change CanTrcv_30_Tja1040dio to CanTrcv_30_Tja1041
 *                                              change name FrTrcv to FrTrcv_30_Tja1080dio
 *                        Lo                    add Cp_AsrXcp
 *                        Ht                    add Cp_XcpOnFrAsr
 *  01.03.01  2010-01-13  Ht                    support SysService_AsrCal
 *  01.03.02  2010-02-15  Ht                    support SysService_SswRcs_Daimler, SysService_Tls, Tp_Http,
 *                                                      SysService_Dns, SysService_Json, DrvTrans_GenericLindioAsr
 *                        Lo                    add Diag_AsrDem for all OEMs
 *                                              rename internal variables and filter methods
 *  01.04.00  2010-03-04  Ht                    change name FrTrcv_30_Tja1080dio to FrTrcv_30_Tja1080
 *  01.04.01  2010-03-10  Ht                    support DrvTrans_GenericFrAsr, DrvTrans_As8223FrspiAsr, DrvEep and If_AsrIfEa
 *  01.04.02  2010-04-07  Lo                    change IfFee to real components and add If_AsrIfWdV85xNec01Sub
 *  01.04.03  2010-06-11  Ht                    add CanTrcv_30_Tja1043
 *                        Lo                    add Il_AsrIpduMEbBmwSub
 *  01.04.04  2010-08-24  Ht                    add CanTrcv_30_Tle62512G, DrvEep_XAt25128EAsr, Tp_AsrTpFrEbBmwSub
 *  01.05.00  2010-08-24  Ht                    support R10:
 *                                              change LinTrcv_30_Tle7259dio to LinTrcv_30_Tle7259
 *  01.05.01  2010-10-14  Ht                    add VStdLib, SysService_SswScc, SysService_IpBase, SysService_Crypto
 *  01.05.02  2010-10-20  Ht                    support comments for Package Merge Tool
 *  01.05.03  2010-11-03  Ht                    add SysService_E2eLibTttechSub, SysService_E2ePwTttechSub
 *  01.05.04  2010-11-16  Ht                    add SysService_Exi, DrvTrans_Int6400EthAsr, Cdd_AsrCdd_Fiat, Diag_AsrDem_Fiat
 *  01.05.05  2010-12-17  Ht                    add SysService_AsrSchM, DrvEep_XXStubAsr, DrvIcu_Tms570Tinhet01ExtVx
 *                                                  DrvWd_XTle4278gEAsr, DrvWd_XXStubAsr
 *  01.05.06  2011-02-17  Ht                    add DrvEed, SysService_AsrBswM
 *  01.05.07  2011-03-04  Ht                    add DrvTrans_Tja1055CandioAsr
 *                                              rename CanTrcv_30_Tja1040dio to CanTrcv_30_Tja1040
 *                                              add SysService_XmlEngine
 *  01.06.00  2011-03-15  Ht                    support ASR4.0
 *                                              add Ccl_Asr4ComM, Ccl_Asr4SmCan, Nm_Asr4NmIf, Nm_AsrNmDirOsek
 *  01.06.01  2011-04-15  Ht                    add Diag_AsrDcm_<OEM>
 *  01.06.02  2011-06-17  Ht                    correct Diag_AsrDcm_<OEM>
 *                                              add Monitoring_AsrDlt and Monitoring_GenericMeasurement
 *  01.06.03  2011-09-20  Ht                    add DrvTrans_Tja1145CanSpiAsr, DrvTrans_E52013CanspiAsr, DrvFls_XXStubAsr,
 *                                              If_AsrIfFeeV85xNec05Sub, If_AsrIfFeeV85xNec06Sub, If_AsrIfFeeV85xNec07Sub
 *                                              SysService_AsrWdMTttechSub and If_AsrIfWdTttechSub
 *  01.06.04  2011-11-22  Ht                    add If_AsrIfFeeTiSub,
 *                                              ESCAN00054718: add Cdd_AsrCdd
 *  01.06.05  2011-12-09  Ht                    add Tp_IpV4, Tp_IpV6
 *  01.06.06  2011-12-14  Ht                    add Monitoring_RuntimeMeasurement
 *  01.06.07  2012-01-03  Ht                    add DrvI2c, SysService_Asr4BswM
 *  01.06.08  2012-01-31  Ht                    add DrvTrans_Ar7000EthAsr, DrvTrans_GenericEthmiiAsr
 *  01.06.09  2012-03-06  Ht                    add If_AsrIfFeeMb9df126Fuji01Sub,
 *                                              Infineon_Tc1767Inf01Sub, Infineon_Tc178xInf01Sub, Infineon_Tc1797Inf01Sub, Infineon_Tc1797Inf02Sub
 *  01.06.10  2012-03-13  Ht                    add Gw_AsrPduRCfg5, Il_AsrComCfg5, Il_AsrIpduMCfg5, Cdd_AsrCddCfg5,
 *                                              Tp_Asr4TpCan, Diag_Asr4Dcm, Diag_Asr4Dem
 *  01.06.11  2012-03-20  Ht                    add Cp_AsrCcp, Cp_XcpOnTcpIpAsr
 *  01.07.00  2012-07-26  Ht                    ESCAN00059365: [AUTOSAR4, compiler warning]: Wrong define name in #undef statement causes re-definition warning
 *                                              add Nm_Asr4NmCan, Nm_Asr4NmFr, Infineon_Xc2000Inf01Sub, Ccl_Asr4ComMCfg5, SysService_Asr4BswMCfg5, SysService_Asr4EcuM, SysService_AsrRamTst,
 *                                                  Ccl_Asr4SmLin
 *            2012-09-04  Ht                    add support for ASR specification 4.0 R3
 *  01.07.01  2012-10-23  Seu                   add SysService_XmlSecurity
 *  01.07.02  2013-01-10  Seu                   MISRA deviation comments added
 *  01.08.00  2013-03-01  Seu    ESCAN00065501  AR4-325: Add support for Post-build RAM memory section
 *            2013-04-12  Seu    ESCAN00066614  Add the deviation for violation of MISRA rule 19.6
 *  01.09.00  2013-12-10  Seu    ESCAN00072537  Add *_NOCACHE_* memory sections for variables
 *            2013-12-16  Seu                   MISRA compliance: usage of character "'" removed, typos corrected
 *  01.10.00  2016-09-27  Seu                   FEATC-317: FEAT-2002: CommonAsr__Common: Support 64 Bit Signal Types for COM according to ASR 4.2.2
 *  01.11.00  2017-07-05  Seu    ESCAN00095756  FEAT-2455: Support ASR4.2 compatible MemMap for MCALs
 *            2017-08-01  Seu    ESCAN00096129  MEMMAP_SW_MINOR_VERSION / MEM_SW_MINOR_VERSION is not correct
 *********************************************************************************************************************/

/* PRQA S 0841 MEMMAP_0841_TAG */ /* MD_MSR_19.6 */

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

 /* PRQA S 0883 1 */ /* MD_MemMap_19.15 */
#undef MEMMAP_VENDOR_ID /* PRQA S 0841 */ /* MD_MSR_19.6 */

#define MEMMAP_VENDOR_ID        (30u)

/* AUTOSAR Software Specification Version Information */


# undef MEMMAP_AR_MAJOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEMMAP_AR_MINOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEMMAP_AR_PATCH_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */

/* AUTOSAR Document version 1.1.0 part of release 3.0.1 */
# define MEMMAP_AR_MAJOR_VERSION       (1u)
# define MEMMAP_AR_MINOR_VERSION       (1u)
# define MEMMAP_AR_PATCH_VERSION       (0u)

/* compatiblity to older versions */
# undef MEM_AR_MAJOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEM_AR_MINOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEM_AR_PATCH_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */

# define MEM_AR_MAJOR_VERSION          MEMMAP_AR_MAJOR_VERSION
# define MEM_AR_MINOR_VERSION          MEMMAP_AR_MINOR_VERSION
# define MEM_AR_PATCH_VERSION          MEMMAP_AR_PATCH_VERSION


# undef MEMMAP_SW_MAJOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEMMAP_SW_MINOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEMMAP_SW_PATCH_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */

# define MEMMAP_SW_MAJOR_VERSION     (1u)
# define MEMMAP_SW_MINOR_VERSION     (11u)
# define MEMMAP_SW_PATCH_VERSION     (0u)

/* compatiblity to older versions */
# undef MEM_SW_MAJOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEM_SW_MINOR_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */
# undef MEM_SW_PATCH_VERSION /* PRQA S 0841 */ /* MD_MSR_19.6 */

# define MEM_SW_MAJOR_VERSION        MEMMAP_SW_MAJOR_VERSION
# define MEM_SW_MINOR_VERSION        MEMMAP_SW_MINOR_VERSION
# define MEM_SW_PATCH_VERSION        MEMMAP_SW_PATCH_VERSION

#define MEMMAP_ERROR

/* Package Merger: Start Section MemMapModuleList */



/**********************************************************************************************************************
 *  Cdd_AsrCdd START
 *********************************************************************************************************************/
/* Copy the memory mapping defines for each of your configured CDDs and replace the prefix _CDD with the MSN of your configured CDD as higher case. */

/*******  CODE sections **********************************************************************************************/

#ifdef _CDD_START_SEC_CODE
# undef _CDD_START_SEC_CODE
# define START_SEC_CODE
#endif
#ifdef _CDD_STOP_SEC_CODE
# undef _CDD_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif

/* Add additional memory mapping defines for each of you configured CDDs here. */

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

/* FAST CONST sections */

/* Postbuild CFG CONST sections */

/* Root table for postbuild data */

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

/* VAR NOINIT sections */

/* VAR ZERO INIT sections */

/* VAR FAST INIT sections */

/* VAR FAST NOINIT sections */

/* VAR FAST ZERO INIT sections */

/**********************************************************************************************************************
 *  Cdd_AsrCdd END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CRYPTO_30_LIBCV START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef CRYPTO_30_LIBCV_START_SEC_CODE
# undef CRYPTO_30_LIBCV_START_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CODE                                               /* mapped to default code section */
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_CODE
# undef CRYPTO_30_LIBCV_STOP_SEC_CODE      /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CODE                                                /* default code stop section */
#endif

#ifdef CRYPTO_30_LIBCV_START_SEC_APPL_CODE
# undef CRYPTO_30_LIBCV_START_SEC_APPL_CODE        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CODE                                               /* mapped to default code section */
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_APPL_CODE
# undef CRYPTO_30_LIBCV_STOP_SEC_APPL_CODE      /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CODE                                                /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef CRYPTO_30_LIBCV_START_SEC_CONST_UNSPECIFIED
# undef CRYPTO_30_LIBCV_START_SEC_CONST_UNSPECIFIED        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_CONST_UNSPECIFIED
# undef CRYPTO_30_LIBCV_STOP_SEC_CONST_UNSPECIFIED        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CONST
#endif

#ifdef CRYPTO_30_LIBCV_START_SEC_CONST_8BIT
# undef CRYPTO_30_LIBCV_START_SEC_CONST_8BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CONST_8BIT
#endif

#ifdef CRYPTO_30_LIBCV_STOP_SEC_CONST_8BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_CONST_8BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_UNSPECIFIED        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif

#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_VAR
#endif

#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_8BIT
# undef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_8BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_8BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_8BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_VAR
#endif

#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_16BIT
# undef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_16BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_16BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_16BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_VAR
#endif

#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_32BIT
# undef CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_32BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_32BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_32BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef CRYPTO_30_LIBCV_START_SEC_VAR_ZERO_INIT_8BIT
# undef CRYPTO_30_LIBCV_START_SEC_VAR_ZERO_INIT_8BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef CRYPTO_30_LIBCV_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef CRYPTO_30_LIBCV_STOP_SEC_VAR_ZERO_INIT_8BIT        /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  CRYPTO_30_LIBCV END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  Security_vSecPrim START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef VSECPRIM_START_SEC_CODE
# undef VSECPRIM_START_SEC_CODE                           /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CODE
#endif
#ifdef VSECPRIM_STOP_SEC_CODE
# undef VSECPRIM_STOP_SEC_CODE                            /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

#ifdef VSECPRIM_START_SEC_CONST_UNSPECIFIED
# undef VSECPRIM_START_SEC_CONST_UNSPECIFIED            /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# undef VSECPRIM_STOP_SEC_CONST_UNSPECIFIED             /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CONST
#endif

#ifdef VSECPRIM_START_SEC_CONST_8BIT
# undef VSECPRIM_START_SEC_CONST_8BIT                     /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CONST_8BIT
#endif
#ifdef VSECPRIM_STOP_SEC_CONST_8BIT
# undef VSECPRIM_STOP_SEC_CONST_8BIT                      /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CONST
#endif

#ifdef VSECPRIM_START_SEC_CONST_32BIT
# undef VSECPRIM_START_SEC_CONST_32BIT                     /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CONST_32BIT
#endif
#ifdef VSECPRIM_STOP_SEC_CONST_32BIT
# undef VSECPRIM_STOP_SEC_CONST_32BIT                      /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CONST
#endif

#ifdef VSECPRIM_START_SEC_CONST_64BIT
# undef VSECPRIM_START_SEC_CONST_64BIT                     /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CONST_64BIT
#endif
#ifdef VSECPRIM_STOP_SEC_CONST_64BIT
# undef VSECPRIM_STOP_SEC_CONST_64BIT                      /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CONST
#endif

/*******  VAR sections  ********************************************************************************************/


/*******  CONST sections  ********************************************************************************************/


/**********************************************************************************************************************
 *  Security_vSecPrim END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  SYSSERVICE_ASRCRC START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/
#ifdef CRC_START_SEC_CODE
    #undef CRC_START_SEC_CODE
    #define START_SEC_CODE
#endif
#ifdef CRC_STOP_SEC_CODE
    #undef CRC_STOP_SEC_CODE
    #define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/
#ifdef CRC_START_SEC_CONST_8
    #undef CRC_START_SEC_CONST_8
    #define START_SEC_CONST_8
#endif
#ifdef CRC_STOP_SEC_CONST_8
    #undef CRC_STOP_SEC_CONST_8
    #define STOP_SEC_CONST
#endif

#ifdef CRC_START_SEC_CONST_16
    #undef CRC_START_SEC_CONST_16
    #define START_SEC_CONST_16
#endif
#ifdef CRC_STOP_SEC_CONST_16
    #undef CRC_STOP_SEC_CONST_16
    #define STOP_SEC_CONST
#endif

#ifdef CRC_START_SEC_CONST_32
    #undef CRC_START_SEC_CONST_32
    #define START_SEC_CONST_32
#endif
#ifdef CRC_STOP_SEC_CONST_32
    #undef CRC_STOP_SEC_CONST_32
    #define STOP_SEC_CONST
#endif

#ifdef CRC_START_SEC_CONST_64
    #undef CRC_START_SEC_CONST_64
    #define START_SEC_CONST_64
#endif
#ifdef CRC_STOP_SEC_CONST_64
    #undef CRC_STOP_SEC_CONST_64
    #define STOP_SEC_CONST
#endif

/**********************************************************************************************************************
 *  SYSSERVICE_ASRCRC END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  SysService_E2eLib START
 *********************************************************************************************************************/


/*******  CODE sections **********************************************************************************************/

#ifdef E2E_START_SEC_CODE
    #undef E2E_START_SEC_CODE
    #define START_SEC_CODE
#endif
#ifdef E2E_STOP_SEC_CODE
    #undef E2E_STOP_SEC_CODE
    #define STOP_SEC_CODE
#endif

/**********************************************************************************************************************
 *  SysService_E2eLib END
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  VSTDLIB START
 *********************************************************************************************************************/

#if defined(VSTDLIB_START_SEC_CODE)
# undef VSTDLIB_START_SEC_CODE                             /* PRQA S 0841 */ /* MD_MSR_Undef */
# define START_SEC_CODE                                    /* mapped to default code section */
#endif
#if defined(VSTDLIB_STOP_SEC_CODE)
# undef VSTDLIB_STOP_SEC_CODE                              /* PRQA S 0841 */ /* MD_MSR_Undef */
# define STOP_SEC_CODE
#endif

/**********************************************************************************************************************
 *  VSTDLIB END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  STBM START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#if defined(STBM_START_SEC_CODE)
#undef STBM_START_SEC_CODE
#define START_SEC_CODE /* mapped to default code section */
#endif
#if defined(STBM_STOP_SEC_CODE)
#undef STBM_STOP_SEC_CODE
#define STOP_SEC_CODE /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#if defined(STBM_START_SEC_CONST_UNSPECIFIED)
#undef STBM_START_SEC_CONST_UNSPECIFIED
#define START_SEC_CONST_UNSPECIFIED
#endif
#if defined(STBM_STOP_SEC_CONST_UNSPECIFIED)
#undef STBM_STOP_SEC_CONST_UNSPECIFIED
#define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR NOINIT sections */

#if defined(STBM_START_SEC_VAR_NOINIT_32BIT)
#undef STBM_START_SEC_VAR_NOINIT_32BIT
#define START_SEC_VAR_NOINIT_32BIT
#endif
#if defined(STBM_STOP_SEC_VAR_NOINIT_32BIT)
#undef STBM_STOP_SEC_VAR_NOINIT_32BIT
#define STOP_SEC_VAR
#endif

#if defined(STBM_START_SEC_VAR_NOINIT_UNSPECIFIED)
#undef STBM_START_SEC_VAR_NOINIT_UNSPECIFIED
#define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#if defined(STBM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
#undef STBM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#if defined(STBM_START_SEC_VAR_ZERO_INIT_8BIT)
#undef STBM_START_SEC_VAR_ZERO_INIT_8BIT
#define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#if defined(STBM_STOP_SEC_VAR_ZERO_INIT_8BIT)
#undef STBM_STOP_SEC_VAR_ZERO_INIT_8BIT
#define STOP_SEC_VAR
#endif

/* POST BUILD sections*/

#if defined(STBM_START_SEC_PBCFG)
#undef STBM_START_SEC_PBCFG
#define START_SEC_VAR_PBCFG
#endif

#if defined(STBM_STOP_SEC_PBCFG)
#undef STBM_STOP_SEC_PBCFG
#define STOP_SEC_VAR
#endif

/*const post build*/

#if defined(STBM_START_SEC_CONST_PBCFG)
#undef STBM_START_SEC_CONST_PBCFG
#define START_SEC_CONST_PBCFG
#endif

#if defined(STBM_STOP_SEC_CONST_PBCFG)
#undef STBM_STOP_SEC_CONST_PBCFG
#define STOP_SEC_CONST
#endif

/*var post build*/

#if defined(STBM_START_SEC_VAR_PBCFG)
#undef STBM_START_SEC_VAR_PBCFG
#define START_SEC_VAR_PBCFG
#endif

#if defined(STBM_STOP_SEC_VAR_PBCFG)
#undef STBM_STOP_SEC_VAR_PBCFG
#define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  STBM END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ETHTSYN START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef ETHTSYN_START_SEC_CODE
#undef ETHTSYN_START_SEC_CODE
#define START_SEC_CODE /* mapped to default code section */
#endif
#ifdef ETHTSYN_STOP_SEC_CODE
#undef ETHTSYN_STOP_SEC_CODE
#define STOP_SEC_CODE /* default code stop section */
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef ETHTSYN_START_SEC_CONST_8BIT
#undef ETHTSYN_START_SEC_CONST_8BIT
#define START_SEC_CONST_8BIT /* mapped to default const 8bit section */
#endif
#ifdef ETHTSYN_STOP_SEC_CONST_8BIT
#undef ETHTSYN_STOP_SEC_CONST_8BIT
#define STOP_SEC_CONST /* default const stop section */
#endif

#ifdef ETHTSYN_START_SEC_CONST_16BIT
#undef ETHTSYN_START_SEC_CONST_16BIT
#define START_SEC_CONST_16BIT
#endif
#ifdef ETHTSYN_STOP_SEC_CONST_16BIT
#undef ETHTSYN_STOP_SEC_CONST_16BIT
#define STOP_SEC_CONST
#endif

#ifdef ETHTSYN_START_SEC_CONST_32BIT
#undef ETHTSYN_START_SEC_CONST_32BIT
#define START_SEC_CONST_32BIT
#endif
#ifdef ETHTSYN_STOP_SEC_CONST_32BIT
#undef ETHTSYN_STOP_SEC_CONST_32BIT
#define STOP_SEC_CONST
#endif

#ifdef ETHTSYN_START_SEC_CONST_UNSPECIFIED
#undef ETHTSYN_START_SEC_CONST_UNSPECIFIED
#define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef ETHTSYN_STOP_SEC_CONST_UNSPECIFIED
#undef ETHTSYN_STOP_SEC_CONST_UNSPECIFIED
#define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR INIT sections */

#ifdef ETHTSYN_START_SEC_VAR_INIT_8BIT
#undef ETHTSYN_START_SEC_VAR_INIT_8BIT
#define START_SEC_VAR_INIT_8BIT /* mapped to default var init 8bit section */
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_INIT_8BIT
#undef ETHTSYN_STOP_SEC_VAR_INIT_8BIT
#define STOP_SEC_VAR /* default var stop section */
#endif

#ifdef ETHTSYN_START_SEC_VAR_INIT_16BIT
#undef ETHTSYN_START_SEC_VAR_INIT_16BIT
#define START_SEC_VAR_INIT_16BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_INIT_16BIT
#undef ETHTSYN_STOP_SEC_VAR_INIT_16BIT
#define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_INIT_32BIT
#undef ETHTSYN_START_SEC_VAR_INIT_32BIT
#define START_SEC_VAR_INIT_32BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_INIT_32BIT
#undef ETHTSYN_STOP_SEC_VAR_INIT_32BIT
#define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_INIT_UNSPECIFIED
#undef ETHTSYN_START_SEC_VAR_INIT_UNSPECIFIED
#define START_SEC_VAR_INIT_UNSPECIFIED
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef ETHTSYN_STOP_SEC_VAR_INIT_UNSPECIFIED
#define STOP_SEC_VAR
#endif

/* VAR NOINIT sections */

#ifdef ETHTSYN_START_SEC_VAR_NOINIT_8BIT
#undef ETHTSYN_START_SEC_VAR_NOINIT_8BIT
#define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_NOINIT_8BIT
#undef ETHTSYN_STOP_SEC_VAR_NOINIT_8BIT
#define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_NOINIT_16BIT
#undef ETHTSYN_START_SEC_VAR_NOINIT_16BIT
#define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_NOINIT_16BIT
#undef ETHTSYN_STOP_SEC_VAR_NOINIT_16BIT
#define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_NOINIT_32BIT
#undef ETHTSYN_START_SEC_VAR_NOINIT_32BIT
#define START_SEC_VAR_NOINIT_32BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_NOINIT_32BIT
#undef ETHTSYN_STOP_SEC_VAR_NOINIT_32BIT
#define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
#undef ETHTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
#define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#undef ETHTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef ETHTSYN_START_SEC_VAR_ZERO_INIT_8BIT
#undef ETHTSYN_START_SEC_VAR_ZERO_INIT_8BIT
#define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
#undef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
#define STOP_SEC_VAR
#endif

#ifdef ETHTSYN_START_SEC_VAR_ZERO_INIT_16BIT
#undef ETHTSYN_START_SEC_VAR_ZERO_INIT_16BIT
#define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_16BIT
#undef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_16BIT
#define STOP_SEC_VAR
#endif

#if defined(STBM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
#undef STBM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(STBM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
#undef STBM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_VAR
#endif
#if defined(ETHTSYN_START_SEC_VAR_ZERO_INIT_UNSPECIFIED)
#undef ETHTSYN_START_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#if defined(ETHTSYN_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED)
#undef ETHTSYN_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_19.6 */
#define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  ETHTSYN END
 *********************************************************************************************************************/

/* PRQA L:MEMMAP_0841_TAG */

#include "MemMap_Common.h"

/**********************************************************************************************************************
 *  END OF FILE: MemMap.h
 *********************************************************************************************************************/
