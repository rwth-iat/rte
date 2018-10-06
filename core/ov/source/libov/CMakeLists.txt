CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

MESSAGE("Processing libov Source directory...")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR} ${OV_INCLUDE_DIR} ${OV_MODEL_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(OV_OVM ${OV_MODEL_DIR}/ov.ovm)
SET(LIBOV_MOD_SRC ${CMAKE_CURRENT_BINARY_DIR}/ov.c)

SET(LIBOV_SRC
	ov_association.c
	ov_childrelationship.c
	ov_class.c
	ov_construction.c
	ov_containment.c
	ov_database.c
	ov_debug.c
	ov_element.c
	ov_embedment.c
	ov_inheritance.c
	ov_instantiation.c
	ov_library.c
	ov_logfile.c
	ov_malloc.c
	ov_memstack.c
	ov_object.c
	ov_operation.c
	ov_parentrelationship.c
	ov_part.c
	ov_path.c
	ov_result.c
	ov_scheduler.c
	ov_string.c
	ov_structure.c
	ov_supervise.c
	ov_time.c
	ov_variable.c
	ov_vector.c
	ov_vendortree.c)

ADD_CUSTOM_COMMAND(OUTPUT ${LIBOV_MOD_SRC}
	COMMAND $<TARGET_FILE:ov_codegen> -I ${OV_MODEL_DIR} -f ${OV_OVM} -l ov
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS ov_codegen)

SET_SOURCE_FILES_PROPERTIES(${LIBOV_MOD_SRC} GENERATED)

ADD_LIBRARY(libov SHARED ${LIBOV_SRC} ${LIBOV_MOD_SRC})
TARGET_LINK_LIBRARIES(libov libml)
TARGET_COMPILE_DEFINITIONS(libov PRIVATE ${OV_DEFINES})

# Install the DLL into the syslibs directory
INSTALL(TARGETS libov
	RUNTIME DESTINATION ${SYSLIB_INSTALL_DIR})

# Install the model directory
INSTALL(FILES	${OV_MODEL_DIR}/ov.ovm
				${OV_MODEL_DIR}/ov.ovf
				${OV_MODEL_DIR}/ov.ovt
	DESTINATION ${DEV_INSTALL_DIR}/libov/model)

INSTALL(DIRECTORY ${OV_INCLUDE_DIR}/libov
	DESTINATION ${DEV_INSTALL_DIR}/libov/include)

INSTALL(TARGETS libov
	RUNTIME DESTINATION ${DEV_INSTALL_DIR}/libov/lib
	ARCHIVE DESTINATION ${DEV_INSTALL_DIR}/libov/lib)