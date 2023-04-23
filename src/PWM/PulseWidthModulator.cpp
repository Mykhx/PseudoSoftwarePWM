#include "PulseWidthModulator.h"

void PulseWidthModulator::registerTask(task taskSwitchToActive, task taskSwitchToInactive, timePoint firstExecutionTime,
                                       double basePeriodFraction) {
    if (basePeriodFraction > 1 or basePeriodFraction <= 0)
        throw std::invalid_argument("Invalid value for basePeriodFraction. Value must be between 0 and 1.");

    auto offsetSubsequentExecutionTime = std::chrono::duration_cast<duration>(
            std::chrono::duration<double>((basePeriod * basePeriodFraction)));
    taskScheduler.addTask(std::move(taskSwitchToActive),
                          firstExecutionTime, basePeriod);
    taskScheduler.addTask(std::move(taskSwitchToInactive),
                          firstExecutionTime + offsetSubsequentExecutionTime, basePeriod);
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
