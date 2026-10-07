#pragma once
#include <stddef.h>
template <typename T>
class ArduinoQueue {
    T values[64];
    size_t count = 0;
    size_t capacity;
public:
    explicit ArduinoQueue(size_t maxItems) : capacity(maxItems) {}
    bool enqueue(const T& value) {
        if (count >= capacity) return false;
        values[count++] = value;
        return true;
    }
    T dequeue() { T value = values[0]; for (size_t i = 1; i < count; ++i) values[i - 1] = values[i]; --count; return value; }
    bool isEmpty() const { return count == 0; }
    size_t item_count() const { return count; }
    void clear() { count = 0; }
};
