add_library(mdspan INTERFACE)

set_target_properties(mdspan PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(mdspan INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/mdspan/include/)


# target_link_libraries(mdspan zephyr_interface)
