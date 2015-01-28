/**************************************************************************************************
 * @file    rambler/memory/MemoryManager.hpp
 * @date    2015-01-28
 * @author  Omar Stefan Evans
 **************************************************************************************************/

#pragma once

namespace rambler { namespace memory {

    class MemoryManager {
    public:
        static MemoryManager * default_manager();

		virtual ~MemoryManager() = default;

        /**
         * Reserves a region of shared memory large enough to hold n objects
         * that are size bytes each and returns a pointer to it. Every byte
         * will be set to zero.
         * @param  n the number of objects
         * @param  size the size of each object
         * @return a pointer to the reserved region of shared memory, or NULL if there is an error.
         * @post   on success: n * size bytes of shared memory will have been reserved;
         *         on failure: zero bytes of memory will have been on reserved.
         * @see    release_memory
         * @see    share_memory
         */
        virtual void * reserve_memory(size_t n, size_t size) = 0;

        /**
         * Reserves a region of shared memory large enough the amount of bytes
         * specified and returns a pointer to it. Every byte will be set to zero.
         * @param  amount the amount of memory to reserve
         * @return a pointer to the reserved region of shared memory, or NULL if there is an error.
         * @post   on success: amount bytes of shared memory will have been reserved;
         *         on failure: zero bytes of memory will have been on reserved.
         * @see    release_memory
         * @see    share_memory
         */
        virtual void * reserve_memory(size_t amount) = 0;

        /**
         * Releases the shared memory pointed to by ptr.
         * The memory will be freed if nothing else is sharing it.
         * @pre   ptr points to the begining of a region of shared memory
         * @param ptr the memory to release
         * @post  The memory pointed to by ptr will have been released,
         *        and freed if nothing else is sharing it.
         */
        virtual void release_memory(void * ptr) = 0;

        /**
         * Shares the memory pointed to by ptr.
         * @pre   ptr points to either the begining of a region of shared memory,
         *        or to the begining of a region of memory on which may be freed.
         * @param ptr the memory to share
         * @post  The memory pointed to by ptr will be shared.
         */
        virtual void share_memory(void * ptr) = 0;

    };
}}
