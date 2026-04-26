idf_component_register(
    SRCS
        "src/Mutex.cpp"
        "src/Queue.cpp"
        "src/Task.cpp"
        "src/OS.cpp"

    INCLUDE_DIRS
        "../include"

    REQUIRES
        freertos
)