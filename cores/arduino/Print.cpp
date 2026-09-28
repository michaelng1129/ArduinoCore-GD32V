// Print.cpp - Arduino Print class for GD32VW55x
// Implementation follows ArduinoCore-API/api/Print.cpp.

#include "Print.h"
#include "WString.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

size_t Print::write(const uint8_t *buffer, size_t size)
{
    size_t n = 0;
    while (size--) {
        if (write(*buffer++)) {
            n++;
        } else {
            break;
        }
    }
    return n;
}

size_t Print::printNumber(unsigned long n, uint8_t base)
{
    char buf[8 * sizeof(long) + 1];  // Enough for binary representation
    char *str = &buf[sizeof(buf) - 1];
    *str = '\0';

    if (base < 2) {
        base = 10;
    }
    do {
        char c = n % base;
        n /= base;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);

    return write(str);
}

size_t Print::printNumber(unsigned long long n, uint8_t base)
{
    char buf[8 * sizeof(long long) + 1];
    char *str = &buf[sizeof(buf) - 1];
    *str = '\0';

    if (base < 2) {
        base = 10;
    }
    do {
        unsigned long long q = n / base;
        char c = (char)(n - q * base);
        n = q;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);

    return write(str);
}

size_t Print::printFloat(double number, uint8_t digits)
{
    size_t n = 0;

    if (isnan(number)) {
        return print("nan");
    }
    if (isinf(number)) {
        return print("inf");
    }
    if (number > 4294967040.0 || number < -4294967040.0) {
        // Too large for integer conversion, use snprintf fallback
        char buf[48];
        int len = snprintf(buf, sizeof(buf), "%.*f", digits, number);
        return len > 0 ? write(buf) : 0;
    }

    // Handle negative numbers
    if (number < 0.0) {
        n += print('-');
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i) {
        rounding /= 10.0;
    }
    number += rounding;

    // Extract the integer part of the number
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    n += print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0) {
        n += print('.');
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0) {
        remainder *= 10.0;
        unsigned int toPrint = (unsigned int)remainder;
        n += print(toPrint);
        remainder -= toPrint;
    }

    return n;
}

size_t Print::print(const char str[])
{
    return write(str);
}

size_t Print::print(char c)
{
    return write((uint8_t)c);
}

size_t Print::print(unsigned char b, int base)
{
    return print((unsigned long)b, base);
}

size_t Print::print(int n, int base)
{
    return print((long)n, base);
}

size_t Print::print(unsigned int n, int base)
{
    return print((unsigned long)n, base);
}

size_t Print::print(long n, int base)
{
    if (base == 0) {
        return write((uint8_t)n);
    } else if (base == 10) {
        if (n < 0) {
            size_t t = print('-');
            n = -n;
            return printNumber((unsigned long)n, 10) + t;
        }
        return printNumber((unsigned long)n, 10);
    } else {
        return printNumber((unsigned long)n, (uint8_t)base);
    }
}

size_t Print::print(unsigned long n, int base)
{
    if (base == 0) {
        return write((uint8_t)n);
    } else {
        return printNumber(n, (uint8_t)base);
    }
}

size_t Print::print(long long n, int base)
{
    if (base == 0) {
        return write((uint8_t)n);
    } else if (base == 10) {
        if (n < 0) {
            size_t t = print('-');
            n = -n;
            return printNumber((unsigned long long)n, 10) + t;
        }
        return printNumber((unsigned long long)n, 10);
    } else {
        return printNumber((unsigned long long)n, (uint8_t)base);
    }
}

size_t Print::print(unsigned long long n, int base)
{
    if (base == 0) {
        return write((uint8_t)n);
    } else {
        return printNumber(n, (uint8_t)base);
    }
}

size_t Print::print(double n, int digits)
{
    return printFloat(n, (uint8_t)digits);
}

size_t Print::print(const String &s)
{
    return write(s.c_str(), s.length());
}

size_t Print::println(void)
{
    return write("\r\n");
}

size_t Print::println(const char str[])
{
    size_t n = print(str);
    n += println();
    return n;
}

size_t Print::println(char c)
{
    size_t n = print(c);
    n += println();
    return n;
}

size_t Print::println(unsigned char b, int base)
{
    size_t n = print(b, base);
    n += println();
    return n;
}

size_t Print::println(int n, int base)
{
    size_t s = print(n, base);
    s += println();
    return s;
}

size_t Print::println(unsigned int n, int base)
{
    size_t s = print(n, base);
    s += println();
    return s;
}

size_t Print::println(long n, int base)
{
    size_t s = print(n, base);
    s += println();
    return s;
}

size_t Print::println(unsigned long n, int base)
{
    size_t s = print(n, base);
    s += println();
    return s;
}

size_t Print::println(long long n, int base)
{
    size_t s = print(n, base);
    s += println();
    return s;
}

size_t Print::println(unsigned long long n, int base)
{
    size_t s = print(n, base);
    s += println();
    return s;
}

size_t Print::println(double n, int digits)
{
    size_t s = print(n, digits);
    s += println();
    return s;
}

size_t Print::println(const String &s)
{
    size_t n = print(s);
    n += println();
    return n;
}
