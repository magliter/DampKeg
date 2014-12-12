/**************************************************************************************************
 * @file    rambler/memory/RCAllocator.hpp
 * @date    2014-12-12
 * @author  Omar Stefan Evans
 * @brief   <# Brief Description #>
 * @details <# Detailed Description #>
 **************************************************************************************************/

#pragma once

#include <limits>
#include "memory.hpp"

namespace rambler { namespace memory {

    template <typename T>
    class RCAllocator {
    public:
        using value_type        = T;
        using pointer           = value_type *;
        using const_pointer     = const pointer;
        using reference         = value_type &;
        using const_reference   = const reference;
        using size_type         = UInt;
        using difference_type   = Int;
        template< class U > struct rebind { typedef RCAllocator<U> other; };


        RCAllocator() = default;
        RCAllocator(const RCAllocator& other) = default;

        template< class U >
        RCAllocator(const RCAllocator<U>& other) {}

        pointer allocate(size_type n, std::allocator<void>::const_pointer hint = 0) {
            return static_cast<pointer>(memory::allocate(n, sizeof(value_type)));
        }

        void deallocate(pointer p, size_type n) {
            memory::release(p);
        }

        size_type max_size() const {
            return std::numeric_limits<size_type>::max();
        }

        template< class U, class... Args >
        void construct( U* p, Args&&... args ) {
            ::new((void *)p) U(std::forward<Args>(args)...);
        }
        
        template< class U >
        void destroy( U* p ) {
            p->~U();
        }
    };

}}
