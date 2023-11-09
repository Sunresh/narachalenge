# CMakeList.txt : CMake project for nara, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("nara")

# Add source to this project's executable.
add_executable (nara WIN32 "nara.cpp" "nara.h" "element.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET nara PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
set(OpenCV_DIR "C:/opencv/build")
find_package(OpenCV REQUIRED)

find_library(NIDAQmx_LIBRARY NIDAQmx PATHS "C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib64/msvc")
include_directories("C:/Program Files (x86)/National Instruments/NI-DAQ/DAQmx ANSI C Dev/include")
include_directories("C:/opencv/build/include")

link_directories("C:/opencv/build/x64/v16/lib")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${NIDAQmx_LIBRARY} )
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})