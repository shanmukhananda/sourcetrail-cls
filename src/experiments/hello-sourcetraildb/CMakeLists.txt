project(sourcetrail-cls.experiments.hello-sourcetraildb)

set(src_list main.cpp)
find_package(Threads)
# ${CMAKE_DL_LIBS} has to link at the end
set(lib_list sourcetraildb Threads::Threads ${CMAKE_DL_LIBS})
add_executable(${PROJECT_NAME} ${src_list})
target_link_libraries(${PROJECT_NAME} ${lib_list})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS
                                                 OFF)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
