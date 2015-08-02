file(GLOB whisk_SRC "[a-zA-Z]*.cpp" "simdag/[a-zA-Z]*.cpp")
add_executable(whisk ${whisk_SRC})

FIND_PACKAGE(Boost 1.54 COMPONENTS program_options log REQUIRED )
set(Boost_USE_MULTITHREADED      OFF)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

# set_property(TARGET whisk PROPERTY Boost_USE_MULTITHREADED OFF)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

set_property(TARGET whisk PROPERTY CXX_STANDARD 11)
set_property(TARGET whisk PROPERTY CXX_STANDARD_REQUIRED ON)

# We use pthreads for no particular reason, except that default boost
# distribution depends on the pthreads, and it is cumbersome to build
# own boost distribution.

find_package (Threads)

target_link_libraries(whisk
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_LOG_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
  simgrid
  )

install(TARGETS whisk DESTINATION bin)
