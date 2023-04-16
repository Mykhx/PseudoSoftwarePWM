#include <iostream>
#include "pwm/PulseWidthModulator.h"
#include "gpiod.hpp"
#include "GPIODeviceController.h"

#define CHIP_NAME "gpiochip0"

void testDemo(PulseWidthModulator &pwm);

int main() {
    std::cout << "Hello, World!" << std::endl;

    PulseWidthModulator pwm = PulseWidthModulator();

    testDemo(pwm);

    gpiod::chip chip("gpiochip0");

    GPIODeviceController deviceController = GPIODeviceController();
    deviceController.prepareRequest()
        .withConsumer("testconsumer")
        .forLine("GPIO17")
        .withDirection(OUTPUT)
        .create();


    return 0;
}

void testDemo(PulseWidthModulator &pwm) {
    auto executionTime = std::chrono::high_resolution_clock::now();
    auto switchTask = [](int state) { std::cout << "state: " << state << std::endl; };
    auto switchTaskOn = []() { std::cout << "state: " << 1 << std::endl; };
    auto switchTaskOff = []() { std::cout << "state: " << 0 << std::endl; };


    //pwm.registerTasks(switchTask, executionTime, 0.2);
    pwm.registerTask(switchTaskOn, switchTaskOff, executionTime, 0.2);

    pwm.startPWM();

    std::cout << "started PWM" << std::endl;
    std::this_thread::sleep_for(1s);
    std::cout << "waking up" << std::endl;

    pwm.stopPWM();

    pwm.clearAllTasks();

    std::cout << "cleared all tasks" << std::endl;
    pwm.startPWM();

    std::this_thread::sleep_for(1s);
}
