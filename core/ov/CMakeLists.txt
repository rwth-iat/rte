
add_ov_library(ov)
ov_library_includes(ov)
target_link_libraries(ov ovheaders)

if(UNIX)
    target_link_libraries(ov -ldl)
endif()

option(OV_VALGRIND "Compile OV with valgrind support" OFF)
if(OV_VALGRIND)
    target_compile_definitions(ov PRIVATE -DOV_VALGRIND=1)
endif()

# options for mutex selection
if(WIN32 OR CYGWIN)
    option(OV_NT_MUTEX "thread save database using windows mutex" ON)
    if(CYGWIN)
        option(OV_PTHREAD_MUTEX "thread save database using pthread mutex" OFF)
    endif()
elseif(UNIX)
    option(OV_PTHREAD_MUTEX "thread save database using pthread mutex" ON)
endif()

# set mutex defines
if (OV_PTHREAD_MUTEX)
    if(WIN32)
        message(SEND_ERROR "pthread not available for WIN32. use NT mutex instead")
    endif()
    if(OV_NT_MUTEX)
        message(WARNING "pthread mutex and nt mutex both active. using pthread mutex")
    endif()
    target_compile_options(ov PRIVATE -DOV_SYNC_PTHREAD=1)
elseif(OV_NT_MUTEX)
    if(UNIX)
        message(SEND_ERROR "NT mutex not available for UNIX. use pthread mutex instead")
    endif()
    target_compile_options(ov PRIVATE -DOV_SYNC_NTMUTEX)
endif ()
