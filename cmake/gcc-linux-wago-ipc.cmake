
set(CMAKE_SYSTEM_NAME Linux)
set(TOOLCHAIN_PREFIX i586-unknown-linux-gnu)
set(CMAKE_SYSTEM_PROCESSOR x86)
set(OV_ARCH_i386 TRUE)

# recommended toolchain for wago IPC is OSELAS 2011.03.2 for i586 linux
set(TOOLCHAIN_PATH /opt/OSELAS.Toolchain-2011.03.2/i586-unknown-linux-gnu/gcc-4.5.2-glibc-2.13-binutils-2.21.1-kernel-2.6.36-sanitized)

# cross compilers to use for C, C++ and Fortran
set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/bin/${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PATH}/bin/${TOOLCHAIN_PREFIX}-gfortran)

# target environment on the build host system
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PATH})

# modify default behavior of FIND_XXX() commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# enable build targets
set(BUILD_OV_LIBRARY_wagoIPClib ON CACHE BOOL "" FORCE)
set(BUILD_OV_LIBRARY_wagoFBKlib ON CACHE BOOL "" FORCE)
