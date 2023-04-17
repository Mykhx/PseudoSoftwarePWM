//
// Created by mkehr on 16.04.2023.
//

#ifndef PSEUDOSOFTWAREPWM_GPIODEVICECONTROLLER_H
#define PSEUDOSOFTWAREPWM_GPIODEVICECONTROLLER_H

#include <utility>

#include "pwm/PulseWidthModulator.h"
#include "GPIODLineDirection.h"

#include "gpiod.hpp"

#define DEFAULT_CHIP_NAME "gpiochip0"
#define DEFAULT_CONSUMER_NAME "GPIODeviceController"


class GPIODeviceController {
protected:
    PulseWidthModulator pwm;
    gpiod::chip gpiodChip;
    std::string consumerName = DEFAULT_CONSUMER_NAME;

    class request {
    private:
        GPIODeviceController &deviceController;

        explicit request(GPIODeviceController &deviceController) : deviceController(deviceController) {};
        std::string consumer;
        std::string lineName;
        GPIODLineDirection lineDirection = OUTPUT;

    public:
        request() = delete;

        static request prepareRequest(GPIODeviceController &deviceController);

        request withConsumer(std::string consumerName);

        request withDirection(GPIODLineDirection direction);

        request forLine(std::string lineName);

        gpiod::line create();
    };

public:
    explicit GPIODeviceController(const std::string &chipName) : gpiodChip(gpiod::chip(chipName)),
                                                                 pwm(PulseWidthModulator()) {}

    GPIODeviceController() : gpiodChip(gpiod::chip(DEFAULT_CHIP_NAME)), pwm(PulseWidthModulator()) {}

    request prepareRequest();
};

#endif //PSEUDOSOFTWAREPWM_GPIODEVICECONTROLLER_H
