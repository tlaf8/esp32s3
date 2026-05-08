idf_component_register(
    SRCS
        "src/mutex.cpp"
        "src/queue.cpp"
        "src/task.cpp"
        "src/os.cpp"

    INCLUDE_DIRS
        "../cipher/include"

    REQUIRES
        freertos
)