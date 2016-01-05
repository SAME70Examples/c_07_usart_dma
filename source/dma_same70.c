#include "dma_same70.h"
#include "same70.h"                     // Device header
#include "extra_defines_same70.h"

static volatile int dma_xfer_in_progress;

void dma_init(void){
	PMC->PMC_PCR = (PMC_PCR_EN)|(PMC_PCR_CMD_WRITE)|(PMC_PCR_PID_XDMAC);//Enable XDMAC Peripheral
	NVIC_EnableIRQ(XDMAC_IRQn);
}

void dma_xfer(void * source, void * destiny, int size){
	int dumy = XDMAC->XDMAC_CHID[0].XDMAC_CIS;//read the CIS to clear pending interrupts
	XDMAC->XDMAC_CHID[0].XDMAC_CSA = (uint32_t)source;
	XDMAC->XDMAC_CHID[0].XDMAC_CDA = (uint32_t)destiny;
	XDMAC->XDMAC_CHID[0].XDMAC_CUBC = size;
	XDMAC->XDMAC_CHID[0].XDMAC_CC = (XDMAC_CC_TYPE_MEM_TRAN | XDMAC_CC_MBSIZE_FOUR
	| XDMAC_CC_DAM_INCREMENTED_AM | XDMAC_CC_SAM_INCREMENTED_AM| XDMAC_CC_DWIDTH_BYTE
	| XDMAC_CC_SIF_AHB_IF0 | XDMAC_CC_DIF_AHB_IF1 | XDMAC_CC_SWREQ_SWR_CONNECTED);
	XDMAC->XDMAC_CHID[0].XDMAC_CNDC = 0;
	XDMAC->XDMAC_CHID[0].XDMAC_CBC = 0;
	XDMAC->XDMAC_CHID[0].XDMAC_CDS_MSP = 0;
	XDMAC->XDMAC_CHID[0].XDMAC_CSUS = 0;
	XDMAC->XDMAC_CHID[0].XDMAC_CDUS = 0;

	dma_xfer_in_progress = 1;

	XDMAC->XDMAC_CHID[0].XDMAC_CIE =  XDMAC_CIE_BIE;
	XDMAC->XDMAC_GIE = XDMAC_GIE_IE0;
	
	XDMAC->XDMAC_GE = XDMAC_GE_EN0;
	XDMAC->XDMAC_GSWR = XDMAC_GSWR_SWREQ0;
	while(dma_xfer_in_progress);
}

void dma_usart1_xfer(char* source, int size){
	int dumy = XDMAC->XDMAC_CHID[0].XDMAC_CIS;//read the CIS to clear pending interrupts
	XDMAC->XDMAC_CHID[0].XDMAC_CSA = (uint32_t)source;
	XDMAC->XDMAC_CHID[0].XDMAC_CDA = (uint32_t) (&(USART1->US_THR));
	XDMAC->XDMAC_CHID[0].XDMAC_CUBC = size;//Number of data in ublock
	XDMAC->XDMAC_CHID[0].XDMAC_CC = (XDMAC_CC_PERID(9))|(XDMAC_CC_TYPE_PER_TRAN | XDMAC_CC_MBSIZE_EIGHT
	| XDMAC_CC_SAM_INCREMENTED_AM | XDMAC_CC_DAM_UBS_DS_AM | XDMAC_CC_DWIDTH_BYTE
	| XDMAC_CC_SIF_AHB_IF0 | XDMAC_CC_DIF_AHB_IF1 | XDMAC_CC_SWREQ_HWR_CONNECTED);
	XDMAC->XDMAC_CHID[0].XDMAC_CBC = 0;//Number of ublocks in block
	XDMAC->XDMAC_CHID[0].XDMAC_CNDC = 0;
	XDMAC->XDMAC_CHID[0].XDMAC_CDS_MSP = (( 0 << XDMAC_CDS_MSP_SDS_MSP_Pos) & XDMAC_CDS_MSP_SDS_MSP_Msk)//
	| ((((uint32_t) -1) << XDMAC_CDS_MSP_DDS_MSP_Pos) & XDMAC_CDS_MSP_DDS_MSP_Msk);//
	XDMAC->XDMAC_CHID[0].XDMAC_CSUS = 0;
	XDMAC->XDMAC_CHID[0].XDMAC_CDUS = 0;

	dma_xfer_in_progress = 1;

	XDMAC->XDMAC_CHID[0].XDMAC_CIE =  XDMAC_CIE_BIE;
	XDMAC->XDMAC_GIE = XDMAC_GIE_IE0;
	
	XDMAC->XDMAC_GE = XDMAC_GE_EN0;
	while(dma_xfer_in_progress);
}

void XDMAC_Handler(void){
	dma_xfer_in_progress = 0;
	int dumy = XDMAC->XDMAC_CHID[0].XDMAC_CIS;//read the CIS to clear pending interrupts
	XDMAC->XDMAC_GID = XDMAC_GID_ID0;
}
