/**
 * @file MockingController.hpp
 * @author TL044CN (tl044cn@shodan.fyi)
 * @brief MockingController class is the main class for controlling the mocking of syscalls
 * @version 0.1
 * @date 2024-05-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "Singleton.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <string>

namespace MockingBird {

/**
 * @brief MockingController class is the main class for controlling the mocking of syscalls
 */
class MockingController : public Singleton<MockingController> {
    friend class Singleton<MockingController>;
public:
    template<typename Ret, typename... Args>
    class MockingFunction;

private:
    class MockingGuard;

    std::unordered_map<std::string, bool> mMockingState;

    MockingController() = default;
    MockingController(MockingController const&) = delete;
    void operator=(MockingController const&) = delete;

public:

    /**
     * @brief getInstance is a static function that returns the singleton
     *        instance of the MockingController
     * @return MockingController& The singleton instance of the MockingController
     */
    static auto getInstance() -> MockingController&;

    /**
     * @brief getMockState is a helper function that returns the mocking state
     *        of a system call
     * @param name The name of the system call
     * @return bool The mocking state of the system call
     */
    static auto getMockState(const std::string& name) -> bool&;

    /**
     * @brief createMockGuard is a helper function that creates a MockGuard
     *        The MockGuard object resets the mocking state to its initial
     *        value when it goes out of scope
     * @param type The type of the system call to mock
     * @param state The state of the mocking
     * @return MockGuard The MockGuard object
     */
    static auto createMockGuard(const std::string& type, bool state = true) -> MockingGuard;

    /**
     * @brief call is a helper function that calls the real or mock function
     *        based on the mocking state
     * @tparam Func The type of the function
     * @tparam Args The type of the arguments
     * @param name The name of the system call
     * @param realFunc The real function
     * @param mockFunc The mock function
     * @param args The arguments
     * @return auto The return value of the function
     */
    template<typename Func, typename... Args>
    static auto call(const std::string& name, Func realFunc, Func mockFunc, Args... args)
        -> decltype(realFunc()(args...)) {
        if ( getMockState(name) )
            return mockFunc()(args...);
        return realFunc()(args...);
    }
};

} // namespace MockingBird