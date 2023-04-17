#include "GPIODeviceController.h"

GPIODeviceController::request GPIODeviceController::prepareRequest() {
    return request::prepareRequest(*this);
}

GPIODeviceController::request GPIODeviceController::request::prepareRequest(GPIODeviceController &deviceController) {
    return request(deviceController);
}

gpiod::line GPIODeviceController::request::create() {
    auto request = gpiod::line_request();
    request.consumer = this->consumer;
    request.request_type = this->lineDirection;
    auto line = deviceController.gpiodChip.find_line(this->lineName);
    line.request(request);
    return line;
}

GPIODeviceController::request GPIODeviceController::request::withDirection(GPIODLineDirection direction) {
    this->lineDirection = direction;
    return *this;
}

GPIODeviceController::request GPIODeviceController::request::forLine(std::string lineName) {
    this->lineName = std::move(lineName);
    return *this;
}

GPIODeviceController::request GPIODeviceController::request::withConsumer(std::string consumerName) {
    this->consumer = std::move(consumerName);
    return *this;
}
