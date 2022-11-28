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
        sure
        GIT_REPOSITORY https://gitlab.com/Lipovsky/sure.git
        GIT_TAG master
)
FetchContent_MakeAvailable(sure)
