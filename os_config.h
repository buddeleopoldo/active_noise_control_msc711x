/******************************************************************************
 Copyright © 1995-2003 Metrowerks Corporation.
 All Rights Reserved

 This is unpublished proprietary source code of Metrowerks.
 The copyright notice above does not evidence any actual or intended
 publication of such source code.
 
 $Date: 2005/04/10 08:24:35 $
 $Id: os_config.h,v 1.1 2005/04/10 08:24:35 lironk Exp $
 $Source: /home/swproj/sw/REPOSITORY/dsp/SmartDSP/demos/starcore/msc711X/EVM_tdm_codec_demo/os_config.h,v $
 $Revision: 1.1 $
 **************************************************************************//*

 @File          os_config.h

 @Description   OS Configuration definitions.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_CONFIG_H
#define __OS_CONFIG_H


#include "msc711x.h"


#define ON      1
#define OFF     0


/* OS General Configuration **************************************************/

/* OS_MULTICORE should be defined as 0 or 1 by the compiler */

#define OS_HEAP_SIZE                        0x8000

#define OS_SHARED_MEM_SIZE                  0

#define OS_TICK                             OFF

#define OS_TICK_PRIORITY                    OS_SWI_PRIORITY0

#define OS_TICK_PARAMETER					MSC711X_TICK_DEFAULT

#define OS_SYSTEM_CLOCK						100 // Mhz AHB Clock

#define OS_HW_TIMERS                        OFF

#define OS_TOTAL_NUM_OF_SWI                 10

#define OS_TOTAL_NUM_OF_SW_TIMERS           0

#define OS_TOTAL_NUM_OF_QUEUES              0

#define OS_TOTAL_NUM_OF_SHARED_QUEUES       0

#define OS_TOTAL_NUM_OF_SIO_DEVICES         1

#define OS_TOTAL_NUM_OF_CIO_DEVICES         0

#define OS_TOTAL_NUM_OF_BIO_DEVICES         0


/* Architecture Configuration ************************************************/

#define MSC711X_DMA                         ON

#define MSC711X_CACHE                       OFF /* Enable cache for M2 */


/* CIO Devices ***************************************************************/

#define MSC711X_UART                        OFF

#define MSC711X_HDI				OFF

#define MSC711X_TDM0                        ON

#define MSC711X_TDM1                        OFF

#define MSC711X_TDM2                        OFF

#define OS_INTERRUPTS_ADDRESS               MSC711x_INTERRUPTS_ADDRESS

/* Application Configuration *************************************************/


#define _16BITS 2 // 2 Bytes

#define NRO_FRAMES 1

#define CH_TX_SIZE _16BITS

#define CH_RX_SIZE _16BITS

#define CH_TX_BUFF_SIZE (NRO_FRAMES * CH_TX_SIZE)

#define CH_RX_BUFF_SIZE (NRO_FRAMES * CH_RX_SIZE)

#define ACTIVE_CHANNELS 2

#define MSC711X_TDM_TX_CH_NUM_ACTIVE ACTIVE_CHANNELS

#define MSC711X_TDM_RX_CH_NUM_ACTIVE ACTIVE_CHANNELS

#define ACTIVE_TX_CHANNELS { 0, 1 }

#define ACTIVE_RX_CHANNELS ACTIVE_TX_CHANNELS


#endif // __OS_CONFIG_H
