# verso - A simple C++ semantic versioning library

**verso** is a simple header-only C++ library for supporting semantic versioning in C++ projects. The library is written and requires C++ 20, so make sure your compiler supports it.

The library has one header file, `verso/verso.hpp`, just include it in your project and you're ready to go. The library does not have any third-party dependencies.

Enjoy!

## Getting verso

You can get verso by cloning this repository, making it a submodule in your project, or downloading the single header file.

### As a project submodule

If you want to include verso as a submodule in your project:

```bash
git submodule add https://github.com/WinterWind33/verso.git
git submodule update --init --recursive
```

After this, you can use CMake to include verso in your project:

```cmake
# You need to have CMake 3.16+
add_subdirectory(path/to/verso)
target_link_libraries(your_target PRIVATE verso)
```

And you're ready to go!

## Usage

Using verso is simple. After you've got the header file by following the above instructions, you can include it in your project:

```cpp
#include <verso/verso.hpp>

// your code...
```

## (Dev) Setting up the repository

If you want to contribute to the development of verso, you can clone the repository and set it up for development:

```bash
git clone https://github.com/WinterWind33/verso.git
cd verso
```

Using Visual Studio Code is highly recommended, you can find recommended extensions in the `.vscode/extensions.json` file.

After that, you need to use CMake to configure the project. You will need to also have tests put in place, so make sure to hit the following commands:

```bash
cmake -B build -DVERSO_BUILD_TESTS=ON # Required to build tests
cmake --build build
```

After that, you're ready to make your changes. You can run the tests by hitting:

```bash
# On Linux systems
./build/tests/verso-tests
```

```powershell
# On Windows systems
.\build\tests\Debug\verso-tests.exe
```

### Tests

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
