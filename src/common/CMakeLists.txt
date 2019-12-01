project(sourcetrail-cls.common)
set(src_list pch.cpp pch.hpp)
add_library(${PROJECT_NAME} ${src_list})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS
                                                 OFF)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
