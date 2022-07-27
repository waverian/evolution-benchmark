add_executable(
        lfk-console
        main.c
        src/app.c
        src/utils/html_report_generator.c
        src/utils/text_report_generator.c
)

target_include_directories(
        lfk-console
        PRIVATE
            src/
)

target_link_libraries( lfk-console
        PRIVATE
        lfk-benchmark
)

if(MSVC)
        target_compile_options(lfk-console PRIVATE /MT)
endif()
