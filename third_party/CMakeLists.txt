cmake_minimum_required(VERSION 3.15)

include(FetchContent)

# --------------------------------------------------------------------

#set(FETCHCONTENT_FULLY_DISCONNECTED ON)
#set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

FetchContent_Declare(
        wheels
        GIT_REPOSITORY https://gitlab.com/Lipovsky/wheels.git
        GIT_TAG master
)
FetchContent_MakeAvailable(wheels)

# --------------------------------------------------------------------

FetchContent_Declare(
        context
        GIT_REPOSITORY https://gitlab.com/Lipovsky/context.git
        GIT_TAG master
)
FetchContent_MakeAvailable(context)
