include(AICxxProject)

add_executable(helloworld helloworld.cxx)
target_link_libraries(helloworld PRIVATE libcppgraphviz ${AICXX_OBJECTS_LIST})

add_executable(indexed_container_set_test indexed_container_set_test.cxx)
target_link_libraries(indexed_container_set_test PRIVATE libcppgraphviz ${AICXX_OBJECTS_LIST})

#add_executable(class_members_test class_members_test.cxx)
#target_link_libraries(class_members_test PRIVATE libcppgraphviz ${AICXX_OBJECTS_LIST})

add_executable(label_node_test label_node_test.cxx)
target_link_libraries(label_node_test PRIVATE libcppgraphviz ${AICXX_OBJECTS_LIST})

add_subdirectory(cppgraphviz)
