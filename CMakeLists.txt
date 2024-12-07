cmake_minimum_required(VERSION 3.10)
project(nvserv CXX)

message(STATUS "NvServ Core: Configure")
message(STATUS "-----------------------")
#Main headers and sources
file(GLOB_RECURSE SOURCES_NVSERV_CORE CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.s
)

if(NOT DEFINED NVSERV_LIBRARY_VERSION OR NOT DEFINED NVSERV_LIBRARY_SOVERSION)
    message(FATAL_ERROR "nvserv::core configuration ERROR:
      please set option NVSERV_LIBRARY_VERSION or NVSERV_LIBRARY_SOVERSION!")
endif()

if(NOT NVSERV_BUILD_LOCAL_SHARED AND NOT NVSERV_BUILD_STATIC)
    message(FATAL_ERROR "nvserv::core configuration ERROR:
      please set option NVSERV_BUILD_STATIC or NVSERV_BUILD_LOCAL_SHARED!")
endif()

set(_LIBRARY_TYPE SHARED STATIC)
set(CORE_FEATURE_DEFINITION)

if(NVSERV_BUILD_LOCAL_SHARED)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    LIST(APPEND CORE_FEATURE_DEFINITION NVSERV_BUILD_LOCAL_SHARED=1)
endif()


if(NOT nvcore_FOUND)
    message(STATUS "Nvql lib nvcore: Initialize NvCore lib.")
    add_subdirectory(deps/nvcore/ build-nvcore)
else()
    message(STATUS "Nvql lib nvcore: nvcore lib are already set, use nvcore from project.")
endif()

if(TARGET ryml)
    message(STATUS "rapidyaml lib: RapidYaml lib are already set, use ryml from project.")
else()
    message(STATUS "rapidyaml lib: initialize rapidyaml lib.")
    add_subdirectory(deps/rapidyaml/ build-rapidyaml)
endif()

if(TARGET RapidJSON)
    message(STATUS "RapidJSON lib: RapidJSON lib are already set, use RapidJSON from project.")
else()
    message(STATUS "RapidJSON lib: initialize RapidJSON lib.")
    set(RAPIDJSON_BUILD_CXX11 ON)
    set(RAPIDJSON_BUILD_DOC  OFF)
    set(RAPIDJSON_BUILD_EXAMPLES  OFF)
    set(RAPIDJSON_BUILD_TESTS  OFF)
    set(RAPIDJSON_BUILD_THIRDPARTY_GTEST OFF)
    add_subdirectory(deps/rapidjson/ build-rapidjson)
endif()

if (TARGET absl::base)
    message(STATUS "Nvql lib ABSL: Abseil lib are already set, use ABSL from project.")
else ()
    message(STATUS "Nvql lib ABSL: initialize Abseil lib.")
    set(ABSL_PROPAGATE_CXX_STD ON)
    add_subdirectory(deps/abseil-lts20230125.1 build-abseil)
endif ()

if(NVSERV_SERVER_REST)
    LIST(APPEND CORE_FEATURE_DEFINITION NVSERV_SERVER_REST=1)
endif()

if(NVSERV_SERVER_GRPC)
    LIST(APPEND CORE_FEATURE_DEFINITION NVSERV_SERVER_GRPC=1)
endif()

# Convert the list to a comma-separated string
string(JOIN ", " CORE_FEATURES_JOIN ${CORE_FEATURE_DEFINITION})

# Print the comma-separated list
message(STATUS "Compile Flags: ${CORE_FEATURES_JOIN}")


# nvserv::core
add_library(${PROJECT_NAME}_core ${_LIBRARY_TYPE} ${SOURCES_NVSERV_CORE} )

# Set runtime path for the shared library
if(NVSERV_BUILD_LOCAL_SHARED)
    set_target_properties(${PROJECT_NAME}_core PROPERTIES
        LINKER_LANGUAGE CXX
        BUILD_RPATH "$ORIGIN"                   # Use the directory of the binary
        INSTALL_RPATH "$ORIGIN"                 # Set for installed binaries too
        BUILD_WITH_INSTALL_RPATH TRUE           # Ensure RPATH is embedded during the build
        VERSION ${NVSERV_LIBRARY_VERSION}       # Semantic Version of library
        SOVERSION ${NVSERV_LIBRARY_SOVERSION}   # Linker version for sysmlink
    )
endif()

if(NVSERV_BUILD_STATIC)
    set_target_properties(${PROJECT_NAME}_core PROPERTIES
        LINKER_LANGUAGE CXX
    )
endif()
target_link_libraries(${PROJECT_NAME}_core  
    PUBLIC
        nvcore 
        ryml
        RapidJSON
        absl::base 
        absl::base 
        absl::flat_hash_map 
        absl::algorithm
        absl::numeric
        absl::random_random
        absl::synchronization 
        absl::strings
        absl::time
        absl::utility
        absl::status
    )

# Ensure dependencies are built
add_dependencies(${PROJECT_NAME}_core nvcore ryml RapidJSON absl::base)
target_compile_features(${PROJECT_NAME}_core PUBLIC ${CXX_FEATURE})
target_compile_definitions(${PROJECT_NAME}_core PRIVATE ${CORE_FEATURE_DEFINITION})

target_include_directories(${PROJECT_NAME}_core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

add_library(${PROJECT_NAME}::core ALIAS ${PROJECT_NAME}_core )

if(NOT TARGET nvserv::core )
    message(STATUS "Unit-Test nvserv::core: ${NVSERV_USE_TEST}")
    if(NVSERV_USE_TEST)
        add_subdirectory(tests)
    endif()  
endif()