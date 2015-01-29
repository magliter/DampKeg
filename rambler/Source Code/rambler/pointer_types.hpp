/**********************************************************************************************************************
 * @file    pointer_types.hpp
 * @date    2014-12-12
 * @brief   <# Brief Description#>
 * @details <#Detailed Description#>
 **********************************************************************************************************************/

#pragma once

#include <memory>
#include "rambler/memory/StrongPointer.hpp"

namespace rambler {

    
    template<typename T> using WeakPointer = std::weak_ptr<T>;

}