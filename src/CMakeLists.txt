##
## Agartha-Software, 2023
## Cevy
## File description:
## cevy-cmake
##

include_directories(".")

add_subdirectory(ecs)
add_subdirectory(network)
add_subdirectory(engine)

## IN DEVELOPPEMENT:
## add_subdirectory(physics)

file(GLOB CEVY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../public/*")
add_library(cevy SHARED ${CEVY_HEADERS})
target_link_libraries(cevy ecs engine) # physics)
set_target_properties(cevy PROPERTIES LINKER_LANGUAGE CXX)

if(DEBUG_MODE)
    target_compile_definitions(engine
        INTERFACE DEBUG
    )
    target_compile_definitions(ecs
       INTERFACE DEBUG
    )
    ## target_compile_definitions(physics
    ##     INTERFACE DEBUG
    ## )
endif(DEBUG_MODE)
