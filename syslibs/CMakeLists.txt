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

# Build communication libraries
ADD_SUBDIRECTORY(comm/ksbase)