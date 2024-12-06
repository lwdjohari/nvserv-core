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

set(CORE_FEATURE_DEFINITION)
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
add_library(${PROJECT_NAME}_core STATIC ${SOURCES_NVSERV_CORE} )
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
set_target_properties(${PROJECT_NAME}_core PROPERTIES LINKER_LANGUAGE CXX)
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
        add_subdirectory(src/tests)
    endif()  
endif()