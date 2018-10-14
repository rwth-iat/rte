CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

# Define the library names
SET(ML_LIB libml)
SET(OV_LIB libov)
SET(FB_LIB fb)

# Get the directory for system libraries
SET(SYSLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../syslibs)

# Search directories of OV needed by all builds
SET(OV_DIR ${CMAKE_CURRENT_LIST_DIR}/../core/ov)
SET(OV_MODEL_DIR ${OV_DIR}/model)
SET(OV_INCLUDE_DIR ${OV_DIR}/include)
SET(OV_MODEL_FILE ${OV_MODEL_DIR}/ov.ovm)

# Set variables for ksbase reused along all libs
SET(KSBASE_ROOT_DIR ${SYSLIB_DIR}/comm/ksbase)
SET(KSBASE_INCLUDE_DIR ${KSBASE_ROOT_DIR}/include)
SET(KSBASE_MODEL_DIR ${KSBASE_ROOT_DIR}/model)
SET(KSBASE_MODEL_FILE ${KSBASE_MODEL_DIR}/ksbase.ovm)

# Search directories of FB needed by all builds
SET(FB_ROOT_DIR ${SYSLIB_DIR}/functionblock/fb)
SET(FB_MODEL_DIR ${FB_ROOT_DIR}/model)
SET(FB_INCLUDE_DIR ${FB_ROOT_DIR}/include)
SET(FB_MODEL_FILE ${FB_MODEL_DIR}/fb.ovm)

# Build communication client libraries
ADD_SUBDIRECTORY(commclient/ksapi)