/**
 * @file MockingGuard.hpp
 * @author TL044CN (tl044cn@shodan.fyi)
 * @brief MockingGuard class is a helper class that resets a boolean flag to its initial value when it goes out of scope
 * @version 0.1
 * @date 2024-05-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include "MockingController.hpp"

namespace MockingBird {

    /**
     * @brief MockingGuard is a helper class that resets a boolean flag to its
     *        initial value when it goes out of scope
     */
    class MockingController::MockingGuard {
    private:
        bool mInitialValue;
        std::string mType;
    public:
        /**
         * @brief MockingGuard is the constructor of the MockGuard class
         * @param type The type of the system call to mock
         * @param state The state of the mocking
         */
        explicit MockingGuard(const std::string& type, bool state = true);

        /**
         * @brief ~MockingGuard is the destructor of the MockingGuard class
         */
        ~MockingGuard();
    };


} // namespace MockingBird