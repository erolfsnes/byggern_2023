#ifndef SRAM_H_
#define SRAM_H_

void SRAM_Test(void);
void SRAM_Init(void);
void SRAM_Write(uint8_t data, uint16_t addr);
uint8_t SRAM_Read (uint16_t addr);

#endif