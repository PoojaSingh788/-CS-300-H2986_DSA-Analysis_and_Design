# CMakeList.txt : CMake project for CourseList, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (CourseList "CourseList.cpp" "CourseList.h" "CSVparser.cpp" "CSVparser.hpp" "pc.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CourseList PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
