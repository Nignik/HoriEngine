file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Core/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Core/*.h")

find_package(HECS REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(HoriEngine STATIC ${SOURCES} ${HEADERS})

target_include_directories(HoriEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core>  # During the build
    $<INSTALL_INTERFACE:include/Core>  # After installation
)

# Install the CMake configuration for package finding
install(TARGETS HoriEngine EXPORT HoriEngineConfig 
        ARCHIVE DESTINATION lib           # For static libraries
        LIBRARY DESTINATION lib           # For shared libraries
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(EXPORT HoriEngineConfig
    FILE HoriEngineConfig.cmake
    NAMESPACE HoriEngine::
    DESTINATION lib/cmake/HoriEngine
)

# Install the header files to the "include" directory in the install location
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Core/ 
    DESTINATION include/Core FILES_MATCHING PATTERN "*.h"
)

target_link_libraries(HoriEngine PUBLIC glfw glad::glad glm::glm imgui::imgui HECS::HECS)

add_custom_command(
    TARGET HoriEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/HoriEngine/shaders
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/HoriEngine/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders
            ${CMAKE_BINARY_DIR}/HoriEngine/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            ${CMAKE_BINARY_DIR}/HoriEngine/resources
)