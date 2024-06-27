#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_all.hpp"
#include "catch2/generators/catch_generators_all.hpp"

#define private public
#define protected public
#include "MockingController.hpp"
#include "MockingFunction.hpp"
#undef protected
#undef private

template<typename... Args>
using MockingFunction = MockingBird::MockingController::MockingFunction<Args...>;

using MockingBird::MockingController;

SCENARIO("MockingFunction is tested", "[MockingFunction]") {

    GIVEN("An instance of MockingFunction") {
        MockingFunction<int, int> func("func",
            [](int x) -> int { return x; },
            [](int x) -> int { return 2 * x; }
        );

        WHEN("We call the real function") {
            int result = func.real()(5);

            THEN("The result should be 5") {
                REQUIRE(result == 5);
            }
        }

        WHEN("We call the mock function") {
            int result = func.mock()(5);

            THEN("The result should be 10") {
                REQUIRE(result == 10);
            }
        }

        WHEN("We call the function") {
            int result = func(5);

            THEN("The result should be 5") {
                REQUIRE(result == 5);
            }

            AND_WHEN("We enable the mocking") {
                MockingController::setMockState("func", true);
                result = func(5);
                THEN("The result should be 10") {
                    REQUIRE(result == 10);
                }
            }
        }

        WHEN("We want to mock a syscall"){
            MockingFunction<int, const char*, int> mock_open("open", MockingFunction<int, const char*, int>::fromSyscall("open"));

            THEN("The result should be -1 and the errno should be ENOENT") {
                int result = mock_open("file.txt", 0);
                REQUIRE(errno == ENOENT);
                REQUIRE(result == -1);
            }

            AND_WHEN("We enable the mocking") {
                MockingController::setMockState("open", true);
                int result = mock_open("file.txt", 0);

                THEN("The result should be -1 and the errno should be EOPNOTSUPP") {
                    REQUIRE(result == -1);
                    REQUIRE(errno == EOPNOTSUPP);
                }
            }
        }

    }
}