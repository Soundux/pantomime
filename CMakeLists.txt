cmake_minimum_required(VERSION 3.1)
project(pantomime LANGUAGES CXX VERSION 1.1)

# --------------------------------------------------------------------------------------------------------
# Library options
# --------------------------------------------------------------------------------------------------------

option(pantomime_fetcher "Build mime-type fetcher" ON)

# --------------------------------------------------------------------------------------------------------
# Create library
# --------------------------------------------------------------------------------------------------------

add_library(${PROJECT_NAME} "src/mime.cpp")
add_library(soundux::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wextra -pedantic)
endif()

# --------------------------------------------------------------------------------------------------------
# Include "include" folder
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PRIVATE "include/pantomime")

# --------------------------------------------------------------------------------------------------------
# Build Fetcher
# --------------------------------------------------------------------------------------------------------

if (pantomime_fetcher)
    add_executable(fetcher "src/fetcher.cpp")

    target_compile_features(fetcher PRIVATE cxx_std_17)
    set_target_properties(fetcher PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

    if (MSVC)
        target_compile_options(fetcher PRIVATE /W3 /WX)
    else()
        target_compile_options(fetcher PRIVATE -Wall -Werror -Wextra -pedantic)
    endif()

    include(FetchContent)
    
    FetchContent_Declare(json GIT_REPOSITORY "https://github.com/nlohmann/json" GIT_TAG v3.10.5 GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(json)

    target_link_libraries(fetcher PRIVATE nlohmann_json::nlohmann_json)
endif()

# --------------------------------------------------------------------------------------------------------
# Install Targets
# --------------------------------------------------------------------------------------------------------

install(DIRECTORY "include/" TYPE INCLUDE)
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib INCLUDES DESTINATION include)