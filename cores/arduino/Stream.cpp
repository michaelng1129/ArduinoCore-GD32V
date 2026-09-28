// Stream.cpp - Arduino Stream class for GD32VW55x
// Implementation follows ArduinoCore-API/api/Stream.cpp.

#include "Stream.h"
#include "Arduino.h"  // for millis()

// Private method to read stream with timeout
int Stream::timedRead()
{
    int c;
    _startMillis = millis();
    do {
        c = read();
        if (c >= 0) {
            return c;
        }
    } while (millis() - _startMillis < _timeout);
    return -1;  // timeout
}

// Private method to peek stream with timeout
int Stream::timedPeek()
{
    int c;
    _startMillis = millis();
    do {
        c = peek();
        if (c >= 0) {
            return c;
        }
    } while (millis() - _startMillis < _timeout);
    return -1;  // timeout
}

// Private method to look at the stream for the next digit (or decimal point)
// without consuming it. Returns the digit, '.' for decimal point, or -1.
int Stream::peekNextDigit(LookaheadMode lookahead, bool detectDecimal)
{
    int c;
    while (true) {
        c = timedPeek();
        if (c < 0) {
            return c;  // timeout
        }
        if (c == '-') {
            return c;
        }
        if (c >= '0' && c <= '9') {
            return c;
        }
        if (detectDecimal && c == '.') {
            return c;
        }
        if (lookahead == SKIP_NONE) {
            return -1;  // fail, do not skip
        }
        if (lookahead == SKIP_WHITESPACE &&
            c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            return -1;  // fail, non-whitespace found
        }
        read();  // discard non-numeric char
    }
}

size_t Stream::readBytes(char *buffer, size_t length)
{
    size_t count = 0;
    while (count < length) {
        int c = timedRead();
        if (c < 0) {
            break;
        }
        *buffer++ = (char)c;
        count++;
    }
    return count;
}

size_t Stream::readBytesUntil(char terminator, char *buffer, size_t length)
{
    if (length == 0) {
        return 0;
    }
    size_t count = 0;
    while (count < length) {
        int c = timedRead();
        if (c < 0 || c == terminator) {
            break;
        }
        *buffer++ = (char)c;
        count++;
    }
    return count;
}

String Stream::readString()
{
    String ret;
    int c = timedRead();
    while (c >= 0) {
        ret += (char)c;
        c = timedRead();
    }
    return ret;
}

String Stream::readStringUntil(char terminator)
{
    String ret;
    int c = timedRead();
    while (c >= 0 && c != terminator) {
        ret += (char)c;
        c = timedRead();
    }
    return ret;
}

long Stream::parseInt(LookaheadMode lookahead, char ignore)
{
    bool isNegative = false;
    long value = 0;
    int c;

    c = peekNextDigit(lookahead, false);
    if (c < 0) {
        return 0;  // timeout
    }

    do {
        if (c == ignore) {
            // ignore this character
        } else if (c == '-') {
            isNegative = true;
        } else if (c >= '0' && c <= '9') {
            value = value * 10 + (c - '0');
        }
        read();  // consume the character
        c = timedPeek();
        if (!((c >= '0' && c <= '9') || c == ignore)) {
            break;
        }
    } while (true);

    if (isNegative) {
        value = -value;
    }
    return value;
}

float Stream::parseFloat(LookaheadMode lookahead, char ignore)
{
    bool isNegative = false;
    bool isFraction = false;
    long value = 0;
    float fraction = 1.0f;
    int c;

    c = peekNextDigit(lookahead, true);
    if (c < 0) {
        return 0;  // timeout
    }

    do {
        if (c == ignore) {
            // ignore this character
        } else if (c == '-') {
            isNegative = true;
        } else if (c == '.') {
            isFraction = true;
        } else if (c >= '0' && c <= '9') {
            value = value * 10 + (c - '0');
            if (isFraction) {
                fraction *= 0.1f;
            }
        }
        read();  // consume the character
        c = timedPeek();
        if (!((c >= '0' && c <= '9') || (c == '.' && !isFraction) || c == ignore)) {
            break;
        }
    } while (true);

    float result = (float)value;
    if (isFraction) {
        result *= fraction;
    }
    if (isNegative) {
        result = -result;
    }
    return result;
}

// As find, but search ends if the terminator string is found
bool Stream::findUntil(char *target, size_t targetLen, char *terminate, size_t termLen)
{
    if (target == nullptr) {
        return true;  // null target means "no target", always found
    }
    if (targetLen == 0) {
        return true;  // empty target is always found
    }
    if (terminate != nullptr && termLen == 0) {
        return false;  // empty terminator can never match
    }

    size_t targetIndex = 0;
    size_t termIndex = 0;
    int c;
    while ((c = timedRead()) >= 0) {
        if (target != nullptr && c == target[targetIndex]) {
            if (++targetIndex >= targetLen) {
                return true;  // target found
            }
        } else {
            targetIndex = 0;  // reset, target not matched
        }
        if (terminate != nullptr && c == terminate[termIndex]) {
            if (++termIndex >= termLen) {
                return false;  // terminator found, target not found
            }
        } else {
            termIndex = 0;
        }
    }
    return false;  // timeout
}

bool Stream::find(char *target, size_t length)
{
    return findUntil(target, length, nullptr, 0);
}

bool Stream::find(char *target)
{
    return find(target, strlen(target));
}

bool Stream::findUntil(char *target, char *terminator)
{
    return findUntil(target, strlen(target),
                     terminator, terminator ? strlen(terminator) : 0);
}
