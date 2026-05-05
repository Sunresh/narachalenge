cmake_minimum_required (VERSION 3.10)
project("nara")
add_executable (nara WIN32 "nara.cpp" )
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET nara PROPERTY CXX_STANDARD 20)
endif()
set(OpenCV_DIR "C:/OpenCV/opencv/build/x64/vc16/lib")

cmake_policy(SET CMP0074 NEW)
# Use wxWidgets_ROOT (no _DIR) for the standard Find module
set(wxWidgets_ROOT "C:/wxWidgets332")
set(wxWidgets_LIB_DIR "C:/wxWidgets332/lib/vc_x64_lib")
# Force CMake to look for the DLL/Shared versions
set(wxWidgets_USE_REL_AND_DBG ON)
set(wxWidgets_USE_STATIC OFF)

find_package(wxWidgets REQUIRED core base)

include_directories("C:/wxWidgets332/include")
# Add the include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})

find_package(OpenCV REQUIRED)
include_directories("C:/OpenCV/opencv/build/include")
link_directories("C:/OpenCV/opencv/build/x64/v16/lib")
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${OpenCV_DIR}/../bin/opencv_world4120.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

include(GNUInstallDirs)

# Install the main executable
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES "${OpenCV_DIR}/../bin/opencv_world4120.dll"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(CPACK_PACKAGE_NAME "NaraChallenge")
set(CPACK_PACKAGE_VENDOR "YourName")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Nara Project with OpenCV")
set(CPACK_PACKAGE_VERSION "1.0.0")

# Important: Some versions of CPack specifically look for this
set(CPACK_NSIS_PACKAGE_NAME "NaraChallenge") 

# Create a shortcut in the Start Menu
set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Nara.lnk' '$INSTDIR\\\\bin\\\\nara.exe'")

set(CPACK_GENERATOR "NSIS")

# THIS MUST BE THE LAST LINE
include(CPack)