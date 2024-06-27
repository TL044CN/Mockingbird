#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_all.hpp"
#include "catch2/generators/catch_generators_all.hpp"

#define private public
#define protected public
#include "MockingController.hpp"
#include "MockingGuard.hpp"
#undef protected
#undef private

using namespace MockingBird;

SCENARIO("MockingController is tested", "[MockingController]") {
    GIVEN("An instance of MockingController") {
        MockingController& controller = MockingController::getInstance();

        WHEN("We get the mocking state for a syscall") {
            std::string syscall = "syscall";
            bool& state = MockingController::getMockState(syscall);

            THEN("Initially, the mocking state should be false") {
                REQUIRE(state == false);
            }

            AND_WHEN("We change the state") {
                state = true;

                THEN("The state should be true") {
                    REQUIRE(state == true);
                }
            }
        }

        WHEN("We get the mocking state for a syscall that does not exist") {
            std::string syscall = "nonexistent";
            bool& state = MockingController::getMockState(syscall);

            THEN("The state should be false") {
                REQUIRE(state == false);
            }
        }

        WHEN("We set the mocking state for a syscall") {
            std::string syscall = "mysyscall";
            bool state = true;
            MockingController::setMockState(syscall, state);

            THEN("The state should be true") {
                REQUIRE(MockingController::getMockState(syscall) == true);
            }
        }

        WHEN("We get the mocking state for a syscall that does not exist") {
            std::string syscall = "nonexistent";
            bool& state = MockingController::getMockState(syscall);

            THEN("The state should be false") {
                REQUIRE(state == false);
            }
        }

        WHEN("We set the mocking state for a syscall that does not exist") {
            std::string syscall = "nonexistent";
            bool state = true;
            MockingController::setMockState(syscall, state);

            THEN("The state should be the value we set") {
                REQUIRE(MockingController::getMockState(syscall) == state);
            }
        }

        WHEN("We reset the mocking state for a syscall") {
            std::string syscall = "syscall";
            MockingController::setMockState(syscall, true);
            MockingController::setMockState(syscall, false);

            THEN("The state should be false") {
                REQUIRE(MockingController::getMockState(syscall) == false);
            }
        }

        WHEN("We create a MockGuard") {
            std::string syscall = "MockSyscall";
            {
                auto guard = MockingController::createMockGuard(syscall);

                THEN("The state should be true") {
                    REQUIRE(MockingController::getMockState(syscall) == true);
                }

            }

            AND_WHEN("The guard goes out of scope") {
                THEN("The state should be false") {
                    REQUIRE(MockingController::getMockState(syscall) == false);
                }
            }
        }

        WHEN("We get the instance of MockingController") {
            MockingController& controller2 = MockingController::getInstance();

            THEN("The instances should be the same") {
                REQUIRE(&controller == &controller2);
            }
        }

    }
}