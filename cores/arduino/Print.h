#ifndef _ARDUINO_PRINT_H_
#define _ARDUINO_PRINT_H_

#include <stddef.h>
#include <stdint.h>
class String;

class Print {
public:
    virtual ~Print() {}
    virtual size_t write(uint8_t value) = 0;
    size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *text);
    size_t print(const char *text);
    size_t print(const String &text);
    size_t print(char value);
    size_t print(unsigned char value, int base = 10);
    size_t print(int value, int base = 10);
    size_t print(unsigned int value, int base = 10);
    size_t print(long value, int base = 10);
    size_t print(unsigned long value, int base = 10);
    size_t print(double value, int digits = 2);
    size_t println(void);
    size_t println(const char *text);
    size_t println(const String &text);
    size_t println(int value, int base = 10);
    size_t println(unsigned long value, int base = 10);
    template <typename T> size_t println(T value) { return print(value) + println(); }
};

#endif
