# --------------------------------------------------------------------

get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)

add_library(${LIB_TARGET} STATIC
        rt/fiber.hpp
        rt/fiber.cpp
        rt/fwd.hpp
        rt/id.hpp
        rt/id_generator.hpp
        rt/parking_lot.hpp
        rt/parking_lot.cpp
        rt/routine.hpp
        rt/run.hpp
        rt/run.cpp
        rt/scheduler.hpp
        rt/scheduler.cpp
        rt/stack_allocator.hpp
        rt/stack_allocator.cpp
        rt/state.hpp
        rt/wait_queue.hpp
        rt/wait_queue.cpp
        rt/watcher.hpp
        sched/id.hpp
        sched/sleep_for.hpp
        sched/spawn.hpp
        sched/yield.hpp
        sched/syscalls.cpp
        sync/condvar.hpp
        sync/join_handle.hpp
        sync/join_handle.cpp
        sync/mutex.hpp
        sync/wait_group.hpp
        sync/wait_group.cpp
        run.hpp)

target_include_directories(${LIB_TARGET} PUBLIC ${LIB_INCLUDE_PATH})
target_link_libraries(${LIB_TARGET} PUBLIC fmt sure wheels)

# --------------------------------------------------------------------
