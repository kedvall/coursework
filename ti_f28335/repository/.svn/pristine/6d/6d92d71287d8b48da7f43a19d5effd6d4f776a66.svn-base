
#include <coecsl.h>
#include <smallprintf.h>

#define MAX_PREC_DIG			5

/* Simple replacement for the traditional sprintf() function.  The arguments
   and formatting string are equivalent.
   
   int sprintf(char *dest, const char *format, ...)

   Function arguments:
   dest		: pointer to destination character string
   format	: pointer to format character string
   ...		: arguments needed for format string
   
   Return value:
   number of characters in string, not including the terminating '\0'

   The following format identifiers are supported:
   d,i		: integer
   x,X		: integer, hex value
   b		: integer, binary value
   c		: character
   f		: floating point value
   s		: null terminated string
   
   The following format modifiers are supported:
   +		: left justification
   -		: always print sign on signed numbers
   0		: prepad field width with 0
   l		: specifies long field format; long int, long float, etc.
   
   Field width and precision specifiers are also recognized.
*/

int sprintf(char *dest, const char *format,...)
{
	int len;
	va_list ap;
	va_start(ap, format);
	len = vsprintf(dest,format,ap);
	va_end(ap);
	return(len);
}

int vsprintf(char *dest, const char *format, va_list ap)
{
	char in_field, left_just, sign_mode;
	char long_flag, width_prec_flag, zero_pad;
	char pad_char;
	char width, precision;
	char cur_len, total_len, tmp_len;
	int32 whole;

	char *p_frmt, *p_out, *p_tmp;
	char tmp_str[SPRINTF_MAX];
	
	union {
		int ival;
		long lval;
		float fval;
		char *sval;
	} value;

	// Initialize flags and pointers
	in_field = 0;
	p_out = dest;
	p_frmt = (char *)format;

	// Start the dissection of the format string
	do {
		tmp_str[0] = 0;
	
		// Find the first format character
		if((p_tmp = strchr(p_frmt, '%')) == NULL) {
			// If no more formats then the next copy will be to the end of the string
			cur_len = strlen(p_frmt);
		} else {
			// If we have a format then determine the number of characters before the format
			cur_len = p_tmp - p_frmt;
			in_field = -1;
		}
		
		if(cur_len > 0) {
			// If we have characters to copy then do so
			memcpy(p_out, p_frmt, cur_len);
			p_frmt += cur_len;
			p_out += cur_len;
		}

		// Stay in this loop until we are done disecting the format
		while((*p_frmt != 0) && (in_field != 0))
		{
			// If we have started disecting a new format block then initialize the field information
			if(in_field == -1) {
				width = -1;
				precision = -1;
				width_prec_flag = 0;
				left_just = 0;
				sign_mode = 0;
				zero_pad = 0;
				long_flag = 0;
				in_field = 1;
			}
		
			p_tmp = tmp_str;
			switch(*++p_frmt) {

			/* Field Modifiers */
		
			// This is a long/double field
			case 'l':
				long_flag = 1;
				break;
			
			// This is a string field
			case 's':
				value.sval = va_arg(ap, char*);
				strcpy(tmp_str, value.sval);
				if(precision >= 0) tmp_str[precision] = 0;
				zero_pad = 0;
				in_field = 0;
				break;
			
			// This field should be left justified
			case '-':
				left_just = 1;
				break;
			case '+':
				sign_mode = 1;
				break;
			case '0':
				zero_pad = 1;
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				// Initialize the temp string
				*p_tmp = *p_frmt;
			
				// Search the format string for the end of the numeric field
				while((*(p_frmt + 1) >= '0') && (*(p_frmt + 1) <= '9'))
					*++p_tmp = *++p_frmt;
			
				// NULL Terminate the temp string
				*++p_tmp = 0;
			
				// Depending on which field we are reading convert the string to a numeric value
				if(width_prec_flag == 0)
					width = atoi(tmp_str);
				else
					precision = atoi(tmp_str);
			
				break;
			case '.':
				width_prec_flag = 1;
				break;
			
			/* Field specifiers */
			
			// Decimal/Integer field
			case 'd':
			case 'i':
				if(long_flag)
					value.lval = va_arg(ap, long);
				else
					value.lval = va_arg(ap, int);
				if((value.lval > 0) && sign_mode) *p_tmp++ = '+';
				ltoa(value.lval, p_tmp);
				p_tmp = tmp_str;
				in_field = 0;
				break;
			
			// Hex field
			case 'x':
			case 'X':
				if(long_flag)
					value.lval = va_arg(ap, long);
				else
					value.lval = (long)(0x0000ffff & va_arg(ap, int));
				ltoah(value.lval, tmp_str);
				in_field = 0;
				break;
			
			// binary field
			case 'b':
				if(long_flag)
					value.lval = va_arg(ap, long);
				else
					value.lval = (long)(0x0000ffff & va_arg(ap, int));
				ltoab(value.lval, tmp_str);
				in_field = 0;
				break;

			// Character field
			case 'c':
				value.ival = va_arg(ap, char);
				tmp_str[0] = (char)value.ival;
				tmp_str[1] = 0;
				in_field = 0;
				break;
			
			// Float field
			case 'f':
				value.fval = va_arg(ap, float);
				precision = (precision >= 0) ? precision : MAX_PREC_DIG;
				if (value.fval < 0) {
					value.fval = -value.fval;
					*p_tmp++ = '-';
				} else if (sign_mode) {
					*p_tmp++ = '+';
				}
				whole = floor(value.fval);
				p_tmp += ltoa(whole, p_tmp);
				if (precision > 0) {
					*p_tmp++ = '.';
					while (precision--) {
						value.fval -= whole;
						value.fval *= 10;
						whole = floor(value.fval);
						*p_tmp++ = '0' + whole;
					}
				}
				*p_tmp = 0;
				
				in_field = 0;
				break;
			
			default:
				tmp_str[0] = *p_frmt;
				tmp_str[1] = 0;
				in_field = 0;
				break;
			}
		}


		/* If we have finished dissecting the field string then	copy the
		   contents of the string into the output string.  When we get to
		   this point p_tmp should be pointing to the beginning of a string
		   that contains the data to be copied */
		if(tmp_str[0] != 0) {
			p_tmp = tmp_str;
			
			// Figure out how long the string should be
			tmp_len = strlen(p_tmp);
			total_len = tmp_len > width ? tmp_len : width;
			
			// Set the current string length to zero
			cur_len = 0;
			
			// If right justified
			if ((!left_just) && (tmp_len < width)) {
				if(zero_pad && ((*p_tmp == '-') || (*p_tmp == '+')))
					*p_out++ = *p_tmp++;
			
				if(zero_pad)
					pad_char = '0';
				else
					pad_char = ' ';
			
				for(;cur_len < (width - tmp_len); cur_len++)
					*p_out++ = pad_char;
			}
		
			// Copy the string
			for(;*p_tmp; cur_len++)
				*p_out++ = *p_tmp++;
		
			// Fill to the right if left justified
			for(;cur_len < total_len; cur_len++)
				*p_out++ = ' ';
		
			p_frmt++;
		}
	
	} while(*p_frmt != 0);
	
	*p_out = 0;
	return((int)(p_out - dest));
}

/*+**

Name
        ltoah

Synopsis
        int ltoah(long number, char *store_string)

Description
        Convert the long to an ascii string of hex characters
        and store it in the variable store_string;

Return Value
        The number of characters placed in the buffer.

Notes

**-*/

//int ltoah(unsigned long number, char *store_string)
int ltoah(Uint32 number, char *store_string)
{
	unsigned char quotient;
	char tmp_str[10];
	int i, j;
	
	for(i=0,j=0;i<8;i++) {
		quotient = (unsigned char)(0x0f & (number >> ((7-i) * 4)));
	
		if((quotient > 0) || (j != 0)) {
			if(quotient < 10)
				tmp_str[j++] = (char)quotient + 48;
			else
				tmp_str[j++] = (char)quotient + 87;
		}
	}
	if(j == 0)
		tmp_str[j++] = '0';
	
	tmp_str[j] = 0;
	strcpy(store_string, tmp_str);
	return(j);
}


// binary
int ltoab(unsigned long num, char *str)
{
	unsigned long m = num;
	int len = 0;

	do {
		len++;
		m >>= 1;
	} while (m);

	m = 1U << (len-1);
	while (m) {
		*str++ = (num & m) ? '1' : '0';
		m >>= 1;
	}
	*str = 0;
	return(len);
}

