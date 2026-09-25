#ifndef _WSTRING_H_
#define _WSTRING_H_

#include <stddef.h>

class String {
public:
    String();
    String(const char *value);
    String(char value);
    String(int value);
    String(unsigned int value);
    String(long value);
    String(unsigned long value);
    String(const String &other);
    ~String();

    String &operator=(const String &other);
    String &operator=(const char *value);
    String &operator+=(const String &other);
    String &operator+=(const char *value);
    String &operator+=(char value);
    bool operator==(const String &other) const;
    bool operator!=(const String &other) const { return !(*this == other); }
    const char *c_str() const { return buffer_ ? buffer_ : ""; }
    unsigned int length() const;
    bool isEmpty() const { return length() == 0; }
    char charAt(unsigned int index) const;
    char operator[](unsigned int index) const { return charAt(index); }
    void reserve(unsigned int capacity);
    void clear();

private:
    char *buffer_;
    unsigned int capacity_;
    unsigned int length_;
    bool assign(const char *value);
};

String operator+(const String &left, const String &right);
String operator+(const String &left, const char *right);
String operator+(const char *left, const String &right);

#endif
