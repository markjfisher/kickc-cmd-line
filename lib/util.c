#pragma code_seg(Code)

inline void swap(char *a, char *b) {
    char t = *a;
    *a = *b;
    *b = t;
}

void reverse(char *str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        swap((str + start), (str + end));
        start++;
        end--;
    }
}

// Implementation of itoa() 
char *itoa(int num, char *str, int base) {
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = 0;
        return str;
    }

    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Process individual digits 
    while (num != 0) {
        // int rem = num % base;
        int rem = simple_mod(num, base);
        str[i++] = (rem > 9) ? (char)((rem - 10) + 'a') : (char)(rem + '0');
        num = simple_div(num, base);
    }

    // If number is negative, append '-' 
    if (isNegative)
        str[i++] = '-';

    str[i] = 0; // Append string terminator 

    // Reverse the string 
    reverse(str, i);

    return str;
}

// doesn't handle negatives
int simple_mod(int num, int base) {
	return num - simple_div(num, base) * base;
}

int simple_div(int a, int b) {
	if (b > a) return 0;
	int r = 0;
	while (a > 0) {
		a -= b;
		r++;
	}
	return r;
}
