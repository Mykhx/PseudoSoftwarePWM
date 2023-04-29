cmake_minimum_required(VERSION 3.18)
project(PseudoSoftwarePWM)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(PseudoSoftwarePWM src/main.cpp src/PWM/PulseWidthModulator.h src/PWM/PulseWidthModulator.cpp src/PWM/TaskScheduler/TaskScheduler.h src/PWM/TaskScheduler/TaskScheduler.cpp src/PWM/TaskScheduler/ScheduledTask.cpp src/PWM/TaskScheduler/ScheduledTask.h src/PWM/TaskScheduler/TaskSchedulerError.h src/ChipControl/GPIODeviceController.h src/ChipControl/GPIODLineDirection.h src/ChipControl/GPIODeviceController.cpp src/Demo.h src/Demo.cpp)

if (THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(PseudoSoftwarePWM PUBLIC "-pthread")
endif ()
if (CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(PseudoSoftwarePWM "${CMAKE_THREAD_LIBS_INIT}")
endif ()

target_link_libraries(PseudoSoftwarePWM gpiodcxx)
