
#include "smartdsp_os.h"
#include "msc711x_init.h"

#include "msc711x_tdm.h"


uint8_t interleaved_buffer_rx[TDM_IB_SIZE(MSC711X_TDM_RX_CH_NUM_ACTIVE, CH_RX_SIZE,
CH_RX_BUFF_SIZE)];
uint8_t interleaved_buffer_tx[TDM_IB_SIZE(MSC711X_TDM_TX_CH_NUM_ACTIVE, CH_TX_SIZE,
CH_TX_BUFF_SIZE)];




#if (MSC711X_DMA == ON)

msc711x_dma_config_t msc711x_dma_config =
{
    2,                      ///< Group 1 priority. 0.3.
    1,                      ///< Group 0 priority. 0.3.
    TRUE,         ///< Group round robin.
    TRUE,         ///< Channel round robin.
    FALSE,
    TRUE
};

#endif // MSC711X_DMA

#if ((MSC711X_TDM0 == ON) || (MSC711X_TDM1 == ON) || (MSC711X_TDM2 == ON))



msc711x_tdm_init_params_t msc711x_tdm_init_params =
{
    FALSE,             /* Loop back mode. */
    TRUE,              /* Recieve and Transmit Sharing. */
    TRUE,             /* Transmit T1 Frame.            */
    FALSE,             /* Receive T1 Frame.             */
    FALSE,              /* Transmit Frame Sync Edge.     */
 
    FALSE,              /* Receive Frame Sync Edge.      */
    FALSE,             /* Common TDM Signals.           */
    TRUE,              /* Recieve Sync Output.          */
    TRUE,              /* sync_out equal channel width. */
   FALSE,              /* Recieve Clock Output Enable.  */
    FALSE,             /* Recieve Sync Active.          */
    TRUE,              /* Recieve Data Edge.            */
    TRUE,              /* Revieve Reversed Data Order.  */
 
    TRUE,   		   /* Transmit Sync Output.         */
    FALSE,   		   /* sync_out equal channel width. */
    TRUE,    		   /* Transmit Clock Output Enable. */
    FALSE,             /* Transmit Sync Active.         */
   FALSE,              /* Transmit Data Edge.           */
    TRUE,              /* Transmit Reversed Data Order. */
    TDM_TFP_CS_16_BIT, /* Transmit Channel Size.        */
 
    TDM_RFP_CS_16_BIT, /* Recieve Channel Size.         */
    0x02,         	   /* Receive number of channels.   */
    0x02,         	   /* Transmit number of channels.  */
    0x00,              /* Recieve Frame Sync Delay.     */
    0x00,              /* Transmit Frame Sync Delay.    */
	OS_HWI_PRIORITY4,  /* Error interrupts priority. 	*/
    	TRUE,														////////// FALSE
    	
    	{
			interleaved_buffer_rx, 			/* Where the driver dumps interleaved data. */
			CH_RX_BUFF_SIZE, 				/* Data buffer size. */
			MSC711X_TDM_RX_CH_NUM_ACTIVE, 	/* Must be constant. */
			/* Channels that are activated in initialization. */
			ACTIVE_RX_CHANNELS,
			OS_HWI_PRIORITY0
		},
		{
			interleaved_buffer_tx, 			/* Where the driver dumps interleaved data. */
			CH_TX_BUFF_SIZE, 				/* Data buffer size. */
			MSC711X_TDM_TX_CH_NUM_ACTIVE, 	/* Must be constant. */
			/* Channels that are activated in initialization. */
			ACTIVE_TX_CHANNELS,
			OS_HWI_PRIORITY0
		}

};


#if (MSC711X_TDM0 == ON)
msc711x_tdm_init_params_t *msc711x_tdm0_init_params = &msc711x_tdm_init_params;
#endif // MSC711x_TDM0

#if (MSC711X_TDM1 == ON)
msc711x_tdm_init_params_t *msc711x_tdm1_init_params = &msc711x_tdm_init_params;
#endif // MSC711x_TDM1

#if (MSC711X_TDM2 == ON)
msc711x_tdm_init_params_t *msc711x_tdm2_init_params = &msc711x_tdm_init_params;
#endif // MSC711x_TDM2

#endif // MSC711x_TDM

#if (MSC711X_HDI == ON)
msc711x_hdi_init_params_t msc711x_hdi_init_params =
{
	FALSE; /* If true, HDI is 8 bit, otherwise - 16 bit. */
};
#endif // MSC711X_HDI

