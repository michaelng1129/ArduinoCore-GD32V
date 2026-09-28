// Stream.h - Arduino Stream class for GD32VW55x
// Matches the ArduinoCore-API Stream interface:
//   https://github.com/arduino/ArduinoCore-API/blob/master/api/Stream.h

#ifndef _ARDUINO_STREAM_H_
#define _ARDUINO_STREAM_H_

#include "Print.h"

// Stream lookahead modes for parseInt()/parseFloat()
enum LookaheadMode {
    SKIP_ALL,       // Skip all input before parsing
    SKIP_NONE,      // Nothing is skipped, parsing starts at the first char
    SKIP_WHITESPACE // Skip only whitespace before parsing
};

#define NO_IGNORE_CHAR  '\x01'  // A char that is never part of a number

class Stream : public Print {
protected:
    unsigned long _timeout;      // Max ms to wait for data before timed read aborts
    unsigned long _startMillis;  // Used for timeout measurement
    int timedRead();             // Read with timeout, -1 on timeout
    int timedPeek();             // Peek with timeout, -1 on timeout
    int peekNextDigit(LookaheadMode lookahead, bool detectDecimal);

public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;

    Stream() : _timeout(1000) {}

    void setTimeout(unsigned long timeout) { _timeout = timeout; }
    unsigned long getTimeout(void) { return _timeout; }

    bool find(char *target);
    bool find(uint8_t *target) { return find((char *)target); }
    bool find(char *target, size_t length);
    bool find(uint8_t *target, size_t length) { return find((char *)target, length); }
    bool find(char target) { return find(&target, 1); }

    bool findUntil(char *target, char *terminator);
    bool findUntil(uint8_t *target, char *terminator) {
        return findUntil((char *)target, terminator);
    }
    bool findUntil(char *target, size_t targetLen, char *terminate, size_t termLen);
    bool findUntil(uint8_t *target, size_t targetLen, char *terminate, size_t termLen) {
        return findUntil((char *)target, targetLen, (char *)terminate, termLen);
    }

    long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);
    float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);

    size_t readBytes(char *buffer, size_t length);
    size_t readBytes(uint8_t *buffer, size_t length) {
        return readBytes((char *)buffer, length);
    }
    size_t readBytesUntil(char terminator, char *buffer, size_t length);
    size_t readBytesUntil(char terminator, uint8_t *buffer, size_t length) {
        return readBytesUntil(terminator, (char *)buffer, length);
    }

    String readString();
    String readStringUntil(char terminator);

protected:
    // Deprecated single-arg overloads, kept for sketch compatibility
    long parseInt(char ignore) { return parseInt(SKIP_ALL, ignore); }
    float parseFloat(char ignore) { return parseFloat(SKIP_ALL, ignore); }
};

#endif
