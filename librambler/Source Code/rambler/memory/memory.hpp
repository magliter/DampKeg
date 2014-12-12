/**************************************************************************************************
 * @file    rambler/memory/memory.hpp
 * @date    2014-12-12
 * @author  Omar Stefan Evans
 * @brief   <# Brief Description #>
 * @details <# Detailed Description #>
 **************************************************************************************************/

#pragma once

#include "rambler/IntegerTypes.hpp"

namespace rambler { namespace memory {

    void * allocate(UInt count, UInt size);
    void * allocateBytes(UInt count);

    void deallocate(void * ptr);

    void release(void * ptr);
    void retain(void * ptr);
    
}}
