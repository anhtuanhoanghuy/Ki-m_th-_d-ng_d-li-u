#include "MKL46Z4.h"
#include "LED.h"
#include "switch.h"
#include "utilities.h"
#include "Seg_LCD.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

volatile uint8_t status = 0x00; //Use this variable to read status of Switch 1 (bit 0), Switch 2 (bit 1), delay_status (bit 2)
int count = 0;

int main(void) {
	init_LED();
	init_switch();
	SegLCD_Init();
	init_SysTick_interupt();
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	delay(5);
	while(1) {
	if ((status & 0x01)== 0) { //chua ngoi tren xe
			green_LED_OFF();
			SegLCD_Clear();
			SegLCD_char('O',2); //OFF
			SegLCD_char('F',3);
			SegLCD_char('F',4);
			status &= ~(1<<2);
			count = 0;
			if((status & 0x02)==0) { //chua ngoi tren xe va chua that day an toan
				red_LED_ON();
			} else { //chua ngoi tren xe ma da that day an toan 
				red_LED_OFF();
			}
		} else {	//Switch 1 nhan -> da ngoi tren xe 00000001
				if((status & 0x04) == 0) {
					SegLCD_Clear();
					SegLCD_char('O',2); //ON
					SegLCD_char('N',3);
					delay(1);
					count++;
					if (count < 10) {
						 if ((status & 0x02) != 0) { //Switch 1 nhan & switch 3 nhan
									red_LED_OFF();
									green_LED_ON();
									SegLCD_Clear();
									SegLCD_char('P',1); //PASS
									SegLCD_char('A',2);
									SegLCD_char('S',3);
									SegLCD_char('S',4);
									status |= (1<<2);
									count = 0;
						 }
					} else {
						 status |= (1<<2);    
						 count = 0;             
						}
				} else {
						if ((status & 0x02) == 0) { //Switch 1 nhan & switch 3 khong nhan
							SegLCD_Clear();
							SegLCD_char('O',2); //ON
							SegLCD_char('N',3);
							green_LED_OFF();
							red_LED_ON();
							delay(1);
							red_LED_OFF();
							delay(1);
						} else {//Switch 1 nhan & switch 3 nhan
							red_LED_OFF();
							green_LED_ON();
							SegLCD_Clear();
							SegLCD_char('P',1); //PASS
							SegLCD_char('A',2);
							SegLCD_char('S',3);
							SegLCD_char('S',4);
						}
				}
			}
		}
}