#include "MockingGuard.hpp"

namespace MockingBird {

MockingController::MockingGuard::MockingGuard(const std::string& type, bool state)
    : mInitialValue(MockingController::getMockState(type)), mType(type) {
    MockingController::getMockState(type) = state;
}

MockingController::MockingGuard::~MockingGuard() {
    MockingController::getMockState(mType) = mInitialValue;
}

}   // namespace MockingBird