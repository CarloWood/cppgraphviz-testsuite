include(AICxxProject)

add_executable(helloworld helloworld.cxx)
target_link_libraries(helloworld PRIVATE CppGraphviz::dot ${AICXX_OBJECTS_LIST})

add_executable(indexed_container_set_test indexed_container_set_test.cxx)
target_link_libraries(indexed_container_set_test PRIVATE AICxx::cppgraphviz CppGraphviz::dot ${AICXX_OBJECTS_LIST})

add_executable(class_members_test class_members_test.cxx)
target_link_libraries(class_members_test PRIVATE AICxx::cppgraphviz CppGraphviz::dot ${AICXX_OBJECTS_LIST})

add_executable(label_node_test label_node_test.cxx)
target_link_libraries(label_node_test PRIVATE AICxx::cppgraphviz CppGraphviz::dot ${AICXX_OBJECTS_LIST})

set(MACRO_VALUES "00" "01" "10" "11")
foreach(MACRO_VALUE IN LISTS MACRO_VALUES)
  string(SUBSTRING ${MACRO_VALUE} 0 1 UNLOCKED_TYPE_IS_TYPEDEF)
  string(SUBSTRING ${MACRO_VALUE} 1 1 TRACKER_IS_TYPEDEF)
  set(TARGET_NAME "test_tracked_object${MACRO_VALUE}")
  add_executable(${TARGET_NAME} test_tracked_object.cxx)
  target_link_libraries(${TARGET_NAME} PRIVATE AICxx::cppgraphviz CppGraphviz::dot ${AICXX_OBJECTS_LIST})
  target_compile_definitions(${TARGET_NAME}
    PRIVATE UNLOCKED_TYPE_IS_TYPEDEF=${UNLOCKED_TYPE_IS_TYPEDEF} TRACKER_IS_TYPEDEF=${TRACKER_IS_TYPEDEF})
endforeach()
