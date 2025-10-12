//
// Created by TIZI on 28/06/2024.
//

#include "Timer.hpp"

inline void Timer::update() {
    currentTime_ = clock_t::now();
}

void Timer::step() {
    currentTime_ = clock_t::now();
    lastStep_ = currentTime_;
}

void Timer::wait(float time) {
    step();
    while (!timePassed(time)) {}
}

bool Timer::timePassed() {
    if (deltaTime() < cycleDuration_) return false;

    step();
    return true;
}

bool Timer::timePassed(float time) {
    return deltaTime() >= time;
}

float Timer::deltaTime() {
    update();
    return std::chrono::duration_cast<duration_t>(currentTime_ - lastStep_).count();
}

float Timer::lifetime() {
    update();
    return std::chrono::duration_cast<duration_t>(currentTime_ - startTime_).count();
}