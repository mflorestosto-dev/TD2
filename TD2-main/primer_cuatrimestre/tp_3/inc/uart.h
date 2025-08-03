#ifndef __UART_H
#define __UART_H

#include <stddef.h>
#include <stdint.h>

/* The ARM PrimeCell UART (uart) is documented at
 *
 * http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183g/index.html
 * 
 * In particular, Section 3 explains the programmer's model, i.e., how to 
 * interact with it: this includes 
 * 
 * - Section 3.2, which summarises the device register layout in Table 3.1
 *   (including an offset from the device base address, in the memory map,
 *   for each register), and
 * - Section 3.3, which summarises the internal structure of each device
 *   register.
 * 
 * Note that the field identifiers used here follow the documentation in a
 * general sense, but with a some minor alterations to improve clarity and
 * consistency.
 */

#define UART0_ADDR 0x10009000
#define UART1_ADDR 0x1000A000
#define UART2_ADDR 0x1000B000
#define UART3_ADDR 0x1000C000

#define RSVD(x,y,z) uint8_t reserved##x[ z - y + 1 ];

typedef volatile struct {
  uint32_t    DR;            // base+0x0000          : data
union {
  uint32_t   RSR;            // base+0x0004          : receive status
  uint32_t   ECR;            //                      | error   clear
} ;
  RSVD( 0, 0x0008, 0x0017 ); // base+0x0008...0x0017 : reserved
  uint32_t    FR;            // base+0x0018          : flag
  RSVD( 1, 0x001C, 0x001F ); // base+0x001C...0x001F : reserved
  uint32_t   LPR;            // base+0x0020          : low-power counter
  uint32_t  IBRD;            // base+0x0024          : integer    baud rate
  uint32_t  FBRD;            // base+0x0028          : fractional baud rate
  uint32_t   LCR;            // base+0x002C          : line control 
  uint32_t    CR;            // base+0x0030          :      control
  uint32_t  IFLS;            // base+0x0034          : interrupt level select 
  uint32_t  IMSC;            // base+0x0038          : interrupt mask
  uint32_t   RIS;            // base+0x003C          : raw    interrupt status
  uint32_t   MIS;            // base+0x0040          : masked interrupt status
  uint32_t   ICR;            // base+0x0044          : interrupt clear
  uint32_t DMACR;            // base+0x0048          : DMA control
  RSVD( 2, 0x004C, 0x0FDF ); // base+0x004C...0x0FDF : reserved
  uint32_t PeriphID[ 4 ];    // base+0x0FE0...0x0FEC : peripheral ID
  uint32_t  PCellID[ 4 ];    // base+0x0FF0...0x0FFC : PrimeCell  ID
} _uart_t;

/* We know from
 * 
 * http://infocenter.arm.com/help/topic/com.arm.doc.dui0417d/index.html
 * 
 * that the platform has 4 uart devices(s), each of which we capture as an
 * instance of the _uart_t structure.
 */


extern _uart_t* __uart_init(uint8_t );
extern void uart_putc( _uart_t* , uint8_t );
extern uint8_t uart_getc( _uart_t* );
extern void uart_puth( _uart_t* , uint8_t );
extern uint8_t uart_geth( _uart_t* );
extern void print_f(char *);
extern void print_f_len(char *, uint16_t );

#endif