cmake_minimum_required(VERSION 3.18)
project(PseudoSoftwarePWM)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(PseudoSoftwarePWM src/main.cpp src/pwm/PulseWidthModulator.h src/pwm/TaskScheduler/TaskScheduler.h src/pwm/TaskScheduler/TaskScheduler.cpp src/pwm/TaskScheduler/ScheduledTask.cpp src/pwm/TaskScheduler/ScheduledTask.h src/pwm/TaskScheduler/TaskSchedulerError.h src/ChipControl/GPIODeviceController.h src/ChipControl/GPIODLineDirection.h src/ChipControl/GPIODDeviceController.cpp src/pwm/PulseWidthModulator.cpp src/Demo.h src/Demo.cpp)

if (THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(PseudoSoftwarePWM PUBLIC "-pthread")
endif ()
if (CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(PseudoSoftwarePWM "${CMAKE_THREAD_LIBS_INIT}")
endif ()

target_link_libraries(PseudoSoftwarePWM gpiodcxx)
