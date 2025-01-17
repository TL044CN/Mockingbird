# Mocking Bird

[![Build Status](https://jenkins.shodan.fyi/buildStatus/icon?job=MockingBird)](https://jenkins.shodan.fyi/job/MockingBird/)
[![Coverage](https://sonarqube.shodan.fyi/api/project_badges/measure?project=TL044CN_Mockingbird_755fb003-552a-42f2-84ab-937b11b0a8e5&metric=coverage&token=sqb_02a14556b1f737aac708a3ae7f4860f74d90ca91)](https://sonarqube.shodan.fyi/dashboard?id=TL044CN_Mockingbird_755fb003-552a-42f2-84ab-937b11b0a8e5)
[![Lines of Code](https://sonarqube.shodan.fyi/api/project_badges/measure?project=TL044CN_Mockingbird_755fb003-552a-42f2-84ab-937b11b0a8e5&metric=ncloc&token=sqb_02a14556b1f737aac708a3ae7f4860f74d90ca91)](https://sonarqube.shodan.fyi/dashboard?id=TL044CN_Mockingbird_755fb003-552a-42f2-84ab-937b11b0a8e5)
[![Quality Gate Status](https://sonarqube.shodan.fyi/api/project_badges/measure?project=TL044CN_Mockingbird_755fb003-552a-42f2-84ab-937b11b0a8e5&metric=alert_status&token=sqb_02a14556b1f737aac708a3ae7f4860f74d90ca91)](https://sonarqube.shodan.fyi/dashboard?id=TL044CN_Mockingbird_755fb003-552a-42f2-84ab-937b11b0a8e5)

## Including Mockingbird using CMake

```cmake
# add the FetchContent system
include(FetchContent)

# declare Mockingbird to be fetched
# (replace the Tag behind the GIT_TAG with the correct version. Usually the latest)
FetchContent_Declare(
  MockingBird
  GIT_REPOSITORY https://github.com/TL044CN/Mockingbird.git
  GIT_TAG        1.0.4
)

# Declare the building of Mockingbird
FetchContent_GetProperties(MockingBird)
if(NOT mockingbird_POPULATED)
  FetchContent_Populate(MockingBird)
  add_subdirectory(${mockingbird_SOURCE_DIR} ${mockingbird_BINARY_DIR})
endif()

# do whatever...

# link Mockingbird
target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE 
        MockingBird
)

# this is an example for including it in a testing setup
```

## The Mocking Function

The Mocking Function is a template, that can select between calling 2 different functions based on the State of the Mocking Controller.
The template arguments have to match the functions that are passed to the MockingFunction class.

### `MockingFunction<Ret, Args...>`
- Ret     : This is the return value of the function to mock.
- Args... : These are all the Arguments that the function takes in.


## Mocking a Syscall

To mock a syscall, you have to create a MockingFunction instance for the given syscall.
```c++
// this should be done in a .cpp file.
// the example uses the "socket" function as an example
#include "MockingFunction.hpp"

// for convenience this can be declared to alias MockFunc to MockingFunction
template<typename... Args>
using MockFunc = MockingBird::MockingController::MockingFunction<Args...>;

// first create a shadow of the original socket function
int socket(int domain, int type, int protocol) {
    // the arguments for the MockingFunction template are <return type, argument types...>
    static MockFunc<int, int, int, int> mock_socket("socket", MockFunc<int, int, int, int>::fromSysycall("socekt"));
    // then return the static mocking function class. (the () operator is overloaded for calling the function)
    return mock_socket(domain, type, protocol);
}
```

## Mocking a Function, that is not a syscall

```c++
#include "MockingFunction.hpp"
// convenience alias
template<typename... Args>
using MockFunc = MockingBird::MockingController::MockingFunction<Args...>

// create a Mocking Function for switching between 2 functions
MockFunc<int, int>("MockFunc1",
	[](int x) -> int { return x; },
	[](int x) -> int { return x*2; }
);
```

The Mockingfunction is now available to the Mocking Controller as "MockFunc1"


## Selecting when to call the MockFunction instead of the real one

In a test scenario you can activate the mocking of a previously defined MockFunction
by activating the corresponding name in the MockingController:
```c++
// this should be done in the file that runs the test.
#include "MockingController.hpp"
using MockingBird::MockingController;

//activate mocking for the previously defined Socket function
MockingController::getMockState("socket") = true;

//deactivate mocking for the function
MockingController::getMockState("socket") = false;
```

You can also decide to activate a Mocking function in a given Scope:

```c++
// mocking state is unchanged here
{
    // create a guard here. it will have the value true by default, but that can be changed
    // by adding the new state after the name of the function to mock.
    auto guard = MockingController::createMockGuard("socket");
    // here the mocking state of "socket" will be true
}
// here the state of "socket" will be reset to the one it was before the guard was created
{
    auto guard = MockingController::createMockGuard("socket", false);
    // mocking will be set to false here.
}
// here the state is once again the same as before the scopes.
```
