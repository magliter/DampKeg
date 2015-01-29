/**********************************************************************************************************************
 * @file    rambler/XMPP/IM/Presence.hpp
 * @date    2014-12-17
 * @brief   <# Brief Description #>
 * @details <# Detailed Description #>
 **********************************************************************************************************************/

#pragma once

#include "rambler/rambler.hpp"

namespace rambler { namespace XMPP { namespace IM {

    struct Presence {
        enum class State {
            Unavailable,
            Available,
            WantsToChat,
            DoNotDisturb,
            Away,
            ExtendedAway
        };

        State const state;
        String const message;

        Presence(State const state, String const message);

        static StrongPointer<Presence const> createWithState(State const state);
        static StrongPointer<Presence const> createWithStateAndMessage(State const state, String const message);

    };

}}}