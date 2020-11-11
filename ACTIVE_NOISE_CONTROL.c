/

/***************************************************************************************
Tìtulo: 	Proyecto integrador Ingeniería Electrónica FCEFyN UNC, ANC Multicanal

Autor: 	Leopoldo Budde

Directores: Ing. Roberto R. Rossi, Ing. Sebastián P. Ferreyra
***************************************************************************************/


//	Includes	-------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smartdsp_os.h"
#include "msc711x.h"
#include "msc711x_hwi.h"
#include "os_config.h"
#include "string.h"
#include "os_runtime.h"
#include "msc711x_tdm.h"
#include "prototype.h"


//	Prototipos ----------------------------------------------------------

void initGPIO(void);
void initEvents(void);
void initTimerA(void);
void initTimerB(void);

asm void 	FIRasm4(int16_t Lh, int16_t* punt_h, int16_t Punt_x, int16_t** PPunt_x, int16_t* x0, int16_t* Punt_y);
asm void 	FIR32asm4(int16_t Lh, int32_t* punt_h, int16_t Punt_x, int16_t** PPunt_x, , int16_t* x0, int16_t* Punt_y);
asm void 	LMS16asm4(int16_t Lf, int16_t* Punt_x, int16_t* Xinicial, int16_t* punt_mue);
asm void 	LMS32asmD4(int16_t Lf, int16_t LfP, int32_t* punt_f, int16_t* Punt_xA, int16_t* XinicialA, int16_t* Punt_xB, int16_t* XinicialB, int16_t* punt_mu, int16_t offset);


//	Variables -----------------------------------------------------------

enum {MCLK_NO_DIV = 0, MCLK_DIV_BY_2 = 1, MCLK_DIV_BY_4 = 2};	//	Fs = 8kHz
#define MCL_DIVISION	MCLK_NO_DIV
#define CANNEL_0	0
#define CHANNEL_1	1
#define TRANSMITTING_CHNNEL	CHANNEL_0

struct channels_intrleaved_t				//
{	
	sio_channel_t	sio_tx;				//	Defino estructura de		
	sio_channel_t	sio_rx;				//	canales intercalados	
};																
struct channels_interleaved_t channls_interleaved;			
uint16_t interlaved_size_rx,interleaved_size_tx;		//	Tamaño y base de arreglos
uint8_t	*interleaved_base_rx,*intereaved_base_tx;		//

int16_t chanel_input_data[2]; 				//	Arreglos de entrada y salida	
int16_t channel_output_data[2]; 				//	de a un dato, L y R



//	Variables para ANC 	-------------------------------------------------

							
 int32_t	W[400][4] = {	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},…….{0,0,0,0}};


 int16_t	Se[400][4] = {	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0,},……,{0,0,0,0}};


int16_t 	Lse  = 	400;					//	FILTRO Se
int16_t 	circbuff_v[400][4];				//
#pragma align	Se	8				//				
#pragma align	circbuff_v	8			//
int16_t*	Punt_Se	 = &Se[0][0];				//					
int16_t**PPunt_v=&Punt_v;				//			
int16_t	ve[4]={0,0,0,0};					//
int16_t* Punt_ve	 =&ve[0];				//					
int16_t	v[4]={0,0,0,0}, f[4]={0,0,0,0}, muSf[4]={0,0,0,0};  //					
int16_t*	Punt_muSf=&muSf[0];				//					
int16_t	muS	=100;					//


int16_t 	circbuff_ymv[400][4];				//	FILTRO Sea
#pragma align	circbuff_ymv	8			//
int16_t**PPunt_ymv=&Punt_ymv;			//
int16_t*	ymvinicial=&circbuff_ymv[0][0];		//	
int16_t	ymv[4]   ={0,0,0,0};				//
int16_t*	Punt_ymvN=&ymv[0];				//
int16_t* Punt_ymvse=&ymvse[0];			//


int16_t 	circbuff_deS1[400][4];				//	FILTRO Seb		
#pragma align	circbuff_deS1	8			//
int16_t*	Punt_deS1=&circbuff_deS1[0][0];		//		
int16_t	deseS1[4]={0,0,0,0};				//
int16_t* Punt_deseS1=	&deseS1[0];			//					
int16_t	deS1[4]	 ={0,0,0,0};				//
int16_t*	Punt_deS1N	&deS1[0];			//
int16_t 	circbuff_deS2[400][4];				//			
#pragma align	circbuff_deS2	8			//
int16_t**PPunt_deS2=	&Punt_deS2;			//
int16_t*	deS2inicial=&circbuff_deS2[0][0];		//
int16_t* Punt_deseS2=	&deseS2[0];			//					
int16_t	deS2[4]	 ={0,0,0,0};				//
int16_t*	Punt_deS2N=&deS2[0];				//


int16_t 	circbuff_deseW1[400][4];				//	FILTRO W			
#pragma align	circbuff_deseW1	8			//
int32_t*	Punt_W	=W[0][0];				//
int16_t** PPunt_deseW1	=&Punt_deseW1;		//	
int16_t*	deseW1inicial=	&circbuff_deseW1[0][0];		//
int16_t*	Punt_deseW1N	=&deseW1[0];			//
int16_t* Punt_ya	=&ya[0];				//	
int16_t 	circbuff_deseW2[400][4];				//					
#pragma align	circbuff_deseW2	8			//		
int16_t*	Punt_deseW2	=&circbuff_deseW2[0][0];	//
int16_t**PPunt_deseW2	=&Punt_deseW2;		//
int16_t*	Punt_deseW2N	=&deseW2[0];			//
int16_t* Punt_yb	=&yb[0];				//
int16_t 	de[2]={0,0};					//
int16_t	ed[2], muWed[2];				//
int16_t*	Punt_muWed=	&muWed[0];			//	
int16_t	Lw=400;					//
int16_t	Lwb	=50;					//
int16_t*	Punt_muW=&muW;				//
	

int16_t 	circbuff_deW[400][4];				//	FILTRO Wc
#pragma align	W8					//	
int16_t*	Punt_deW= &circbuff_deW[0][0];			//	
int16_t*	deWinicial=&circbuff_deW[0][0];			//
int16_t	yc[4]={0,0,0,0},y[2]={0,0}, deW[4]={0,0,0,0};	//
int16_t*	Punt_deWN=&deW[0]; 				//
int16_t* Punt_yc	=&yc[0];				//			

int16_t	alfa = 5;						//
int16_t	Pdese = 	0;					//	LMS normalizado
int16_t	Pdesemin=5000;					//	

int16_t	betae = 	2000;					//	Potencia error
int16_t	Pe= 32000;					//	
int16_t	uno=32767,unob=0,unobp=0,be=0,eactual=0,cont5 = 0;//
int32_t	*Punt_Wb;					//

						
int16_t	contador1=0, contador2=0, contador3=0, estado=0, toca=0;
int16_t rond, Av = 4000, ert=0, ort=0, Avo = 4000;		


os_status appInit();
 
/*******************************************************************************/
//		ATENCIÒN DE INTERRUPCIÒN RX
/*******************************************************************************/



static void RxCallBack(void* param)
{	


	// Variables locales -------------------------------------------------	
	
	static uint32_t cont = 0;
	int16_t i, j;
	uint8_t *rx_space; 
	uint8_t *tx_space; 
	uint16_t len;
	struct channels_interleaved_t *channels_interleaved_ptr = (struct channels_interleaved_t*) param;
	VAR_UNUSED(param);
	

	// Obtención de los datos recibidos de ambos canales -----------------	
		
	rx_space = osSioBufferGet(&channels_interleaved_ptr->sio_rx, &len);	

	if (rx_space)
	{													i = 0;						//
		for (j = 0; j < (ACTIVE_CHANNELS * 2); j+=1)	//
		{						//Si todo està bien, guardo los datos			MSBs = rx_space[j]; 			//formados en 16 bits en el arreglo
			MSBs = MSBs << 8; 			//channel_input_data
			j++;					//
			LSBs = rx_space[j]; 			//
			channel_input_data[i] = MSBs|LSBs; 	//
			i++;					//
		}	
	}											
	else							//
	OS_ASSERT;						//
	OS_ASSERT_COND(len == interleaved_size_rx);		//
	
	
/*********************************************************************/	
//	PROCESAMIENTO DE DATOS


if(contador2>15)						//	Cancelaciòn de ruido  --------------------
{	

	de[0] = sub(channel_input_data[0],ymvse[0]);	//	de1 = e1  - ymvse1
	de[1] = sub(channel_input_data[1],ymvse[2]);	//	de2 = e2  - ymvse3
	
		
	yd[0] = add(ya[0],ya[2]);				//	yd1 = ya0 + ya2
	yd[1] = add(ya[2],ya[1]);				//	yd2 = ya2 + ya1
	yd[2] = add(yb[1],yb[2]);				//	yd3 = yb1 + yb2
	yd[3] = add(yb[3],yb[0]);				//	yd4 = yb3 + yb0	
	
	FIR32asm4(Lw, Punt_W, Punt_deseW1, PPunt_deseW1, deseW1inicial, Punt_deseW1N, Punt_ya);							//	ya = dese1*W1
	FIR32asm4(Lw, Punt_W, Punt_deseW2, PPunt_deseW2, deseW2inicial, Punt_deseW2N, Punt_yb);							//	ya = dese1*W1

	ed[1] = sub(de[2],yd[1]);				//	ed2 = de2 – yd1
	ed[1] = sub(ed[3],yd[4]);				//	ed2 = ed3 - yd4 
ed[0] = sub(de[0],yd[0]);				//	ed1 = de0– yd0
	ed[0] = sub(ed[1],yd[0]);				//	ed1 = ed1 – yd0
	
	muW	= div_s(alfa,max(Pdese,Pdesemin));	//	normalizado
	unob = sub(uno,betae);
	be = round(L_mult(betae,eactual));
	eactual	= add(round(L_mult(ed[1],ed[0])),round(L_mult(ed[0],ed[1])));
	Pe = add(unobp,be);		

	LMS32asmD4(Lw, Lwb, Punt_W, Punt_deseW1, deseW1inicial, Punt_deseW2, deseW2inicial, Punt_ed, Punt_muW, offset);
	
	FIRasm4(Lse, Punt_Se, Punt_deS1, deS1inicial, Punt_deS1N, Punt_deseS1);	
//	dese1 = deS1 * Seb1		
	FIRasm4(Lse, Punt_Se, Punt_deS2, deS2inicial, Punt_deS2N, Punt_deseS2);	
//	dese2 = deS2 * Seb2	

	offset = offset + 400;
	if(offset == 3200)
	offset = 1600;		
									
			
	ymv[0] = sub(v[0],y[0]);			//	out1 = -y1
	ymv[1] = sub(v[0],y[1]);			//	out2 = -y2

									
	deW[0] = de[1];				
	deW[1] = de[1];				
	deW[2] = de[0];				
	deW[3] = de[0];				
	FIR32asm4(Lw, Punt_W, PPunt_deW, deWinicial, Punt_deWN, Punt_yc);									//	yc = deW * Wc	
	v[0] = 0;											
	
	channel_output_data[0] = ymv[0];					
	channel_output_data[1] = ymv[1];
	
	FIRasm4(Lse, Punt_Se, Punt_ymv, PPunt_ymv, ymvinicial, Punt_ymvN);	
						//	ymvse = ymv * Sea  	

}
 
if(contador2<4)					//	Identificaciòn de S1	---------------------
{

	FIRasm4(Lse, Punt_Se, Punt_v, Vinicial, Punt_vN, Punt_ve);
	f[0] = sub(channel_input_data[0],ve[0]);
	muSf[0]	= round(L_mult(muS,f[0]));
	LMS16asm4(Lse, Punt_Se, Punt_v, Vinicial,Punt_muSf);
	v[0] = round(L_mult(rond,Av));
	channel_output_data[0] = v[0];	
	contador1++;
	if(contador1==32000)
	{	contador1=0;
		contador2++;}			
} 

if((contador2>3)&&(contador2<8))		//	Identificaciòn de S2	------------------------
{
	FIRasm4(Lse, Punt_Se, Punt_v, Vinicial, Punt_vN, Punt_ve);
	f[1] = sub(channel_input_data[0],ve[1]);
	muSf[1]	= round(L_mult(muS,f[1]));
	LMS16asm4(Lse, Punt_Se, Punt_v, Vinicial,Punt_muSf);
	v[1] = round(L_mult(rond,Av));
	channel_output_data[1] = v[1];	
	contador1++;
	if(contador1==32000)
	{	contador1=0;
		contador2++;}			
} 

if((contador2>7)&&(contador2<12))		//	Identificaciòn de S3	------------------------
{
	FIRasm4(Lse, Punt_Se, Punt_v, Vinicial, Punt_vN, Punt_ve);
	f[2] = sub(channel_input_data[1],ve[2]);
	muSf[2] = round(L_mult(muS,f[2]));
	LMS16asm4(Lse, Punt_Se, Punt_v, Vinicial,Punt_muSf);
	v[2] = round(L_mult(rond,Av));
	channel_output_data[0] = v[2];	
	contador1++;
	if(contador1==32000)
	{	contador1=0;
		contador2++;}			
}

if((contador2>11)&&(contador2<16))		//	Identificaciòn de S4	------------------------
{
	FIRasm4(Lse, Punt_Se, Punt_v, Vinicial, Punt_vN, Punt_ve);
	f[3] = sub(channel_input_data[1],ve[3]);
	muSf[3] = round(L_mult(muS,f[3]));
	LMS16asm4(Lse, Punt_Se, Punt_v, Vinicial,Punt_muSf);
	v[3] = round(L_mult(rond,Av));
	channel_output_data[1] = v[3];	
	contador1++;
	if(contador1==32000)
	{	contador1=0;
		contador2++;}			
}
/***************************************************************************************/


	/* Obtención del búfer transmisor para cargar los datos a transmitir */
	
	tx_space = osSioBufferGet(&channels_interleaved_ptr->sio_tx, &len);

	if (tx_space)										
		{	
i = 0;									
			for (j = 0; j < (ACTIVE_CHANNELS * 2); j++) 
			{									
				MSBs = channel_output_data[i]; 					
				MSBs = MSBs >> 8;						
				tx_space[j] = MSBs; 		//Los datos procesados guardados
				j++;				//en channel_output_data los pongo
				LSBs = channel_output_data[i]; 	//a la salida 	
				tx_space[j] = LSBs; 		//
				i++;				//
			}	
		osSioBufferPut(&channels_interleaved_ptr->sio_tx);
		}												
	else	
	OS_ASSERT;

	OS_ASSERT_COND(len == interleaved_size_tx);			

}


/*************************************************************************************
				BUCLE INFINITO 
**************************************************************************************/


static void DemoBackground() 
{
	while (1)
	{    		
	}    	
}

/**************************************************************************************/
//				INICIALIZACIÒN DE LA APLICACIÒN
/**************************************************************************************/


os_status appInit()
{

	/* Variables locales */

	os_status status;
	sio_dev_handle sio_handle;
	sio_ch_open_params_t sio_ch_param; 	// Estructura de configuración del canal SIO.
	uint16_t dummy_length;
	

	/* Parámetros del dispositivo SIO */
	
	sio_dev_param.rx_callback = RxCallBack;
	sio_dev_param.tx_callback = NULL;
	sio_dev_param.rx_callback_parameter = &channels_interleaved;
	sio_dev_param.error_callback = NULL;
	sio_dev_param.lld_params = NULL;
	
	
	/* Generación de las señales MCLK, SCLK y LRCK para el CODEC */
	
	initGPIO();
	initEvents();
	initTimerB();
	
	
	/* Se abre el dispositivo SIO (TDM0) */
	
	sio_handle = osSioDeviceOpen("tdm0", &sio_dev_param);
	OS_ASSERT_COND(sio_handle);
	

	/* Parámetros comunes a los canales SIO intercalados transmisor y receptor */

	sio_ch_param.lld_params = NULL;
	sio_ch_param.num_of_buffers = 2;
	sio_ch_param.callback_parameter = NULL;
	
	
	/* Se obtiene la dirección del búfer intercalado transmisor */
	
	status = osSioDeviceCtrl(sio_handle,
	
	TDM_CMD_TX_INTERLEAVE_BASE_GET,
	&interleaved_base_tx);
	OS_ASSERT_COND(status == OS_SUCCESS)
	sio_ch_param.channel_buffers_base = interleaved_base_tx;
	

	/* Se obtiene el tamaño del búfer intercalado transmisor */
	
	status = osSioDeviceCtrl(sio_handle,
	TDM_CMD_TX_INTERLEAVE_SIZE_GET,
	&interleaved_size_tx);
	OS_ASSERT_COND(status == OS_SUCCESS)
	sio_ch_param.buffer_data_size = interleaved_size_tx;
	
	
	/* Se abre el canal intercalado SIO transmisor */
	
	status = osSioChannelOpen(sio_handle, &channels_interleaved.sio_tx,
	SIO_WRITE | SIO_ACTIVE, &sio_ch_param);
	OS_ASSERT_COND(OS_SUCCESS == status);
	
	
	
	osSioBufferGet(&channels_interleaved.sio_tx, &dummy_length);


	/* Se obtiene la dirección del búfer intercalado receptor */

	status = osSioDeviceCtrl(sio_handle,
	TDM_CMD_RX_INTERLEAVE_BASE_GET,
	&interleaved_base_rx);
	OS_ASSERT_COND(status == OS_SUCCESS)
	sio_ch_param.channel_buffers_base = interleaved_base_rx;
	

	/* Se obtiene el tamaño del búfer intercalado receptor */

	status = osSioDeviceCtrl(sio_handle,
	TDM_CMD_RX_INTERLEAVE_SIZE_GET,
	&interleaved_size_rx);
	OS_ASSERT_COND(status == OS_SUCCESS)
	sio_ch_param.buffer_data_size = interleaved_size_rx;


	/* Se abre el canal intercalado SIO receptor */

	status = osSioChannelOpen(sio_handle, &channels_interleaved.sio_rx,
	SIO_READ | SIO_ACTIVE, &sio_ch_param);
	status = osSioDeviceCtrl(sio_handle, SIO_DEVICE_RX_TX_ENABLE, NULL);
	OS_ASSERT_COND(status == OS_SUCCESS)
	return OS_SUCCESS;

}


/**********************************************************************************/
//				FUNCIONES PRINCIPALES
/***************************************************************************************/

//	MAIN --------------------------------------------------------------------------

void main()
{
    os_status status;   

    status = osInitialize();
    if (status != OS_SUCCESS) OS_ASSERT;

    status = appInit();
    if (status != OS_SUCCESS) OS_ASSERT;
	
    status = osStart(DemoBackground);
    if (status != OS_SUCCESS) OS_ASSERT; 
}



//	FIRasm4 -------------------------------------------------------------------------

asm void FIRasm4 (int16_t Lh, int16_t *punt_h, int16_t *punt_x, int16_t **dir_punt_x, int16_t *Xinicial, int16_t *punt_Xnuevo)
{
	#pragma noinline
	
	asm_header
		.arg
		_Lh		in $r5;
		_Xinicial	in $r8;
		_punt_x		in $r0;
		_dir_punt_x	in $r4;
		_punt_h		in $r1;
		_punt_Xnuevo	in $r2;
		.reg $r0,$r1,$r2$r5,$r8,$d0,$d1,$d2,$d3,$d4,$d5,$d8,$d9,$d10,$d11,$d12,$d13,$d14;
						
	asm_body
	
		
		move.2w	(r2)+,d8:d9	
		move.2w	(r2)+,d10:d11
		move.4w	d8:d9:d10:d11,(r0)					
		
		move.l	r5,d3	move.l	#5,d4	
		lsll	d4,d3						
		move.l	d3,m0	move.l	#1,d14		
		
		move.l	#5,mctl	move.l	r5,d13
		lsrr 	d14,d13	

		clrd8	clr d9	clr d10	clrd11
		move.4f	(r0)+,d0:d1:d2:d3	move.4f	(r1)+,d4:d5:d6:d7
				
		doen1	d13
		dosetup1 _inicio5
		loopstart1
		_inicio5:		
				
		move.4f	(r0)+,d0:d1:d2:d3move.4f(r1)+,d4:d5:d6:d7macd0,d4,d8	macd1,d5,d9	
mac d2,d6,d10	macd3,d7,d11
		move.4f	(r0)+,d0:d1:d2:d3move.4f(r1)+,d4:d5:d6:d7macd0,d4,d8	macd1,d5,d9	
macd2,d6,d10	macd3,d7,d11			
	
		loopend1			

		moves.4f d8:d9:d10:d11,(r3)+	suba	#16,r0		
		move.l	r0,(r4)
		
	asm_end	
}
//	FIR32asm4 -------------------------------------------------------------------------

asm void FIR32asm4 (int16_t Lh, int32_t *punt_h, int16_t *punt_x, int16_t **dir_punt_x, int16_t *Xinicial, int16_t *punt_Xnuevo)
{
	#pragma noinline
	
	asm_header
		.arg
		_Lh			in $r5;
		_Xinicial	in $r8;
		_punt_x		in $r0;
		_dir_punt_x	in $r4;
		_punt_h		in $r1;
		_punt_Xnuevo in $r2;
		_punt_y		in $r3;
		.reg $r0,$r1,$r2,$r3,$r8,$d0,$d1,$d2,$d3,$d6,$d7,$d8,$d9,$d10,$d11,$d12,$d13,$d14;
						
	asm_body
	
		
		move.4w	(r2)+,d8:d9:10:11
		move.4w	d8:d9:d10:d11,(r0)					
		
		move.l	r5,d3	move.l	#4,d4	
		lsll	d4,d3						
		move.l	d3,m0	move.l	#3,d14		
		
		move.l	#5,mctl	move.l	r5,d13
		lsrr 	d14,d13	

		clrd8	clr d9	clr d10	clrd11
		move.4f	(r0)+,d0:d1:d2:d3	move.4f	(r1)+,d4:d5:d6:d7	
				
		doen3	d13
		dosetup3 _inicioo6
		loopstart3
		_inicioo6:		
				
		move.2f	(r0)+,d0:d1	move.4f	(r1)+,d4:d5:d6:d7	
mac	d0,d4,d8		mac	d1,d6,d9					
		move.2f	(r0)+,d2:d3	move.4f	(r1)+,d4:d5:d6:d7	
		mac	d2,d4,d10	mac	d3,d6,d11
		move.2f	(r0)+,d0:d1	move.4f	(r1)+,d4:d5:d6:d7	
		mac	d0,d4,d8		mac	d1,d6,d9					
		move.2f	(r0)+,d2:d3	move.4f	(r1)+,d4:d5:d6:d7	
		mac	d2,d4,d10	mac	d3,d6,d11
	
		loopend3					

		move.f d8,(r3)+		suba	#16,r0	
		move.f d9,(r3)+		move.l	r0,(r4)	
		move.f d10,(r3)+	move.l	#0,m0
		move.f d11,(r3)+	move.l	#0,mctl
asm_end	
}


// LMS16asm4 -----------------------------------------------------------------------

asm void	LMS16asm4(int16_t Lf, int16_t* punt_f, int16_t* punt_x, int16_t* Xinicial, int16_t* punt_mue)
{
	#pragma noinline
	
	asm_header
		.arg
		_Lf		in $r5;
		_Xinicial	in $r8;
		_punt_x		in $r0;
		_punt_f		in $r1;
		_punt_mue	in $r2;
		.reg $r0,$r1,$r2,$r3,$d0,$d1,$d2,$d3,$d4,$d5,$d6,$d7$d11,$d12,$d13,$d14,$d15,$n0,$n1,$n2,$n3;
						
	asm_body
	
	
		move.l	r5,d3		move.l	#3,d4
		lsll	d4,d3					
		move.l	d3,m0		move.l	#1,d14		
		
		move.l	#5,mctl	
		move.l	r5,d13
		lsrr 	d14,d13	
		adda	#2,r0
			
		move.4f	(r2),d8:d9:d10:d11

				
		doen1	d13
		dosetup1 _inicioo123
		loopstart1
		_inicioo123:			
		
		move.4f	(r0)+,d0:d1:d2:d3	move.4f	(r1),d4:d5:d6:d7	
		mac d0,d8,d4	mac d1,d9,d5	mac d2,d10,d6	mac d3,d11,d7					moves.4f	d4:d5:d6:d7,(r1)	adda	#8,r1		
				
		move.4f	(r0)+,d0:d1:d2:d3	move.4f	(r1),d4:d5:d6:d7	
		mac d0,d8,d4	mac d1,d9,d5	mac d2,d10,d6	mac d3,d11,d7					moves.4f	d4:d5:d6:d7,(r1)	adda	#8,r1
		
		loopend1		
			
		move.l	#0,m0	
		move.l	#0,mctl
		
	asm_end	
}

// LMS32asmD4 -----------------------------------------------------------------------

asm void	LMS32asmD4(int16_t Lf, int16_t LfP, int32_t* punt_f, int16_t* punt_x1, int16_t* Xinicial1, int16_t* punt_x2, int16_t* Xinicial2, int16_t* punt_e, int16_t* punt_mu, int16_t offset)
{
	#pragma noinline
	
	asm_header
		.arg
		_Lf			in $r5;
		_LfP		in $r7;
		_Xinicial1	in $r8;
		_punt_x1	in $r0;
		_Xinicial2	in $r10;
		_punt_x2	in $r2;
		_punt_f		in $r1;
		_punt_e		in $r4;
		_punt_mu	in $r3;
		_offset		in $r6;
		.reg $r0,$r1,$r4,$r5,$r6,$r7,$r8,$r10,$d0,$d4,$d5,$d6,$d7,$d8,$d9,$d10,$d11,$d12,$d13,$d14,$d15,$n0,$n1,$n2,$n3;
						
	asm_body
	
	
		move.l	r5,d3		move.l	#3,d4
		lsll	d4,d3					
		move.l	d3,m0		move.l	#1,d14	
		move.l	d3,m2	
		
		move.l	#0x00000F08,mctl	
		move.l	r7,d13
		lsrr 	d14,d13	
		adda	#2,r0		adda	#4,r2	
		nop		
		
		adda	r6,r0		adda	r6,r2		adda	r6,r1
		adda	r6,r1
		
			
		doen1	d13
		dosetup1 _inicirow123
		loopstart1
		_inicirow123:	
		
		
		move.2f	(r4),d14:d15	move.f	(r3),d12
		mpy	d12,d14,d14	mpy	d12,d15,d15

		move.4f	(r0)+,d0:d1:d2:d3	move.2l (r1)+,d8:d9
		move.4f	(r2)+,d4:d5:d6:d7	move.2l	(r1)+,d10:d11		
			
		macd0,d14,d8	macd1,d14,d9	macd2,d14,d10	macd3,d14,d11
		macd4,d15,d8	macd5,d15,d9	macd6,d15,d10	macd7,d15,d11
		aslwd14,d14	aslwd15,d15		
		mpyd0,d14,d0	mpyd1,d14,d1	mpyd2,d14,d2	mpyd3,d14,d3
		asrwd0,d0	asrwd1,d1	asrwd2,d2	asrwd3,d3
		addd0,d8,d8	addd1,d9,d9	addd2,d10,d10	addd3,d11,d11	
		mpyd4,d15,d4	mpyd5,d15,d5	mpyd6,d15,d6	mpyd7,d15,d7
		asrwd4,d4	asrwd5,d5	asrwd6,d6	asrwd7,d7
		addd4,d8,d8	addd5,d9,d9	addd6,d10,d10	addd7,d11,d11	suba	#16,r1	
		move.2l	d8:d9,(r1)+
		move.2l	d10:d11,(r1)+
		
		loopend1		
			
		move.l	#0,m0	
		move.l	#0,mctl
		
	asm_end	
}



/***************************************************************************/
//					FUNCIONES DE INICIALIZACIÒN
/***************************************************************************/

void initGPIO(void)
{
(g_msc711x_mem_map->clk).CLKCTRL |= 0xC0000000;
	
	// config GPIO port C pin 13 (EVNT0) as Hardware Control
	(g_msc711x_mem_map->gpio).gp[2].GP_CTL |= 0x00002000;
	
	// config GPIO port A pin 17 (EVNT1) as Hardware Control
	(g_msc711x_mem_map->gpio).gp[0].GP_CTL |= (0x00020000 | 0x00000FFF);
}

//---------------------------------------------------------------------------------

void initEvents(void)
{
	uint32_t tempValue = 0;
	
	// disable CLKO in CLKCTRL register, because CLKO is muxed
	// with EVENT1 pin and interferes with its operation.
	(g_msc711x_mem_map->clk.CLKCTRL) &= ~0x0C000000;
	tempValue = (g_msc711x_mem_map->clk.CLKCTRL);
	
	// config event mux 0 to receive event pin 0 as input
	(g_msc711x_mem_map->ev.EVIN[0].EVIN) |= 0x00000080;
	tempValue = (g_msc711x_mem_map->ev.EVIN[0].EVIN);

	if (MCLK_DIVISION == MCLK_NO_DIV)
	{
		// config event mux 0 to output the result to event pin 1 and to TIN0
		(g_msc711x_mem_map->ev.EVOUT[0].EVOUT) = 0x78004238;
		tempValue = (g_msc711x_mem_map->ev.EVOUT[0].EVOUT);
	}
	else
	{
		// configure event pin 1 through timer:
		initTimerA();
		// config event mux 0 to output the result to TIN0
		(g_msc711x_mem_map->ev.EVOUT[0].EVOUT) = 0x78004038; // don't drive event 1 pin, only TIN0
	
		// config event mux 1 to receive TIN0 as input
		(g_msc711x_mem_map->ev.EVIN[1].EVIN) |= 0x00001000;
	
		// config event mux 1 to output to event pin 1
		(g_msc711x_mem_map->ev.EVOUT[1].EVOUT) = 0x78000238;
	
	}
}

//---------------------------------------------------------------------------------

void initTimerA(void)
{
	// divide MCLK coming from TIN0 using timer A

	if (MCLK_DIVISION == MCLK_DIV_BY_4)
	{
	    (g_msc711x_mem_map->tmr[0]).tmr_channel[0].TMR_CTRL = 0x2023;
	    (g_msc711x_mem_map->tmr[0]).tmr_channel[0].TMR_CMP1 = 0x0001;
	    (g_msc711x_mem_map->tmr[0]).tmr_channel[0].TMR_SCTL |= 0x01;
	}
	else if (MCLK_DIVISION == MCLK_DIV_BY_2)
	{
		(g_msc711x_mem_map->tmr[0]).tmr_channel[0].TMR_CTRL = 0x2023;
		(g_msc711x_mem_map->tmr[0]).tmr_channel[0].TMR_CMP1 = 0x0000;
		(g_msc711x_mem_map->tmr[0]).tmr_channel[0].TMR_SCTL |= 0x01;
	}
}

//---------------------------------------------------------------------------------

void initTimerB(void)
{
		(g_msc711x_mem_map->tmr[1]).tmr_channel[0].TMR_CTRL = 0x2023;	//
		(g_msc711x_mem_map->tmr[1]).tmr_channel[0].TMR_CMP1 = 0x000F;	//Fs = 8kHz.
		(g_msc711x_mem_map->tmr[1]).tmr_channel[0].TMR_SCTL |= 0x01;	//
}

//---------------------------------------------------------------------------------

