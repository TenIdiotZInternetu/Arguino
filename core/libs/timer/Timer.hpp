//
// Created by TIZI on 28/06/2024.
//

#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

/**
 * @brief Class for encapsulating std::chrono clock. Tracks time steps and waits for time to pass.
 */
struct Timer {
public:
    /**
     * @brief Type for clock used by the timer
     */
    using clock_t = std::chrono::high_resolution_clock;

    /**
     * @brief Type for duration of time
     */
    using duration_t = std::chrono::microseconds;

    /**
     * @brief Type for time points
     */
    using time_point_t = std::chrono::time_point<clock_t>;

private:
    /**
     * @brief Time when the timer was created
     */
    time_point_t startTime_ = clock_t::now();

    /**
     * @brief Time when the timer was last stepped
     */
    time_point_t lastStep_ = clock_t::now();

    /**
     * @brief Time when the timer was last updated
     */
    time_point_t currentTime_ = clock_t::now();

    /**
     * @brief Duration of the timer's waiting cycle in milliseconds
     */
    float cycleDuration_;

public:
    /**
     * @brief Initializes the timer with a cycle duration of 0
     */
    Timer() : cycleDuration_(0) {
        update();
    }

    /**
     * @brief Initializes the timer with a given cycle duration
     * @param duration Duration of the timer's waiting cycle in milliseconds
     */
    Timer(float duration) : cycleDuration_(duration) {
        update();
    }

    /**
     * @brief Updates the timer's current time
     */
    void update();

    /**
     * @brief Updates the timer and creates a new time step.
     */
    void step();

    /**
     * @brief Blocks the thread for a given amount of time
     * @param time Time to wait in milliseconds
     */
    void wait(float time);

    /**
     * @brief Checks if the internal cycle duration of the timer has passed since the last step.
     * @return True if enough time has passed.
     */
    bool timePassed();

    /**
     * @brief Checks if a given amount of time has passed since the last step.
     * @param time Time to check in milliseconds
     * @return True if enough time has passed.
     */
    bool timePassed(float time);

    /**
     * @return Time passed since the last step in milliseconds
     */
    [[nodiscard]] float deltaTime();

    /**
     * @return Time passed since the timer was created in milliseconds
     */
    [[nodiscard]] float lifetime();
};

#endif //SRC_TIMER_HPP
