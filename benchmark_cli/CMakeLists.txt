add_executable(
        benchmark-cli
        main.c
        src/app.c
)

target_include_directories(
        benchmark-cli
        PRIVATE
        src/
)

target_link_libraries(benchmark-cli
        PRIVATE
        benchmark
)

if (MSVC)
    target_compile_options(benchmark-cli PRIVATE /MT)
endif ()
