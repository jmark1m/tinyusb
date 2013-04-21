/*
 * board_ea4357.c
 *
 *  Created on: Jan 17, 2013
 *      Author: hathach
 */

/*
 * Software License Agreement (BSD License)
 * Copyright (c) 2013, hathach (tinyusb.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the tiny usb stack.
 */

#include "board.h"

#if BOARD == BOARD_EA4357

#include "common/assertion.h" // TODO there is hal_debugger_ in assertion

#define UART_PORT   LPC_USART0

#if 0
static const struct {
  uint8_t port;
  uint8_t pin;
}leds[CFG_LED_NUMBER] = { {0, 8} };
#endif

// MIC2555 1YML = 0101111, 0YML = 0101101
#define MIC255_ADDR BIN8(0101111)

//static uint8_t mic255_regs_read(uint8_t regs_addr)
//{
//  uint8_t value;
//
//  ASSERT( SUCCESS == I2C_MasterTransferData(
//      LPC_I2C0,
//      & (I2C_M_SETUP_Type)
//      {
//        .sl_addr7bit         = MIC255_ADDR,
//        .retransmissions_max = 3,
//
//        .tx_data             = &regs_addr,
//        .tx_length           = 1,
//
//        .rx_data             = &value,
//        .rx_length           = 1
//      },
//      I2C_TRANSFER_POLLING), 0xFF);
//
//  return value;
//}

//static bool mic255_regs_write(uint8_t regs_addr, uint8_t data)
//{
//  uint8_t xfer_data[2] = { regs_addr, data} ;
//
//  ASSERT( SUCCESS == I2C_MasterTransferData(
//      LPC_I2C0,
//      & (I2C_M_SETUP_Type)
//      {
//        .sl_addr7bit         = MIC255_ADDR,
//        .retransmissions_max = 3,
//
//        .tx_data             = xfer_data,
//        .tx_length           = 2,
//      },
//      I2C_TRANSFER_POLLING), false);
//
//  return true;
//}


//static uint16_t mic255_get_vendorid(void)
//{
//  uint8_t vendor_low  = mic255_regs_read(0);
//  uint8_t vendor_high = mic255_regs_read(1);
//
//  return (vendor_high << 8) | vendor_low;
//}

void board_init(void)
{
  SystemInit();
  CGU_Init();

  SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / CFG_TICKS_PER_SECOND); // 1 msec tick timer

  // USB0 Power: EA4357 channel B U20 GPIO26 active low (base board), P2_3 on LPC4357
  scu_pinmux(0x2, 3, MD_PUP | MD_EZI, FUNC7);		// USB0 VBus Power
  
  // USB1 Power: EA4357 channel A U20 is enabled by SJ5 connected to pad 1-2, no more action required
  scu_pinmux(0x2, 5, MD_PLN | MD_EZI | MD_ZI, FUNC2);	// USB1_VBUS monitor presence, must be high for bus reset occur

  // init I2C and set up MIC2555 to have 15k pull-down on USB1 D+ & D-
//  I2C_Init(LPC_I2C0, 100000);
//  I2C_Cmd(LPC_I2C0, ENABLE);
//
//  ASSERT_INT(0x058d, mic255_get_vendorid(), (void) 0); // verify vendor id
//  ASSERT( mic255_regs_write(6, BIN8(1100)), (void) 0); // pull down D+/D- for host

#if 0
  // Leds Init
	for (uint8_t i=0; i<CFG_LED_NUMBER; i++)
	{
	  scu_pinmux(leds[i].port, leds[i].pin, MD_PUP|MD_EZI|MD_ZI, 0); // MD_PDN
	  GPIO_SetDir(leds[i].port, BIT_(leds[i].pin), 1); // output
	}
#endif

#if CFG_UART_ENABLE
  //------------- UART init -------------//
	scu_pinmux(0xF ,10 , MD_PDN, FUNC1); 	              // PF.10 : UART0_TXD
	scu_pinmux(0xF ,11 , MD_PLN|MD_EZI|MD_ZI, FUNC1); 	// PF.11 : UART0_RXD

	UART_CFG_Type UARTConfigStruct;
  UART_ConfigStructInit(&UARTConfigStruct);
	UARTConfigStruct.Baud_rate = CFG_UART_BAUDRATE;
	UARTConfigStruct.Clock_Speed = 0;

	UART_Init(UART_PORT, &UARTConfigStruct);
	UART_TxCmd(UART_PORT, ENABLE); // Enable UART Transmit
#endif

#if CFG_PRINTF_TARGET == PRINTF_TARGET_SWO
#endif
}

//--------------------------------------------------------------------+
// LEDS
//--------------------------------------------------------------------+
void board_leds(uint32_t mask, uint32_t state) __attribute__ ((warning("not supported yet")));
void board_leds(uint32_t mask, uint32_t state)
{
#if 0
  uint8_t i;
  for(i=0; i<CFG_LED_NUMBER; i++)
  {
    if ( mask & BIT_(i) )
    {
      (mask & state) ? GPIO_SetValue(leds[i].port, BIT_(leds[i].pin)) : GPIO_ClearValue(leds[i].port, BIT_(leds[i].pin)) ;
    }
  }
#endif
}

//--------------------------------------------------------------------+
// UART
//--------------------------------------------------------------------+
#if CFG_UART_ENABLE
uint32_t board_uart_send(uint8_t *buffer, uint32_t length)
{
  return UART_Send(UART_PORT, buffer, length, BLOCKING);
}

uint32_t board_uart_recv(uint8_t *buffer, uint32_t length)
{
  return UART_Receive(UART_PORT, buffer, length, BLOCKING);
}
#endif


/******************************************************************************
 *
 * Description:
 *   Initialize the trim potentiometer, i.e. ADC connected to TrimPot on
 *   Base Board.
 *
 *****************************************************************************/
//void trimpot_init(void)
//{
//  // pinsel for AD0.3 on p7.5
//	scu_pinmux(	7	,	5	,	PDN_DISABLE | PUP_DISABLE	| INBUF_DISABLE,	0	);
//  LPC_SCU->ENAIO0 |= (1<<3);
//
//  ADC_Init(LPC_ADC0, 400000, 10);
//
//	ADC_IntConfig(LPC_ADC0, ADC_ADINTEN2, DISABLE);
//	ADC_ChannelCmd(LPC_ADC0, ADC_CH_TRIMPOT, ENABLE);
//}

#endif
