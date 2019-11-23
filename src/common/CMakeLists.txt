project(sourcetrail-cls.common)

set(hpp_list
    pch.hpp
)

set(cpp_list
    pch.cpp
)

set(file_list ${hpp_list} ${cpp_list})
add_custom_target(${PROJECT_NAME} SOURCES ${file_list})
