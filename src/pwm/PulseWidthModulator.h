#ifndef PWMMODEL_PSEUDOPULSEWITHMODULATOR_H
#define PWMMODEL_PSEUDOPULSEWITHMODULATOR_H

#include <chrono>
#include <iostream>
#include <functional>
#include <memory>
#include <utility>
#include "TaskScheduler/TaskScheduler.h"

using namespace std::chrono_literals;
using timeProvider = std::chrono::steady_clock;
using timePoint = timeProvider::time_point;
using microseconds = std::chrono::microseconds;

class PulseWidthModulator {
private:
    TaskScheduler taskScheduler;

    duration basePeriod = 1000us;
    std::vector<task> switchTaskList;

    std::function<void()> test1;
    std::function<void()> test2;
public:
    PulseWidthModulator() : taskScheduler(TaskScheduler()) {};

    PulseWidthModulator(microseconds basePeriod) : taskScheduler(TaskScheduler()), basePeriod(basePeriod) {};

    void registerTask(task taskSwitchToActive, task taskSwitchToInactive, timePoint firstExecutionTime,
                      double basePeriodFraction);

    void startPWM();

    void stopPWM();

    void clearAllTasks();
};

#endif //PWMMODEL_PSEUDOPULSEWITHMODULATOR_H