#ifndef SRAM_H_
#define SRAM_H_

void SRAM_test(void);
void SRAM_init(void);
void SRAM_write(uint8_t data, uint16_t addr);
uint8_t SRAM_read (uint16_t addr);

#endif