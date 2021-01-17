cmake_minimum_required(VERSION 3.15)

include(FetchContent)

# --------------------------------------------------------------------

# Offline mode (uncomment next line to enable)
# set(FETCHCONTENT_FULLY_DISCONNECTED ON)

# --------------------------------------------------------------------

FetchContent_Declare(
        wheels
        GIT_REPOSITORY https://gitlab.com/Lipovsky/wheels.git
        GIT_TAG master
)

set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(wheels)

# --------------------------------------------------------------------

FetchContent_Declare(
        context
        GIT_REPOSITORY https://gitlab.com/Lipovsky/context.git
        GIT_TAG tiny
)

set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(context)
