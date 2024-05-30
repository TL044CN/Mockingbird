/**
 * @file MockingFunction.hpp
 * @author TL044CN (tl044cn@shodan.fyi)
 * @brief MockingFunction class is a class for mocking a function
 * @version 0.1
 * @date 2024-05-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "MockingController.hpp"

#include <dlfcn.h>
#include <cerrno>
#include <stdexcept>

namespace MockingBird {

/**
 * @brief MockingFunction is a class that mocks a function
 * @tparam Ret The return type of the function
 * @tparam Args The arguments of the function
 * @note This class is used to mock system calls. It copies the real function
 */
template<typename Ret, typename... Args>
class MockingController::MockingFunction {
private:
    std::string mName;
    std::function<Ret(Args...)> mRealFunc;
    std::function<Ret(Args...)> mMockFunc;
public:
    /**
     * @brief MockingFunction is the constructor of the MockingFunction class
     * @param name The name of the function
     * @param realFunc The real function
     * @param mockFunc The mock function
     */
    MockingFunction(const std::string& name, Ret(*realFunc)(Args... args), Ret(*mockFunc)(Args...))
        : mName(name), mMockFunc(mockFunc) {
        mRealFunc = reinterpret_cast<Ret(*)(Args...)>(dlsym(RTLD_NEXT, name.c_str()));
        if ( !mRealFunc ) throw std::runtime_error("Failed to load real function");
    }

    /**
     * @brief MockingFunction is the constructor of the MockingFunction class
     * @param name The name of the function
     * @param realFunc The real function
     * @param mockFunc The mock function
     */
    MockingFunction(const std::string& name, Ret(*realFunc)(Args... args))
        : MockingFunction(name, realFunc, [](Args... args) -> Ret {
        errno = EOPNOTSUPP;
        return -1;
        }) {}

    /**
     * @brief operator() is the function call operator of the MockingFunction class
     * @param args The arguments
     * @return Ret The return value of the function
     */
    Ret operator()(Args... args) {
        if ( MockingController::getMockState(mName) )
            return mMockFunc(args...);
        return mRealFunc(args...);
    }

    /**
     * @brief real is a helper function that returns the real function
     * @return std::function<Ret(Args...)> The real function
     */
    std::function<Ret(Args...)> real() {
        return mRealFunc;
    }

    /**
     * @brief mock is a helper function that returns the mock function
     * @return std::function<Ret(Args...)> The mock function
     */
    std::function<Ret(Args...)> mock() {
        return mMockFunc;
    }
};

} // namespace MockingBird