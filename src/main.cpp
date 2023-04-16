#include <iostream>
#include "pwm/PulseWidthModulator.h"
#include "gpiod.hpp"
#include "GPIODeviceController.h"

#define CHIP_NAME "gpiochip0"

void testDemo(PulseWidthModulator &pwm);

int main() {
    std::cout << "Hello, World!" << std::endl;

    PulseWidthModulator pwm = PulseWidthModulator();

    //(pwm);

    gpiod::chip chip("gpiochip0");

    GPIODeviceController deviceController = GPIODeviceController();
    auto aLine = deviceController.prepareRequest()
        .withConsumer("testconsumer")
        .forLine("GPIO17")
        .withDirection(OUTPUT)
        .create();

    aLine.set_value(1);
    std::this_thread::sleep_for(2s);
    aLine.set_value(0);

    auto turnOn = [&aLine]{aLine.set_value(1);};
    auto turnOff = [&aLine]{aLine.set_value(0);};

    while(true) {
        pwm.registerTask(turnOn, turnOff, timeProvider::now(), 0.9);
        pwm.startPWM();
        std::cout << "0.9" << std::endl;
        std::this_thread::sleep_for(1s);
        pwm.clearAllTasks();
        pwm.stopPWM();

        turnOff();

        pwm.registerTask(turnOn, turnOff, timeProvider::now(), 0.8);
        pwm.startPWM();
        std::cout << "0.8" << std::endl;
        std::this_thread::sleep_for(1s);
        pwm.clearAllTasks();
        pwm.stopPWM();

        turnOff();

        pwm.registerTask(turnOn, turnOff, timeProvider::now(), 0.7);
        pwm.startPWM();
        std::cout << "0.7" << std::endl;
        std::this_thread::sleep_for(1s);
        pwm.clearAllTasks();
        pwm.stopPWM();

        turnOff();

        pwm.registerTask(turnOn, turnOff, timeProvider::now(), 0.6);
        pwm.startPWM();
        std::cout << "0.6" << std::endl;
        std::this_thread::sleep_for(1s);
        pwm.clearAllTasks();
        pwm.stopPWM();

        turnOff();

        pwm.registerTask(turnOn, turnOff, timeProvider::now(), 0.5);
        pwm.startPWM();
        std::cout << "0.5" << std::endl;
        std::this_thread::sleep_for(1s);
        pwm.clearAllTasks();
        pwm.stopPWM();

        turnOff();

        pwm.registerTask(turnOn, turnOff, timeProvider::now(), 0.4);
        pwm.startPWM();
        std::cout << "0.4" << std::endl;
        std::this_thread::sleep_for(1s);
        pwm.clearAllTasks();
        pwm.stopPWM();

        turnOff();

        pwm.registerTask(turnOn, turnOff, timeProvider::now(), 0.3);
        pwm.startPWM();
        std::cout << "0.3" << std::endl;
        std::this_thread::sleep_for(1s);
        pwm.clearAllTasks();
        pwm.stopPWM();

        turnOff();

        pwm.registerTask(turnOn, turnOff, timeProvider::now(), 0.2);
        pwm.startPWM();
        std::cout << "0.2" << std::endl;
        std::this_thread::sleep_for(1s);
        pwm.clearAllTasks();
        pwm.stopPWM();
    }
    std::this_thread::sleep_for(5s);


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
