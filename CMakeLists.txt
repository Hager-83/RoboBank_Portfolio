# ⬅️ Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# ⬅️ Project name
project(RoboBankPortfolioProject)

# ⬅️ Set the C++ standard to use
set(CMAKE_CXX_STANDARD 17)

# ✅ Make Google Test use the same runtime library as the rest of the project
# This avoids linking issues on Windows (especially with Visual Studio)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# =========================
# 📌 Add Google Test to the project
# =========================
# ⬅️ Add the googletest folder as a subdirectory so CMake builds
# gtest and gtest_main libraries along with the project
add_subdirectory(googletest)

# =========================
# 📌 Main Project Executable
# =========================
# ⬅️ Define the main executable for the actual application
add_executable(RoboBank
    main.cpp
    cal.cpp
    portfolio.cpp
)

# =========================
# 📌 Google Test Setup
# =========================

# ⬅️ Enable testing support in CMake
enable_testing()

# ⬅️ Create a test executable that includes:
# - test.cpp (test file)
# - other project source files needed for the tests
add_executable(RoboBankTests
    test.cpp
    cal.cpp
    portfolio.cpp
)

# ⬅️ Link the test executable with Google Test libraries
target_link_libraries(RoboBankTests PRIVATE gtest gtest_main)

# ⬅️ Register the test with CTest so we can run it using `ctest`
add_test(NAME AllTests COMMAND RoboBankTests)
