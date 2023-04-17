#include "pwm/PulseWidthModulator.h"
#include "gpiod.hpp"
#include "GPIODeviceController.h"

int main() {
    PulseWidthModulator pwm = PulseWidthModulator();

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


    return 0;
}

