/**********************************************************************************************************************
 * @file    State.hpp
 * @date    2015-01-27
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#pragma once

namespace rambler { namespace Stream {

    enum State {
        NotOpen           = 0x00,
        Opening           = 0x01,
        Open              = 0x02,
        Reading           = 0x03,
        Writing           = 0x04,
        AtEnd             = 0x05,
        Closed            = 0x06,
        Error             = 0x07,

        OpenAndSecuring   = 0xF1,
        OpenAndSecured    = 0xF2,
        ReadingSecurely   = 0xF3,
        WritingSecurely   = 0xF4,

        OpenAndSecuredAndAuthenticated = 0xFF2
    };

}}