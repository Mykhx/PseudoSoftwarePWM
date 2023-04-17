#include "PulseWidthModulator.h"

void PulseWidthModulator::registerTask(task taskSwitchToActive, task taskSwitchToInactive, timePoint firstExecutionTime,
                                       double basePeriodFraction) {
    if (basePeriodFraction > 1 or basePeriodFraction <= 0)
        throw std::invalid_argument("Invalid value for basePeriodFraction. Value must be between 0 and 1.");

    auto offsetSubsequentExecutionTime = std::chrono::duration_cast<duration>(std::chrono::duration<double>((basePeriod * basePeriodFraction)));
    printInfo(firstExecutionTime, basePeriodFraction, offsetSubsequentExecutionTime);
    taskScheduler.addTask(std::move(taskSwitchToActive),
                          firstExecutionTime, basePeriod);
    taskScheduler.addTask(std::move(taskSwitchToInactive),
                          firstExecutionTime + offsetSubsequentExecutionTime, basePeriod);
}

void PulseWidthModulator::printInfo(const timePoint &firstExecutionTime, double basePeriodFraction,
                                    const duration &offsetSubsequentExecutionTime) const {
    std::cout << "registered Task with" << std::endl;

    std::cout << "first  time        : " << firstExecutionTime.time_since_epoch().count() << "\n";
    std::cout << "second time        : " << (firstExecutionTime + offsetSubsequentExecutionTime).time_since_epoch().count() << "\n";
    std::cout << "second time (rel)  : " << offsetSubsequentExecutionTime.count() << "\n";

    std::cout << "base period        : " << basePeriod.count() << "\n";
    std::cout << "base period frac   : " << basePeriodFraction << "\n";
    std::cout << "scaled base period : " << basePeriodFraction * basePeriod.count() << "\n";
}

void PulseWidthModulator::startPWM() {
    taskScheduler.startTaskLoop();
}

void PulseWidthModulator::stopPWM() {
    taskScheduler.stopTaskLoop();
}

void PulseWidthModulator::clearAllTasks() {
    taskScheduler.clearTaskQueue();
}
