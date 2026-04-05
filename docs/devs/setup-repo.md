# Setting up the repository for development

You can clone the repository and set it up for development:

```bash
git clone https://github.com/WinterWind33/verso.git
cd verso
```

Using Visual Studio Code is highly recommended, you can find recommended extensions in the `.vscode/extensions.json` file.

After that, you need to use CMake to configure the project. You will need to also have tests put in place, so make sure to hit the following commands:

```bash
# VERSO_BUILD_TESTS=ON is required to build the tests, while VERSO_BUILD_EXAMPLES=ON is optional and can be omitted if you don't want to build the example projects.
cmake -B build -DVERSO_BUILD_TESTS=ON -DVERSO_BUILD_EXAMPLES=ON
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
