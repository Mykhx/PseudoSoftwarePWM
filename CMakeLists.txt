cmake_minimum_required(VERSION 3.25)
project(softwarePWMLED)

set(CMAKE_CXX_STANDARD 20)

add_executable(softwarePWMLED src/main.cpp src/pwm/PulseWidthModulator.h src/pwm/TaskScheduler/TaskScheduler.h src/pwm/TaskScheduler/TaskScheduler.cpp src/pwm/TaskScheduler/ScheduledTask.cpp src/pwm/TaskScheduler/ScheduledTask.h src/pwm/TaskScheduler/TaskSchedulerError.h)
