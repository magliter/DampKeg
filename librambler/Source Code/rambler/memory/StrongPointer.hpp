/**************************************************************************************************
 * @file    rambler/memory/StrongPointer.hpp
 * @date    2014-12-12
 * @author  Omar Stefan Evans
 * @brief   <# Brief Description #>
 * @details <# Detailed Description #>
 **************************************************************************************************/

#pragma once

#include <memory>
#include "rambler/memory/RCAllocator.hpp"

namespace rambler {
    namespace memory {
        template<typename T> using StrongPointer = std::shared_ptr<T>;

        template<typename T, typename... Args>
        StrongPointer<T> makeStrongPointer(Args... args) {
            return std::allocate_shared<T>(RCAllocator<T>(), args...);
        }
    }

    using memory::StrongPointer;
    using memory::makeStrongPointer;
}
