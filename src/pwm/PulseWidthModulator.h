#ifndef PWMMODEL_PSEUDOPULSEWITHMODULATOR_H
#define PWMMODEL_PSEUDOPULSEWITHMODULATOR_H

#include <chrono>
#include <iostream>
#include <functional>
#include <memory>
#include <utility>
#include "TaskScheduler/TaskScheduler.h"

#define INACTIVE 0
#define ACTIVE 1

using namespace std::chrono_literals;
using timeProvider = std::chrono::high_resolution_clock;
using timePoint = timeProvider::time_point;
using milliseconds = std::chrono::milliseconds;
using setStateTask = std::function<void(int)>;

class PulseWidthModulator {
private:
    TaskScheduler taskScheduler = TaskScheduler();

    std::chrono::duration<int,std::milli> basePeriod = 100ms;
    std::vector<task> switchTaskList;

    std::function<void()> test1;
    std::function<void()> test2;
public:
    PulseWidthModulator() : taskScheduler(TaskScheduler()) {};
    PulseWidthModulator(milliseconds basePeriod) : taskScheduler(TaskScheduler()), basePeriod(basePeriod) {};

    void registerTask(task taskSwitchToActive, task taskSwitchToInactive, timePoint firstExecutionTime, double basePeriodFraction) {
        if (basePeriodFraction > 1 or basePeriodFraction <= 0)
            throw std::invalid_argument("Invalid value for basePeriodFraction. Value must be between 0 and 1.");

        //ToDo: this is probably off
        auto offsetSubsequentExecutionTime =basePeriod; //std::chrono::duration_cast<std::chrono::milliseconds>(basePeriod * basePeriodFraction);

        std::cout << "registered Task with" << std::endl;

        std::cout << "first  time        : " << firstExecutionTime.time_since_epoch().count() << "\n";
        std::cout << "second time        : " << (firstExecutionTime + offsetSubsequentExecutionTime).time_since_epoch().count() << "\n";
        std::cout << "second time (rel)  : " << offsetSubsequentExecutionTime.count() << "\n";

        std::cout << "base period        : " << basePeriod.count() << "\n";
        std::cout << "base period frac   : " << basePeriodFraction << "\n";
        std::cout << "scaled base period : " << basePeriodFraction * basePeriod.count() << "\n";

        taskScheduler.addTask(std::move(taskSwitchToActive), firstExecutionTime + offsetSubsequentExecutionTime, basePeriod);
        taskScheduler.addTask(std::move(taskSwitchToInactive), firstExecutionTime + offsetSubsequentExecutionTime, basePeriod);
    }

    void startPWM() {
        taskScheduler.startTaskLoop();
    }

    void stopPWM() {
        taskScheduler.stopTaskLoop();
    }

    void clearAllTasks() {
        taskScheduler.clearTaskQueue();
    }
};

#endif //PWMMODEL_PSEUDOPULSEWITHMODULATOR_H