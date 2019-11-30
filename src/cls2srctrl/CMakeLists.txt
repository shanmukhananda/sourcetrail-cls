project(sourcetrail-cls.cls2srctrl)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(Threads)

# ${CMAKE_DL_LIBS} has to link at the end
set(lib_list Boost::program_options sourcetraildb Threads::Threads
             ${CMAKE_DL_LIBS})
set(src_list
    types.cpp
    main.cpp
    types.hpp
    config.cpp
    config.hpp
    cls2srctrl.cpp
    cls2srctrl.hpp)
add_executable(${PROJECT_NAME} ${src_list})
target_link_libraries(${PROJECT_NAME} ${lib_list})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS
                                                 OFF)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
