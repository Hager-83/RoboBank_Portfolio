# CMake generated Testfile for 
# Source directory: D:/Graduation_Project/Projects_final/RoboBank _Portfolio
# Build directory: D:/Graduation_Project/Projects_final/RoboBank _Portfolio/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AllTests "D:/Graduation_Project/Projects_final/RoboBank _Portfolio/build/RoboBankTests.exe")
set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Graduation_Project/Projects_final/RoboBank _Portfolio/CMakeLists.txt;51;add_test;D:/Graduation_Project/Projects_final/RoboBank _Portfolio/CMakeLists.txt;0;")
subdirs("googletest")
