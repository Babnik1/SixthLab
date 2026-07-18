cmake_minimum_required( VERSION 3.12 )

project( matrix VERSION 1.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( SOURCES 
        main.cpp 
)

add_executable( matrix ${SOURCES} )

set_target_properties( matrix PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )
