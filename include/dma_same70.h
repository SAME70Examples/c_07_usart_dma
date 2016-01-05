#ifndef _DMA_SAME70_H
#define _DMA_SAME70_H

void dma_init(void);
void dma_xfer(void * source, void * destiny, int size);
void dma_usart1_xfer(char* source, int size);

#endif// _DMA_SAME70_H
