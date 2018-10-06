CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

# Define the library names
SET(ML_LIB libml)
SET(OV_LIB libov)
SET(FB_LIB fb)

# Search directories of OV needed by all builds
SET(OV_DIR ${CMAKE_CURRENT_LIST_DIR}/../core/ov)
SET(OV_MODEL_DIR ${OV_DIR}/model)
SET(OV_INCLUDE_DIR ${OV_DIR}/include)

SET(OV_MODEL_FILE ${OV_MODEL_DIR}/ov.ovm)

# Set variables for ksbase reused along all libs
SET(KSBASE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/comm/ksbase)
SET(KSBASE_INCLUDE_DIR ${KSBASE_ROOT_DIR}/include)
SET(KSBASE_MODEL_DIR ${KSBASE_ROOT_DIR}/model)
SET(KSBASE_MODEL_FILE ${KSBASE_MODEL_DIR}/ksbase.ovm)

# Build communication libraries
ADD_SUBDIRECTORY(comm/ksbase)
ADD_SUBDIRECTORY(comm/TCPbind)