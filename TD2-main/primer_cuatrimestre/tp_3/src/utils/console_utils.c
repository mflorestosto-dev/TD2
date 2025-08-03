
#include <stdarg.h>
#include "../../inc/uart.h"
#include "../../inc/console_utils.h"

uint32_t div(uint32_t num, uint32_t den);
void Printf(const char *string,...);

/* A mapping from an integer between 0 and 15 to its ASCII character
 * equivalent. */
static const char * const g_pcHex = "0123456789ABCDEF";

/**
 * A simple UART based printf function supporting \%c, \%d, \%p, \%s, \%u,
 * \%x, and \%X.
 *
 * \param string is the format string.
 * \param vaArg is the value identifying a variable arguments list
 *        initialized with va_start.
 *
 * This function is very similar to the C library <tt>vprintf()</tt> function.
 * All of its output will be sent to the UART.  Only the following formatting
 * characters are supported:
 *
 * - \%c to print a character
 * - \%d to print a decimal value
 * - \%s to print a string
 * - \%u to print an unsigned decimal value
 * - \%x to print a hexadecimal value using lower case letters
 * - \%X to print a hexadecimal value using lower case letters (not upper case
 *    letters as would typically be used)
 * - \%p to print a pointer as a hexadecimal value
 * - \%\% to print out a \% character
 *
 * The type of the arguments after \e string must match the requirements of
 * the format string.  For example, if an integer was passed where a string
 * was expected, an error of some kind will most likely occur.
 *
 * \return None.
 */
__attribute__((section(".kernel_text"))) void Printf(const char *string,...)
{
    /*
    * Se guardan los registros en el STACK para que al retornar el estado sea el mismo que cuando se llamó a esta función
    */
    // asm("STMFD SP!,{R0-R12}");
    _uart_t* UART0 = ( _uart_t* ) UART0_ADDR;
    unsigned int pos, count, base, neg;
    char *pcStr, pcBuf[16], cFill;
    unsigned int value,helper_value;
    unsigned int i = 0;
    va_list vaArg;
    uint32_t idx;    

    /* Start the variable arguments processing. */
    va_start (vaArg, string);

    /* Loop while there are more characters in the string. */
    while(*string)
    {
        /* Find the first non-% character, or the end of the string. */
        for(idx = 0u; (string[idx] != '%') && (string[idx] != '\0'); idx++)
        {
        }

        /* Write this portion of the string. */
        // UART_printf(UART_0,string, idx);        
        print_f_len(string,idx);

        /* Skip the portion of the string that was written. */
        string += idx;

        /* See if the next character is a %. */
        if(*string == '%')
        {
            /* Skip the %. */
            string++;

            /*
            ** Set the digit count to zero, and the fill character to space
            ** (i.e. to the defaults).
            */
            count = 0u;
            cFill = ' ';

            /*
            ** It may be necessary to get back here to process more characters.
            ** Goto's aren't pretty, but effective. I feel extremely dirty for
            ** using not one but two of the beasts.
            */
again:

            /* Determine how to handle the next character. */
            switch(*string++)
            {
                /* Handle the digit characters. */
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    /*
                    ** If this is a zero, and it is the first digit, then the
                    ** fill character is a zero instead of a space.
                    */
                    if((string[-1] == '0') && (count == 0))
                    {
                        cFill = '0';
                    }

                    /* Update the digit count. */
                    count *= 10;
                    count += string[-1] - '0';

                    /* Get the next character. */
                    goto again;
                }

                /* Handle the %c command. */
                case 'c':
                {
                    /* Get the value from the varargs. */
                    value = va_arg(vaArg, unsigned int);

                    /* Print out the character. */
                    // UART_printf(UART_0,(char *)&value, 1);
                    // print_f((char *)&value);                    
                    print_f_len((char *)&value,1);

                    /* This command has been handled. */
                    break;
                }

                /* Handle the %d command. */
                case 'd':
                {
                    /* Get the value from the varargs. */
                    value = va_arg(vaArg, unsigned int);

                    /* Reset the buffer position. */
                    pos = 0u;

                    /*
                    ** If the value is negative, make it positive and indicate
                    ** that a minus sign is needed.
                    */
                    if((int)value < 0)
                    {
                        /* Make the value positive. */
                        value = -(int)value;

                        /* Indicate that the value is negative. */
                        neg = 1u;
                    }
                    else
                    {
                        /*
                        ** Indicate that the value is positive so that a minus
                        ** sign isn't inserted.
                        */
                        neg = 0u;
                    }

                    /* Set the base to 10. */
                    base = 10u;

                    /* Convert the value to ASCII. */
                    goto convert;
                }

                /* Handle the %s command. */
                case 's':
                {
                    /* Get the string pointer from the varargs. */
                    pcStr = va_arg(vaArg, char *);

                    /* Determine the length of the string. */
                    for(idx = 0u; pcStr[idx] != '\0'; idx++)
                    {
                    }

                    /* Write the string. */
                    // UART_printf(UART_0, pcStr, idx);                    
                    print_f_len(pcStr,idx);

                    /* Write any required padding spaces */
                    if(count > idx)
                    {
                        count -= idx;
                        while(count--)
                        {
                            // UART_printf(UART_0,(const char *)" ", 1);
                            print_f_len((const char *)" ", 1);                            
                        }
                    }
                    /* This command has been handled. */
                    break;
                }

                /* Handle the %u command. */
                case 'u':
                {
                    /* Get the value from the varargs. */
                    value = va_arg(vaArg, unsigned int);

                    /* Reset the buffer position. */
                    pos = 0u;

                    /* Set the base to 10. */
                    base = 10u;

                    /* Indicate that the value is positive so that a minus sign
                     * isn't inserted. */
                    neg = 0u;

                    /* Convert the value to ASCII. */
                    goto convert;
                }

                /*
                ** Handle the %x and %X commands.  Note that they are treated
                ** identically; i.e. %X will use lower case letters for a-f
                ** instead of the upper case letters it should use.  We also
                ** alias %p to %x.
                */
                case 'x':
                case 'X':
                case 'p':
                {
                    /* Get the value from the varargs. */
                    value = va_arg(vaArg, unsigned int);

                    //UART_printf(UART_0,"\r\nIniciando conversión %X...\r\n",28);

                    /* Reset the buffer position. */
                    pos = 0u;

                    /* Set the base to 16. */
                    base = 16u;

                    /*
                    ** Indicate that the value is positive so that a minus sign
                    ** isn't inserted.
                    */
                    neg = 0u;

                    /*
                    ** Determine the number of digits in the string version of
                    ** the value.
                    */
convert:
                    for(idx = 1;
                        (((idx * base) <= value)) && idx != 0x10000000;//((div((idx * base),base)) == idx);
                        idx *= base, count--)
                    {
                        //UART_printf(UART_0,"\r\nDAMN!", 7);
                    }

                    /*
                    ** If the value is negative, reduce the count of padding
                    ** characters needed.
                    */
                    if(neg)
                    {
                        count--;
                    }

                    /*
                    ** If the value is negative and the value is padded with
                    ** zeros, then place the minus sign before the padding.
                    */
                    if(neg && (cFill == '0'))
                    {
                        /* Place the minus sign in the output buffer. */
                        pcBuf[pos++] = '-';

                        /*
                        ** The minus sign has been placed, so turn off the
                        ** negative flag.
                        */
                        neg = 0u;
                    }

                    /* Provide additional padding at the beginning of the
                     * string conversion if needed. */
                    if((count > 1) && (count < 16))
                    {
                        for(count--; count; count--)
                        {
                            //UART_printf(UART_0,"\r\n??????????", 7);
                            pcBuf[pos++] = cFill;
                        }
                    }

                    /* If the value is negative, then place the minus sign
                     * before the number. */
                    if(neg)
                    {
                        /* Place the minus sign in the output buffer. */
                        pcBuf[pos++] = '-';
                    }

                    /* Convert the value into a string. */
                    helper_value = value;
                    //for(;i==0;i--)
                    for(; idx; idx = base == 10 ? div(idx,base) : idx >> 4)
                    {
                        //UART_printf(UART_0,"\r\n\tdiv!", 7);
                        i = div(helper_value,idx);
                        pcBuf[pos++] = g_pcHex[i];
                        helper_value = helper_value-idx*i;
                    }

                    /* Write the string. */
                    // UART_printf(UART_0,pcBuf, pos);                    
                    print_f_len(pcBuf, pos);

                    /* This command has been handled. */
                    break;
                }

                /* Handle the %% command. */
                case '%':
                {
                    /* Simply write a single %. */
                    // UART_printf(UART_0,string - 1, 1);
                    // print_f(string - 1);                    
                    print_f_len(string - 1, 1);

                    /* This command has been handled. */
                    break;
                }

                /* Handle all other commands. */
                default:
                {
                    /* Indicate an error. */
                    // UART_printf(UART_0,(const char *)"ERROR", 5);                    
                    print_f_len((const char *)"ERROR", 5);

                    /* This command has been handled. */
                    break;
                }
            }
        }
    }

    /* End the variable arguments processing. */
    va_end(vaArg);

    /*
    * Se recuperan los registros del STACK para que al retornar el estado sea el mismo que cuando se llamó a esta función
    */
    // asm("LDMFD SP!,{R0-R12}");
}

__attribute__((section(".kernel_text"))) uint32_t div(uint32_t num, uint32_t den)
{
    uint32_t k = 0;
    //UART_printf(UART_0,"\r\ndiv", 5);
    while(num >= den)
    {
        //UART_printf(UART_0,"\r\n\t\tk", 5);
        num -= den;
        k++;
    }
    return k;
}