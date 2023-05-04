include(AICxxProject)

add_executable(helloworld helloworld.cxx)
target_link_libraries(helloworld PRIVATE ${AICXX_OBJECTS_LIST})
