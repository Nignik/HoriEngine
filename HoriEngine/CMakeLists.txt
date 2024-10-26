file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

find_package(HECS REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(HoriEngine STATIC ${SOURCES})
target_include_directories(HoriEngine INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core>  # During the build
    $<INSTALL_INTERFACE:include/Core>  # After installation
)

# Install the header files to the "include" directory in the install location
install(DIRECTORY Core/ DESTINATION include/Core)

# Install the CMake configuration for package finding
install(TARGETS HoriEngine EXPORT HoriEngineConfig)

# Generate and install the CMake config file for find_package
install(EXPORT HoriEngineConfig
    FILE HoriEngineConfig.cmake
    NAMESPACE HoriEngine::
    DESTINATION lib/cmake/HoriEngine
)

target_link_libraries(HoriEngine PUBLIC glfw glad::glad glm::glm imgui::imgui HECS::HECS)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/HoriEngine/shaders
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/HoriEngine/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders
            ${CMAKE_BINARY_DIR}/HoriEngine/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            ${CMAKE_BINARY_DIR}/HoriEngine/resources
)