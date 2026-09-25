#include "Print.h"
#include "WString.h"
#include <stdio.h>

size_t Print::write(const uint8_t *buffer, size_t size) { size_t n = 0; while (n < size) n += write(buffer[n]); return n; }
size_t Print::write(const char *text) { return text ? write((const uint8_t *)text, __builtin_strlen(text)) : 0; }
size_t Print::print(const char *text) { return write(text); }
size_t Print::print(const String &text) { return write(text.c_str()); }
size_t Print::print(char value) { return write((uint8_t)value); }
static size_t printUnsigned(Print &out, unsigned long value, int base) {
    if (base < 2 || base > 16) base = 10;
    char buf[sizeof(value) * 8 + 1]; unsigned int n = 0;
    do { unsigned int d = value % (unsigned int)base; buf[n++] = (char)(d < 10 ? '0' + d : 'A' + d - 10); value /= (unsigned int)base; } while (value);
    size_t written = 0; while (n) written += out.write((uint8_t)buf[--n]); return written;
}
size_t Print::print(unsigned char v, int base) { return printUnsigned(*this, v, base); }
size_t Print::print(int v, int base) { if (base == 10 && v < 0) return write('-') + printUnsigned(*this, (unsigned long)(-(long)v), base); return printUnsigned(*this, (unsigned int)v, base); }
size_t Print::print(unsigned int v, int base) { return printUnsigned(*this, v, base); }
size_t Print::print(long v, int base) { if (base == 10 && v < 0) return write('-') + printUnsigned(*this, (unsigned long)(-(v + 1)) + 1, base); return printUnsigned(*this, (unsigned long)v, base); }
size_t Print::print(unsigned long v, int base) { return printUnsigned(*this, v, base); }
size_t Print::print(double v, int digits) { char buf[48]; if (digits < 0) digits = 0; if (digits > 8) digits = 8; int n = snprintf(buf, sizeof(buf), "%.*f", digits, v); return n > 0 ? write(buf) : 0; }
size_t Print::println(void) { return write('\r') + write('\n'); }
size_t Print::println(const char *v) { return print(v) + println(); }
size_t Print::println(const String &v) { return print(v) + println(); }
size_t Print::println(int v, int base) { return print(v, base) + println(); }
size_t Print::println(unsigned long v, int base) { return print(v, base) + println(); }
