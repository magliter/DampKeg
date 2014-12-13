//
//  rambler.cpp
//  Rambler
//
//  Created by Omar Evans on 12/12/14.
//  Copyright (c) 2014 DampKeg. All rights reserved.
//

#include "rambler/rambler.hpp"
#include "rambler/memory/memory.hpp"
#include "rambler/XML/XML.hpp"

namespace rambler {

    void initialize() {
        static bool initialized = false;
        if (initialized) {
            return;
        }

        memory::initialize();
        XML::initialize();

        initialized = true;
    }
}