cmake_minimum_required(VERSION 3.5)

cmake_policy(SET CMP0040 NEW)
cmake_policy(SET CMP0072 NEW)

file (STRINGS "VERSION" APP_VERSION)

project(TheEye VERSION "${APP_VERSION}" LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(OpenCV_STATIC ON)

find_package( OpenCV REQUIRED )
find_package( Eigen3 REQUIRED )
find_package( Iconv REQUIRED )

if (WIN32)
    find_package(glm CONFIG REQUIRED)
endif()

add_subdirectory(libs)

include_directories( ${OpenCV_INCLUDE_DIRS} )

###
### Options
###
# Unsed previous parameters from cache for the sake of dynamism
unset(KITGL_SUPPORT_GLFW CACHE)

# Enable desired libs
option(KITGL_SUPPORT_GLFW "Set ON/OFF to enable/disable GLFW support." ON)

option(USE_SYSTEM_DATA "Use system-wide data directory" OFF)

if(USE_SYSTEM_DATA)
    add_definitions(-DDATA_PATH="/usr/share/the-eye")
else()
    add_definitions(-DDATA_PATH="./data")
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DDEBUGGING)
endif()

###
### Initialization
###
# Project parameters
set(PROJ_LINK_LIBS)
set(PROJ_SRC)

# Add all files/folders under src folder automatically to the project
file(GLOB PROJ_SRC src/*.h src/*.cpp)
file(GLOB_RECURSE srcLinux src/linux/*.h src/linux/*.cpp)
file(GLOB_RECURSE srcWindows src/windows/*.h src/windows/*.cpp)

###
### Compiler settings
###
if(UNIX)
    add_definitions(
        -fPIC -fcommon
        )
endif()

###
### GLFW
###
if(KITGL_SUPPORT_GLFW)
    # Disable internal builds
    unset(GLFW_BUILD_EXAMPLES CACHE)
    unset(GLFW_BUILD_TESTS CACHE)
    unset(GLFW_BUILD_DOCS CACHE)
    option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
    option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
    option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
    include_directories(libs/glfw/include)
    include_directories(libs/glfw/deps)
    list(APPEND PROJ_LINK_LIBS glfw)
endif(KITGL_SUPPORT_GLFW)

if(UNIX)
    list(APPEND PROJ_LINK_LIBS Xinerama)
endif()

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${PROJ_SRC})
if (WIN32)
    message(STATUS "Adding Windows sources")
    target_sources(${PROJECT_NAME} PRIVATE ${srcWindows})
elseif(UNIX)
    message(STATUS "Adding Linux sources")
    target_sources(${PROJECT_NAME} PRIVATE ${srcLinux})
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE
    RenderLib
    ImageProcessingToolsLib
    ${OpenCV_LIBS}
    Iconv::Iconv
    ${PROJ_LINK_LIBS})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm-header-only GdiPlus)

    get_target_property(GLM_INCLUDE_DIR glm::glm-header-only INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "GLM include dir: ${GLM_INCLUDE_DIR}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIR})
endif()


file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(Other SOURCES
    VERSION LICENSE README.md
)

add_custom_target(BuildScripts SOURCES
    scripts/BuildTools.sh scripts/buildLinuxBinaries.sh  scripts/makeDebianPackage.sh
)

