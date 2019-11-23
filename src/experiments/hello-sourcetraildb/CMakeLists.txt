project(sourcetrail-cls.experiments.hello-sourcetraildb)

add_executable(${PROJECT_NAME} main.cpp)
set(lib_list
    sourcetraildb
    pthread
    dl
)
target_link_libraries(${PROJECT_NAME} ${lib_list})
