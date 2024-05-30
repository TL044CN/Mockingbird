#include "MockingController.hpp"
#include "MockingGuard.hpp"

namespace MockingBird {

auto MockingController::getInstance() -> MockingController& {
    return Singleton<MockingController>::getInstance();
}

auto MockingController::getMockState(const std::string& name) -> bool& {
    auto& instance = getInstance();
    if( instance.mMockingState.contains(name) )
        return instance.mMockingState[name] = false;
    return instance.mMockingState[name];
}

auto MockingController::createMockGuard(const std::string& type, bool state) -> MockingGuard {
    return MockingGuard(type, state);
}

} // namespace MockingBird