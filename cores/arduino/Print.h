// Print.h - Arduino Print class for GD32VW55x
// Matches the ArduinoCore-API Print interface.

#ifndef _ARDUINO_PRINT_H_
#define _ARDUINO_PRINT_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

class String;

class Print {
private:
    int write_error;
    size_t printNumber(unsigned long n, uint8_t base);
    size_t printNumber(unsigned long long n, uint8_t base);
    size_t printFloat(double number, uint8_t digits);

protected:
    void setWriteError(int err = 1) { write_error = err; }

public:
    Print() : write_error(0) {}

    int getWriteError() { return write_error; }
    void clearWriteError() { setWriteError(0); }

    virtual size_t write(uint8_t b) = 0;
    size_t write(const char *str) {
        if (str == nullptr) {
            return 0;
        }
        return write((const uint8_t *)str, strlen(str));
    }
    virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size) {
        return write((const uint8_t *)buffer, size);
    }

    // Default to zero, meaning "a single write may block".
    // Subclasses with buffering should override this.
    virtual int availableForWrite() { return 0; }

    size_t print(const char str[]);
    size_t print(char c);
    size_t print(unsigned char b, int base = 10);
    size_t print(int n, int base = 10);
    size_t print(unsigned int n, int base = 10);
    size_t print(long n, int base = 10);
    size_t print(unsigned long n, int base = 10);
    size_t print(long long n, int base = 10);
    size_t print(unsigned long long n, int base = 10);
    size_t print(double n, int digits = 2);
    size_t print(const String &s);

    size_t println(void);
    size_t println(const char str[]);
    size_t println(char c);
    size_t println(unsigned char b, int base = 10);
    size_t println(int n, int base = 10);
    size_t println(unsigned int n, int base = 10);
    size_t println(long n, int base = 10);
    size_t println(unsigned long n, int base = 10);
    size_t println(long long n, int base = 10);
    size_t println(unsigned long long n, int base = 10);
    size_t println(double n, int digits = 2);
    size_t println(const String &s);

    virtual void flush() { /* empty by default */ }
};

#endif
