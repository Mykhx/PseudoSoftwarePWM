#include "ChipControl/GPIODeviceController.h"
#include "gpiod.hpp"
#include "PWM/PulseWidthModulator.h"
#include "Demo.h"
#include <random>

using namespace std::chrono_literals;

void modulateLine(PulseWidthModulator &pwm, const task &turnOn, const task &turnOff,
                  double increment, timePoint stopTime);

void modulateLine3(PulseWidthModulator &pwm, const task &turnOn0, const task &turnOff0, double increment0,
                   const task &turnOn1, const task &turnOff1, double increment1,
                   const task &turnOn2, const task &turnOff2, double increment2,
                   timePoint stopTime);

void modulateLineRGBColor(PulseWidthModulator &pwm,
                          const task &turnOn0, const task &turnOff0,
                          const task &turnOn1, const task &turnOff1,
                          const task &turnOn2, const task &turnOff2,
                          timePoint stopTime);

double getSubsequentIncrement(double increment, double currentPWMFraction);

void defaultLimitFrequencyGPIOPin17() {

    gpiod::chip chip("gpiochip0");

    GPIODeviceController deviceController = GPIODeviceController();
    auto aLine = deviceController.prepareRequest()
            .withConsumer("demo")
            .forLine("GPIO17")
            .withDirection(OUTPUT)
            .create();

    auto startTime = timeProvider::now();

    while (timeProvider::now() < startTime + 10s) {
        aLine.set_value(1);
        aLine.set_value(0);
    }
}

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

    auto turnOn = [&aLine] { aLine.get_value() == 0 ? aLine.set_value(1) : void(); };
    auto turnOff = [&aLine] { aLine.get_value() == 1 ? aLine.set_value(0) : void(); };

    double increment = 0.05;
    modulateLine(pwm, turnOn, turnOff, increment, timeProvider::now() + 60s);

    turnOff();
}

void demoGPIOPin172722Modulation() {
    PulseWidthModulator pwm = PulseWidthModulator(2000us);

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

    std::this_thread::sleep_for(1s);

    turnOn17();
    turnOn27();
    turnOn22();

    modulateLineRGBColor(pwm,
                         turnOff17, turnOn17,
                         turnOff22, turnOn22,
                         turnOff27, turnOn27,
                         timeProvider::now() + 60s);

    std::this_thread::sleep_for(1s);

    std::random_device random_device;
    std::mt19937 random_engine(random_device());
    std::uniform_int_distribution<int> distribution_2_8(2, 10);
    modulateLine3(pwm,
                  turnOff17, turnOn17, abs(static_cast<double>(distribution_2_8(random_engine))) / 100,
                  turnOff27, turnOn27, abs(static_cast<double>(distribution_2_8(random_engine))) / 100,
                  turnOff22, turnOn22, abs(static_cast<double>(distribution_2_8(random_engine))) / 100,
                  timeProvider::now() + 60s);

    std::this_thread::sleep_for(1s);

    double increment = 0.05;

    timePoint stopTime;
    std::cout << "Line: " << 17 << "\n";
    modulateLine(pwm, turnOff17, turnOn17, increment, timeProvider::now() + 5s);

    std::cout << "Line: " << 22 << "\n";
    modulateLine(pwm, turnOff22, turnOn22, increment, timeProvider::now() + 5s);

    std::cout << "Line: " << 27 << "\n";
    modulateLine(pwm, turnOff27, turnOn27, increment, timeProvider::now() + 5s);

    std::this_thread::sleep_for(1s);

    turnOff17();
    turnOff27();
    turnOff22();
}

void modulateLine(PulseWidthModulator &pwm, const task &turnOn, const task &turnOff,
                  double increment, timePoint stopTime) {
    double currentPWMFraction = 0.5;
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

void modulateLine3(PulseWidthModulator &pwm, const task &turnOn0, const task &turnOff0, double increment0,
                   const task &turnOn1, const task &turnOff1, double increment1, const task &turnOn2,
                   const task &turnOff2, double increment2, timePoint stopTime) {
    double currentPWMFraction0 = 0.5;
    double currentPWMFraction1 = 0.5;
    double currentPWMFraction2 = 0.5;

    pwm.startPWM();
    while (timeProvider::now() < stopTime) {
        std::cout << "current increments "
                  << currentPWMFraction0 << " "
                  << currentPWMFraction1 << " "
                  << currentPWMFraction2 << "\n";

        pwm.registerTask(turnOff0, turnOn0, timeProvider::now(), currentPWMFraction0);
        pwm.registerTask(turnOff1, turnOn1, timeProvider::now(), currentPWMFraction1);
        pwm.registerTask(turnOff2, turnOn2, timeProvider::now(), currentPWMFraction2);

        std::this_thread::sleep_for(5s);


        increment0 = getSubsequentIncrement(increment0, currentPWMFraction0);
        increment1 = getSubsequentIncrement(increment1, currentPWMFraction1);
        increment2 = getSubsequentIncrement(increment2, currentPWMFraction2);

        currentPWMFraction0 -= increment0;
        currentPWMFraction1 -= increment1;
        currentPWMFraction2 -= increment2;

        pwm.clearAllTasks();
        turnOn0();
        turnOn1();
        turnOn2();
    }
    pwm.stopPWM();

    turnOn0();
    turnOn1();
    turnOn2();

    std::this_thread::sleep_for(1s);
}

double getSubsequentIncrement(double increment, double currentPWMFraction) {
    if (currentPWMFraction <= abs(increment)
        or currentPWMFraction >= 1 - abs(increment)
        or currentPWMFraction <= 0.2
        or currentPWMFraction >= 0.9)
        increment = -increment;
    return increment;
}

void modulateLineRGBColor(PulseWidthModulator &pwm, const task &turnOn0, const task &turnOff0, const task &turnOn1,
                          const task &turnOff1, const task &turnOn2, const task &turnOff2, timePoint stopTime) {

    std::random_device random_device;
    std::mt19937 random_engine(random_device());
    std::uniform_int_distribution<int> distribution(1, 99);

    PulseWidthModulator pwm0 = PulseWidthModulator(3000us);
    PulseWidthModulator pwm1 = PulseWidthModulator(3000us);
    PulseWidthModulator pwm2 = PulseWidthModulator(3000us);
    pwm0.startPWM();
    pwm1.startPWM();
    pwm2.startPWM();
    while (timeProvider::now() < stopTime) {
        double currentPWMFraction0 = abs(static_cast<double>(distribution(random_engine))) / 100;
        double currentPWMFraction1 = abs(static_cast<double>(distribution(random_engine))) / 100;
        double currentPWMFraction2 = abs(static_cast<double>(distribution(random_engine))) / 100;

        std::cout << "current fractions "
                  << currentPWMFraction0 << " "
                  << currentPWMFraction1 << " "
                  << currentPWMFraction2 << "\n";

        auto firstExecution = timeProvider::now();

        pwm0.registerTask(turnOff0, turnOn0, firstExecution, currentPWMFraction0);
        pwm1.registerTask(turnOff1, turnOn1, firstExecution, currentPWMFraction1);
        pwm2.registerTask(turnOff2, turnOn2, firstExecution, currentPWMFraction2);


        std::this_thread::sleep_for(1s);

        pwm0.clearAllTasks();
        pwm1.clearAllTasks();
        pwm2.clearAllTasks();
    }
    pwm0.stopPWM();
    pwm1.stopPWM();
    pwm2.stopPWM();

    turnOn0();
    turnOn1();
    turnOn2();

    std::this_thread::sleep_for(1s);
}
