#include "GPIODeviceController.h"
#include "gpiod.hpp"
#include "pwm/PulseWidthModulator.h"
#include "Demo.h"

void modulateLine(PulseWidthModulator &pwm, const task& turnOn, const task& turnOff,
                  double increment, timePoint stopTime);

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

    auto turnOn = [&aLine] { aLine.get_value() == 0 ? aLine.set_value(1) : void (); };
    auto turnOff = [&aLine] {  aLine.get_value() == 1 ? aLine.set_value(0) : void (); };

    double increment = 0.05;
    modulateLine(pwm, turnOn, turnOff, increment, timeProvider::now() + 60s);

    turnOff();
}

void demoGPIOPin172722Modulation() {
    PulseWidthModulator pwm = PulseWidthModulator();

    gpiod::chip chip("gpiochip0");

    GPIODeviceController deviceController = GPIODeviceController();
    auto line17 = deviceController.prepareRequest()
            .withConsumer("demo")
            .forLine("GPIO17")
            .withDirection(OUTPUT)
            .create();
    auto line22 = deviceController.prepareRequest()
            .withConsumer("demo")
            .forLine("GPIO22")
            .withDirection(OUTPUT)
            .create();
    auto line27 = deviceController.prepareRequest()
            .withConsumer("demo")
            .forLine("GPIO27")
            .withDirection(OUTPUT)
            .create();

    auto turnOn17 = [&line17] { line17.set_value(1); };
    auto turnOff17 = [&line17] { line17.set_value(0); };
    auto turnOn22 = [&line22] { line22.set_value(1); };
    auto turnOff22 = [&line22] { line22.set_value(0); };
    auto turnOn27 = [&line27] { line27.set_value(1); };
    auto turnOff27 = [&line27] { line27.set_value(0); };

    turnOff17();
    turnOn27();
    turnOn22();

    std::this_thread::sleep_for(2s);

    turnOn17();
    turnOn27();
    turnOn22();

    std::this_thread::sleep_for(1s);

    double increment = 0.05;

    timePoint stopTime;
    std::cout << "Line: " << 17 << "\n";
    modulateLine(pwm, turnOff17, turnOn17, increment, timeProvider::now() + 10s);

    std::cout << "Line: " << 22 << "\n";
    modulateLine(pwm, turnOff22, turnOn22, increment, timeProvider::now() + 10s);

    std::cout << "Line: " << 27 << "\n";
    modulateLine(pwm, turnOff27, turnOn27, increment, timeProvider::now() + 10s);

    std::this_thread::sleep_for(1s);

    turnOff17();
    turnOff27();
    turnOff22();
}

void modulateLine(PulseWidthModulator &pwm, const task& turnOn, const task& turnOff,
                  double increment, timePoint stopTime) {
    double currentPWMFraction = 1.0;
    pwm.startPWM();
    while (timeProvider::now() < stopTime) {
        std::cout << "current increment " << currentPWMFraction << "\n";
        pwm.registerTask(turnOff, turnOn, timeProvider::now(), currentPWMFraction);
        std::this_thread::sleep_for(500ms);
        pwm.clearAllTasks();
        currentPWMFraction -= increment;
        if (currentPWMFraction < abs(increment) or currentPWMFraction > 1 - abs(increment))
            increment = -increment;
    }
    turnOff();
    pwm.stopPWM();

    std::this_thread::sleep_for(1s);
}
