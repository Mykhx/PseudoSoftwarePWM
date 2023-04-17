//
// Created by mkehr on 17.04.2023.
//

#ifndef PSEUDOSOFTWAREPWM_GPIODLINEDIRECTION_H
#define PSEUDOSOFTWAREPWM_GPIODLINEDIRECTION_H

#include <gpiod.hpp>

enum GPIODLineDirection  : int {
        INPUT = gpiod::line_request::DIRECTION_INPUT,
        OUTPUT = gpiod::line_request::DIRECTION_OUTPUT
    };



#endif //PSEUDOSOFTWAREPWM_GPIODLINEDIRECTION_H
