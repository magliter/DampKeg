/**************************************************************************************************
 * @file    rambler/memory/memory.cpp
 * @date    2014-12-12
 * @author  Omar Stefan Evans
 * @brief   <# Brief Description #>
 * @details <# Detailed Description #>
 **************************************************************************************************/

#include "memory.hpp"

#include <cstdlib>

#include <atomic>
#include <mutex>
#include <iostream>
#include <unordered_map>

#include "SafeInt3.hpp"

namespace rambler { namespace memory {

    class MemoryManager {
    public:
        static void * allocate(UInt amount) {
            if (!defaultMemoryManager) {
                defaultMemoryManager = new MemoryManager;
            }

            void * ptr = calloc(amount, 1);

            if (ptr == nullptr) {
                return ptr;
            }

            defaultMemoryManager->mutex.lock();
            defaultMemoryManager->MemoryManager::referenceCount[ptr] = 1;
            defaultMemoryManager->mutex.unlock();

            std::cout << "Allocated " << ptr << "!\n";

            return ptr;
        }

        static void deallocate(void * ptr) {
            /* NOTE: Replace lock/unlock with lock_shared/unlock_shared once shared_timed_mutex
             * becomes available.
             */

            std::cout << "Deallocating " << ptr << "!\n";

            defaultMemoryManager->mutex.lock();

            if (defaultMemoryManager->referenceCount.count(ptr) == 0) {
                defaultMemoryManager->mutex.unlock();
                return; // Consider throwing an exception instead.
            }

            defaultMemoryManager->MemoryManager::referenceCount.erase(ptr);

            defaultMemoryManager->mutex.unlock();

            free(ptr);
        }

        static void release(void * ptr) {
            /* NOTE: Replace lock/unlock with lock_shared/unlock_shared once shared_timed_mutex
             * becomes available.
             */

            std::cout << "Releasing " << ptr << "!\n";

            defaultMemoryManager->mutex.lock();

            if (defaultMemoryManager->referenceCount.count(ptr) == 0) {
                defaultMemoryManager->mutex.unlock();
                return; // Consider throwing an exception instead.
            }

            if (--defaultMemoryManager->referenceCount.at(ptr) == 0) {
                defaultMemoryManager->mutex.unlock();
                return deallocate(ptr);
            }

            defaultMemoryManager->mutex.unlock();
        }

        static void retain(void * ptr) {
            /* NOTE: Replace lock/unlock with lock_shared/unlock_shared once shared_timed_mutex
             * becomes available.
             */

            std::cout << "Retaining " << ptr << "!\n";

            defaultMemoryManager->mutex.lock();

            if (defaultMemoryManager->referenceCount.count(ptr) == 0) {
                defaultMemoryManager->mutex.unlock();
                return; // Consider throwing an exception instead.
            }

            ++defaultMemoryManager->referenceCount.at(ptr);
            defaultMemoryManager->mutex.unlock();
        }
    private:
        static MemoryManager * defaultMemoryManager;

        std::unordered_map<void *, std::atomic<UInt>> referenceCount;
        std::mutex mutex; /* Use shared_timed_mutex when it becomes available. */
    };

    MemoryManager * MemoryManager::defaultMemoryManager;

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
