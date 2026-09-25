#include "WString.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

String::String() : buffer_(NULL), capacity_(0), length_(0) { assign(""); }
String::String(const char *value) : buffer_(NULL), capacity_(0), length_(0) { assign(value); }
String::String(char value) : buffer_(NULL), capacity_(0), length_(0) { char s[2] = {value, 0}; assign(s); }
String::String(int value) : buffer_(NULL), capacity_(0), length_(0) { char s[24]; snprintf(s, sizeof(s), "%d", value); assign(s); }
String::String(unsigned int value) : buffer_(NULL), capacity_(0), length_(0) { char s[24]; snprintf(s, sizeof(s), "%u", value); assign(s); }
String::String(long value) : buffer_(NULL), capacity_(0), length_(0) { char s[32]; snprintf(s, sizeof(s), "%ld", value); assign(s); }
String::String(unsigned long value) : buffer_(NULL), capacity_(0), length_(0) { char s[32]; snprintf(s, sizeof(s), "%lu", value); assign(s); }
String::String(const String &other) : buffer_(NULL), capacity_(0), length_(0) { assign(other.c_str()); }
String::~String() { free(buffer_); }

bool String::assign(const char *value) {
    if (!value) value = "";
    unsigned int n = (unsigned int)strlen(value);
    if (n + 1 > capacity_) {
        char *next = (char *)realloc(buffer_, n + 1);
        if (!next) return false;
        buffer_ = next;
        capacity_ = n + 1;
    }
    memcpy(buffer_, value, n + 1);
    length_ = n;
    return true;
}

String &String::operator=(const String &other) { if (this != &other) assign(other.c_str()); return *this; }
String &String::operator=(const char *value) { assign(value); return *this; }
String &String::operator+=(const String &other) { return *this += other.c_str(); }
String &String::operator+=(const char *value) {
    if (!value) return *this;
    unsigned int add = (unsigned int)strlen(value);
    if (length_ + add + 1 > capacity_) {
        unsigned int cap = length_ + add + 1;
        char *next = (char *)realloc(buffer_, cap);
        if (!next) return *this;
        buffer_ = next;
        capacity_ = cap;
    }
    memcpy(buffer_ + length_, value, add + 1);
    length_ += add;
    return *this;
}
String &String::operator+=(char value) { char s[2] = {value, 0}; return *this += s; }
bool String::operator==(const String &other) const { return strcmp(c_str(), other.c_str()) == 0; }
unsigned int String::length() const { return length_; }
char String::charAt(unsigned int index) const { return index < length_ ? buffer_[index] : 0; }
void String::reserve(unsigned int capacity) {
    if (capacity <= capacity_) return;
    char *next = (char *)realloc(buffer_, capacity);
    if (next) { buffer_ = next; capacity_ = capacity; if (length_ == 0) buffer_[0] = 0; }
}
void String::clear() { if (buffer_) buffer_[0] = 0; length_ = 0; }
String operator+(const String &a, const String &b) { String out(a); out += b; return out; }
String operator+(const String &a, const char *b) { String out(a); out += b; return out; }
String operator+(const char *a, const String &b) { String out(a); out += b; return out; }
