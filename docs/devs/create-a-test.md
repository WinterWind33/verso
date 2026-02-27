# Create a new verso test

The tests are written with a custom, very lightweight testing framework to avoid bringing in third-party dependencies and to keep the library as simple as possible. The tests are located in the `tests` folder.

You can find and create the tests in the `tests/verso` folder. The tests are run using the `verso-tests` executable, which is built when you configure the project with `-DVERSO_BUILD_TESTS=ON`.

In the `tests` folder you can find the basic runner framework:

- `test.hpp` - The main header file containing the `Test` class and the helper registration macros
- `runner-main.cpp` - The main file for the test runner executable `verso-tests`

To create a new test, just create a `.cpp` file in the `tests/verso` folder and include the `test.hpp` header. You can then create a new test class by inheriting from the `Test` class and implementing the `run()` method.

```cpp
#include "test.hpp"

class MyTest : public Test {
public:
    MyTest() : Test("My test name") {}

    void run() override {
        // You can use the custom assertion methods of the Test class here
        test_true("Assertion description", true); // Example assertion
    }
};
```

Right after your test class, you can automatically register the test using the `VERSO_REGISTER_TEST` macro:

```cpp
VERSO_REGISTER_TEST(MyTest)
```

After creating and registering your test, remember to register it with CTest by adding the following line to the `tests/CMakeLists.txt` file:

```cmake
# Remember to add -S to print the test name while it is executed even if it passes, otherwise
# CTest won't be able to detect the results of the test correctly.
add_test(NAME MyTest COMMAND verso-tests --filter "My test name" -S)
set_tests_properties(MyTest PROPERTIES
        TIMEOUT 10
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        PASS_REGULAR_EXPRESSION "\\[ OK \\]"
        FAIL_REGULAR_EXPRESSION "\\[ ERROR \\]"
    )
```

## Running the tests

To run the tests, simply execute the `verso-tests` binary:

```bash
# Run all tests
# -S prints the tests name while they are executed even if they pass
./verso-tests -S

# Run a specific test by name
./verso-tests -S --filter "My test name"
```

Or, you can run the tests using CTest:

```bash
cd build/tests

# Run all tests
ctest -C Debug
```
