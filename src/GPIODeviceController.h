//
// Created by mkehr on 16.04.2023.
//

#ifndef PSEUDOSOFTWAREPWM_GPIODEVICECONTROLLER_H
#define PSEUDOSOFTWAREPWM_GPIODEVICECONTROLLER_H

#include <utility>

#include "pwm/PulseWidthModulator.h"
#include "gpiod.hpp"

#define DEFAULT_CHIP_NAME "gpiochip0"

enum direction : int
{
    INPUT = gpiod::line_request::DIRECTION_INPUT,
    OUTPUT = gpiod::line_request::DIRECTION_OUTPUT
};

class GPIODeviceController {
protected:
    PulseWidthModulator pwm;
    gpiod::chip gpiodChip;

    std::string consumerName = "GPIODeviceController";

public:
    class request {
    private:
        GPIODeviceController& deviceController;
        request(GPIODeviceController& deviceController) : deviceController(deviceController) {};
        std::string consumer;
        std::string lineName;
        direction lineDirection;

    public:
        request() = delete;

        static request prepareRequest(GPIODeviceController& deviceController) {
            return request(deviceController);
        }

        request withConsumer(std::string consumer) {
            this->consumer = std::move(consumer);
            return *this;
        }

        request forLine(std::string lineName) {
            this->lineName = std::move(lineName);
            return *this;
        }

        request withDirection(direction direction) {
            this->lineDirection = direction;
            return *this;
        }

        gpiod::line create() {
            auto request = gpiod::line_request();
            request.consumer = this->consumer;
            request.request_type = this->lineDirection;
            auto line = deviceController.gpiodChip.find_line(this->lineName);
            line.request(request);
            return line;
        }
    };

    GPIODeviceController(const std::string& chipName) : gpiodChip(gpiod::chip(chipName)), pwm(PulseWidthModulator()) {}
    GPIODeviceController() : gpiodChip(gpiod::chip(DEFAULT_CHIP_NAME)), pwm(PulseWidthModulator()) {}

    request prepareRequest() {
        return request::prepareRequest(*this);
    }

};

#endif //PSEUDOSOFTWAREPWM_GPIODEVICECONTROLLER_H
