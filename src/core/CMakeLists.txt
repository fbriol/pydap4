file(GLOB_RECURSE WRAPPED_SOURCES "*.cpp")
pybind11_add_module(core ${WRAPPED_SOURCES})
target_link_libraries(core PUBLIC ${LIBS})
install(TARGETS core DESTINATION lib/)