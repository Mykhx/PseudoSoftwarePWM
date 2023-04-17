#include "GPIODeviceController.h"
#include "gpiod.hpp"
#include "pwm/PulseWidthModulator.h"
#include "Demo.h"

void demoGPIOPin17Modulation() {
    PulseWidthModulator pwm = PulseWidthModulator();

    gpiod::chip chip("gpiochip0");

    GPIODeviceController deviceController = GPIODeviceController();
    auto aLine = deviceController.prepareRequest()
            .withConsumer("demo")
            .forLine("GPIO17")
            .withDirection(OUTPUT)
            .create();

    aLine.set_value(1);
    std::this_thread::sleep_for(2s);
    aLine.set_value(0);

    auto turnOn = [&aLine] { aLine.set_value(1); };
    auto turnOff = [&aLine] { aLine.set_value(0); };

    auto stopTime = timeProvider::now() + 60s;

    double currentPWMFraction = 1;
    double increment = 0.01;
    while (timeProvider::now() < stopTime) {
        pwm.registerTask(turnOn, turnOff, timeProvider::now(), currentPWMFraction);
        pwm.startPWM();
        std::cout << "currentPWMFraction:" << currentPWMFraction << "\n";
        std::this_thread::sleep_for(50ms);
        pwm.clearAllTasks();
        pwm.stopPWM();
        currentPWMFraction -= increment;
        if (currentPWMFraction < 0.01 or currentPWMFraction > 0.99)
            increment = -increment;
    }

    turnOff();
}
