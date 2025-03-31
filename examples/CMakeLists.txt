# Examples CMakeLists.txt

# Copy example files to the build directory
file(GLOB EXAMPLE_FILES "*.ar")
foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
    get_filename_component(FILENAME ${EXAMPLE_FILE} NAME)
    configure_file(${EXAMPLE_FILE} ${CMAKE_BINARY_DIR}/examples/${FILENAME} COPYONLY)
endforeach()

# Add a custom target to run examples
add_custom_target(run_examples
    COMMAND ${CMAKE_COMMAND} -E echo "Running Argon examples..."
    DEPENDS argon
)

# Add custom commands for each example
foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
    get_filename_component(FILENAME ${EXAMPLE_FILE} NAME)
    get_filename_component(BASENAME ${EXAMPLE_FILE} NAME_WE)
    
    add_custom_target(run_example_${BASENAME}
        COMMAND ${CMAKE_COMMAND} -E echo "Running example: ${FILENAME}"
        COMMAND $<TARGET_FILE:argon> ${CMAKE_BINARY_DIR}/examples/${FILENAME}
        DEPENDS argon
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    add_dependencies(run_examples run_example_${BASENAME})
endforeach()
