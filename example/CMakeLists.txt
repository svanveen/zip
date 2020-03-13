add_executable(zip-example
    main.cpp
)

target_link_libraries(zip-example
    PRIVATE
        zip
)