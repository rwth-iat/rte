CMAKE_MINIMUM_REQUIRED(VERSION 3.11)
PROJECT(acpltov)

OPTION(OV_BUILD_SYSLIBS "Determine if syslibs should get built" ON)

INCLUDE(FindFLEX)
INCLUDE(FindBISON)

SET(OV_GENERAL_DEFINES "")

#Determine the Platform defines for libks
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	IF (${CMAKE_CL_64})
		SET(BUILD_ARCH "x64")
		SET(CMAKE_CXX_FLAGS_DEBUG "-DPLT_DEBUG_NEW=0")
	ELSE()
		SET(BUILD_ARCH "x86")
    ENDIF()
    
    SET(FLEX_FLAGS -d)
    SET(BISON_FLAGS "-t -d")

    IF(${MSVC})
        # Replace the MD (dynamic linking of runtime)
        # with MT flag (static linking of runtime)
        # for all configurations
        set(CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS_MINSIZEREL
        )
        foreach(CompilerFlag ${CompilerFlags})
            string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach()    

        SET(OV_GENERAL_DEFINES ${OV_GENERAL_DEFINES} "-D_CRT_SECURE_NO_WARNINGS=1")        
    ENDIF()
    SET(OV_PLATFORM_DEFINES "-DOV_SYSTEM_NT=1")	
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Base Compiler Flags
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fno-implicit-templates")						
    IF (${BUILD_ARCH} STREQUAL "x86")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    ELSEIF(${BUILD_ARCH} STREQUAL "x64")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
        SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    ELSE()
        MESSAGE("Defaulting to x86 build. Specify architecture by defining BUILD_ARCH (possible values x86 and x64)")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
        SET(BUILD_ARCH "x86")
    ENDIF()

    # Define the system used
    SET(OV_PLATFORM_DEFINES "-DOV_SYSTEM_LINUX=1")

ELSE()
    MESSAGE(FATAL_ERROR "Unsupported platform")    
ENDIF()

SET(OV_DEFINES ${OV_GENERAL_DEFINES} ${OV_PLATFORM_DEFINES})
MESSAGE("OV Defines are: " ${OV_DEFINES})

# Core system build
ADD_SUBDIRECTORY(core/ov)

#Add system libs to build
IF(${OV_BUILD_SYSLIBS})
    MESSAGE("Including syslibs into build")
    ADD_SUBDIRECTORY(syslibs)
ENDIF()