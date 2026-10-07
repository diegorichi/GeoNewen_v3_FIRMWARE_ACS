#pragma once
template <int, unsigned long (*Clock)(), typename Arg>
class Timer {
public:
    template <typename Function>
    void every(unsigned long, Function, Arg) {}
    void tick() {}
};
