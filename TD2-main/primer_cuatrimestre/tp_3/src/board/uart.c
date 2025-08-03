// Revisar que el path sea correcto en su estructura de TP
#include "../../inc/uart.h"

_uart_t* __uart_init(uint8_t );
void uart_putc( _uart_t* , uint8_t );
uint8_t uart_getc( _uart_t* );
void uart_puth( _uart_t* , uint8_t );
uint8_t uart_geth( _uart_t* );
void print_f( char *);
void print_f_len(char *, uint16_t );

// Revisar la sección a la cual fueron asignadas las funciones a continuación
__attribute__((section(".kernel_text"))) _uart_t* __uart_init(uint8_t uart_num) {   
        _uart_t* UART = NULL;
        uint8_t required_uart_exists = 0, is_uart0 = 0;

        switch (uart_num)
            {
            case 0:
                UART = ( _uart_t* ) UART0_ADDR;
                required_uart_exists = 1;
                is_uart0 = 1;
                break;
            case 1:
                UART = ( _uart_t* ) UART1_ADDR;
                required_uart_exists = 1;
                break;
            case 2:
                UART = ( _uart_t* ) UART2_ADDR;
                required_uart_exists = 1;
                break;
            case 3:
                UART = ( _uart_t* ) UART3_ADDR;
                required_uart_exists = 1;
                break;
            default:
                required_uart_exists = 0;
                break;
            }

        if (required_uart_exists == 1)
            {
              if (is_uart0 == 0)
                {
                  UART->IMSC |= 0x00000010; // enable UART (Rx) interrupt only for UARTs 1, 2 and 3
                }
              UART->CR = 0x00000301; // enable UART (Tx+Rx)
            }

        return UART;
    }


__attribute__((section(".kernel_text"))) int  xtoi( char x ) {
    if      ( ( x >= '0' ) && ( x <= '9' ) ) {
      return (  0 + ( x - '0' ) );
    }
    else if ( ( x >= 'a' ) && ( x <= 'f' ) ) {
      return ( 10 + ( x - 'a' ) );
    }
    else if ( ( x >= 'A' ) && ( x <= 'F' ) ) {
      return ( 10 + ( x - 'A' ) );
    }

    return -1;
  }

__attribute__((section(".kernel_text"))) char itox( int  x ) {
    if ( ( x >=  0 ) && ( x <=  9 ) ) 
      {
        return '0' + ( x -  0 );
      }
    else if( ( x >= 10 ) && ( x <= 15 ) ) 
      {
        return 'A' + ( x - 10 );
      }

    return -1;
  }

__attribute__((section(".kernel_text"))) void uart_putc( _uart_t* d, uint8_t x ) {
    while( d->FR & 0x20 ) 
      {
        /* wait while transmit FIFO is full */
      }

    d->DR = x;
  }

__attribute__((section(".kernel_text"))) uint8_t uart_getc( _uart_t* d ) {
    while( d->FR & 0x10 ) 
      {
        /* wait while receive FIFO is empty */
      }

    return d->DR;
  }

__attribute__((section(".kernel_text"))) void uart_puth( _uart_t* d, uint8_t x ) {
    uart_putc( d, itox( ( x >> 4 ) & 0xF ) );
    uart_putc( d, itox( ( x >> 0 ) & 0xF ) );
  }

__attribute__((section(".kernel_text"))) uint8_t uart_geth( _uart_t* d ) {
    uint8_t x  = ( xtoi( uart_getc( d ) ) << 4 );
            x |= ( xtoi( uart_getc( d ) ) << 0 );

    return x;
  }

__attribute__((section(".kernel_text"))) void print_f(char *s ) {   
      _uart_t* UART0 = ( _uart_t* ) UART0_ADDR;
      while(*s != '\0') 
        { /* Loop until end of string */	  
          uart_putc(UART0,(unsigned int)(*s)); // Transmit char
          s++; /* Next char */
        }
  }

__attribute__((section(".kernel_text"))) void print_f_len(char *s, uint16_t len) {   
      _uart_t* UART0 = ( _uart_t* ) UART0_ADDR;
      uint16_t i = 0;
      for(i = 0; i < len; i++)
        {
          uart_putc(UART0,(unsigned int)(s[i]));
        }
  }

