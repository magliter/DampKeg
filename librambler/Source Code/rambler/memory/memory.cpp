/**************************************************************************************************
 * @file    rambler/memory/memory.cpp
 * @date    2014-12-12
 * @author  Omar Stefan Evans
 * @brief   <# Brief Description #>
 * @details <# Detailed Description #>
 **************************************************************************************************/

#include "memory.hpp"

#include <cstdio>
#include <cstdlib>

#include <atomic>
#include <mutex>
#include <unordered_map>

#include "SafeInt3.hpp"

namespace rambler { namespace memory {

    class MemoryManager {
    public:
        static void initialize() {
            static bool initialized = false;
            if (initialized) {
                return;
            }

            memoryManager = new MemoryManager;
            initialized = true;
        }

        static void * allocate(UInt amount) {
            puts("Allocating!");

            void * ptr = calloc(amount, 1);

            if (ptr == nullptr) {
                return ptr;
            }

            memoryManager->mutex.lock();
            memoryManager->MemoryManager::referenceCount[ptr] = 1;
            memoryManager->mutex.unlock();

            return ptr;
        }

        static void deallocate(void * ptr) {
            /* NOTE: Replace lock/unlock with lock_shared/unlock_shared once shared_timed_mutex
             * becomes available.
             */
            puts("Deallocating!");

            memoryManager->mutex.lock();

            if (memoryManager->referenceCount.count(ptr) == 0) {
                memoryManager->mutex.unlock();
                return; // Consider throwing an exception instead.
            }

            memoryManager->MemoryManager::referenceCount.erase(ptr);

            memoryManager->mutex.unlock();

            free(ptr);
        }

        static void release(void * ptr) {
            /* NOTE: Replace lock/unlock with lock_shared/unlock_shared once shared_timed_mutex
             * becomes available.
             */

            puts("Releasing!");

            memoryManager->mutex.lock();

            if (memoryManager->referenceCount.count(ptr) == 0) {
                memoryManager->mutex.unlock();
                return; // Consider throwing an exception instead.
            }

            if (--memoryManager->referenceCount.at(ptr) == 0) {
                memoryManager->mutex.unlock();
                return deallocate(ptr);
            }

            memoryManager->mutex.unlock();
        }

        static void retain(void * ptr) {
            /* NOTE: Replace lock/unlock with lock_shared/unlock_shared once shared_timed_mutex
             * becomes available.
             */

            puts("Retaining!");

            memoryManager->mutex.lock();

            if (memoryManager->referenceCount.count(ptr) == 0) {
                memoryManager->mutex.unlock();
                return; // Consider throwing an exception instead.
            }

            ++memoryManager->referenceCount.at(ptr);
            memoryManager->mutex.unlock();
        }
    private:
        static MemoryManager * memoryManager;

        std::unordered_map<void *, std::atomic<UInt>> referenceCount;
        std::mutex mutex; /* Use shared_timed_mutex when it becomes available. */
    };

    MemoryManager * MemoryManager::memoryManager;

    void initialize() {
        static bool initialized = false;
        if (initialized) {
            return;
        }

        MemoryManager::initialize();

        initialized = true;
    }

    void * allocate(UInt count, UInt size) {
        UInt amount;

        if (!SafeMultiply(count, size, amount)) {
            return nullptr;
        }

        return allocateBytes(amount);
    }

    void * allocateBytes(UInt amount) {
        return MemoryManager::allocate(amount);
    }

    void deallocate(void * ptr) {
        MemoryManager::deallocate(ptr);
    }
    
    void release(void * ptr) {
        MemoryManager::release(ptr);
    }
    
    void retain(void * ptr) {
        MemoryManager::retain(ptr);
    }

}}
