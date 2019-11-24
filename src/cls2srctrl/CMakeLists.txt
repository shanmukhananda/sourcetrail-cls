project(sourcetrail-cls.cls2srctrl)

find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
set(lib_list
    Boost::program_options
    sourcetraildb
    pthread
    dl
    stdc++fs
)

set(src_list
    main.cpp
    config.hpp
)
add_executable(${PROJECT_NAME} ${src_list})
target_link_libraries(${PROJECT_NAME} ${lib_list})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)
