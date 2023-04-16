#include <iostream>
#include "pwm/PulseWidthModulator.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    PulseWidthModulator pwm = PulseWidthModulator();

    auto executionTime = std::chrono::high_resolution_clock::now();
    auto switchTask = [](int state) { std::cout << "state: " << state << std::endl; };
    auto switchTaskOn = []() { std::cout << "state: " << 1 << std::endl; };
    auto switchTaskOff = []() { std::cout << "state: " << 0 << std::endl; };


    //pwm.registerTasks(switchTask, executionTime, 0.2);
    pwm.registerTask(switchTaskOn, switchTaskOff, executionTime, 0.2);

    pwm.startPWM();

    std::cout << "started PWM" << std::endl;
    std::this_thread::sleep_for(3s);
    std::cout << "waking up" << std::endl;

    pwm.stopPWM();

    pwm.clearAllTasks();

    std::cout << "cleared all tasks" << std::endl;
    pwm.startPWM();

    std::this_thread::sleep_for(1s);



    return 0;
}
