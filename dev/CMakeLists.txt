# Build executable targets

add_custom_target(tinyfibers_build_examples ALL DEPENDS
        tinyfibers_example_hello
        tinyfibers_example_mutex
        tinyfibers_example_sleep
        tinyfibers_example_stack
        tinyfibers_example_stackoverflow
        tinyfibers_example_yield)

add_custom_target(await_build_all ALL DEPENDS
        tinyfibers_play
        tinyfibers_build_examples
        tinyfibers_tests)
