ovmparse.obj: ov_ovmparser.c ../../source/codegen/ov_codegen.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h

ovmscann.obj: ov_ovmscanner.c ../../source/codegen/ov_codegen.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ov_ovmparser.h

codegen.obj: ../../source/codegen/ov_codegen.c \
 ../../source/codegen/ov_codegen.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h

builder.obj: ../../source/builder/ov_builder.c \
 ../../source/builder/ov_builder.h \
 ../../source/builder/../codegen/ov_codegen.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h

ov.obj: ov.c ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

associat.obj: ../../source/libov/ov_association.c \
 ../../include/libov/ov_association.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

childrel.obj: ../../source/libov/ov_childrelationship.c \
 ../../include/libov/ov_association.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

class.obj: ../../source/libov/ov_class.c \
 ../../include/libov/ov_class.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h \
 ../../include/libov/ov_object.h ../../include/libov/ov_scheduler.h

construc.obj: ../../source/libov/ov_construction.c \
 ../../include/libov/ov_association.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

containm.obj: ../../source/libov/ov_containment.c \
 ../../include/libov/ov_association.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

database.obj: ../../source/libov/ov_database.c \
 ../../include/libov/ov_database.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_object.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h \
 ../../../libmpm/mp_malloc.h

debug.obj: ../../source/libov/ov_debug.c \
 ../../include/libov/ov_debug.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_logfile.h

element.obj: ../../source/libov/ov_element.c \
 ../../include/libov/ov_element.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

embedmen.obj: ../../source/libov/ov_embedment.c \
 ../../include/libov/ov_association.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

inherita.obj: ../../source/libov/ov_inheritance.c \
 ../../include/libov/ov_association.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

instanti.obj: ../../source/libov/ov_instantiation.c \
 ../../include/libov/ov_association.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

library.obj: ../../source/libov/ov_library.c \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

logfile.obj: ../../source/libov/ov_logfile.c \
 ../../include/libov/ov_logfile.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_time.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h

malloc.obj: ../../source/libov/ov_malloc.c \
 ../../include/libov/ov_malloc.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h

memstack.obj: ../../source/libov/ov_memstack.c \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_malloc.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h

object.obj: ../../source/libov/ov_object.c \
 ../../include/libov/ov_object.h ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_scheduler.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

operatio.obj: ../../source/libov/ov_operation.c \
 ../../include/libov/ov_operation.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_time.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

parentre.obj: ../../source/libov/ov_parentrelationship.c \
 ../../include/libov/ov_association.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

part.obj: ../../source/libov/ov_part.c ../../include/libov/ov_part.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

path.obj: ../../source/libov/ov_path.c ../../include/libov/ov_path.h \
 ov.h ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_time.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_malloc.h

result.obj: ../../source/libov/ov_result.c \
 ../../include/libov/ov_result.h ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h

schedule.obj: ../../source/libov/ov_scheduler.c \
 ../../include/libov/ov_scheduler.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h

string.obj: ../../source/libov/ov_string.c \
 ../../include/libov/ov_string.h ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h

structur.obj: ../../source/libov/ov_structure.c \
 ../../include/libov/ov_structure.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h

supervis.obj: ../../source/libov/ov_supervise.c \
 ../../include/libov/ov_supervise.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h

time.obj: ../../source/libov/ov_time.c ../../include/libov/ov_time.h \
 ov.h ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h

variable.obj: ../../source/libov/ov_variable.c \
 ../../include/libov/ov_variable.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_time.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

vector.obj: ../../source/libov/ov_vector.c \
 ../../include/libov/ov_vector.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_database.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

vendortr.obj: ../../source/libov/ov_vendortree.c \
 ../../include/libov/ov_vendortree.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h ../../include/libov/ov_path.h

dbutil.obj: ../../source/dbutil/ov_dbutil.c \
 ../../include/libov/ov_database.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_result.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h \
 ../../include/libov/ov_path.h

avticket.obj: ../../../ks/src/avticket.cpp \
 ../../../ks/include/ks/avticket.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/xdr.h \
 ../../../plt/include/plt/rtti.h ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../plt/include/plt/alloc.h

connecti.obj: ../../../ks/src/connection.cpp \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/xdr.h \
 ../../../plt/include/plt/rtti.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/rpc.h \
 ../../../ks/include/ks/rpcproto.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../plt/include/plt/alloc.h \
 ../../../ks/include/ks/result.h

connecti.obj: ../../../ks/src/connectionmgr.cpp \
 ../../../ks/include/ks/connectionmgr.h ../../../ks/include/ks/time.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/event.h ../../../ks/include/ks/connection.h \
 ../../../ks/include/ks/rpcproto.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../plt/include/plt/alloc.h \
 ../../../ks/include/ks/result.h

event.obj: ../../../ks/src/event.cpp ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../ks/include/ks/xdr.h \
 ../../../plt/include/plt/rtti.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/time.h

register.obj: ../../../ks/src/register.cpp \
 ../../../ks/include/ks/register.h ../../../ks/include/ks/xdr.h \
 ../../../plt/include/plt/rtti.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/rpc.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../plt/include/plt/alloc.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/result.h

result.obj: ../../../ks/src/result.cpp ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../../ks/include/ks/rpc.h ../../../ks/include/ks/ks.h

rpc.obj: ../../../ks/src/rpc.cpp ../../../plt/include/plt/config.h

rpcproto.obj: ../../../ks/src/rpcproto.cpp \
 ../../../ks/include/ks/rpcproto.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h

server.obj: ../../../ks/src/server.cpp ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../../ks/include/ks/avticket.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/serverconnection.h \
 ../../../ks/include/ks/xdrudpcon.h ../../../ks/include/ks/register.h \
 ../../../ks/include/ks/avmodule.h ../../../plt/include/plt/log.h

serverco.obj: ../../../ks/src/serverconnection.cpp \
 ../../../ks/include/ks/serverconnection.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../../ks/include/ks/avticket.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h ../../../ks/include/ks/xdrudpcon.h \
 ../../../ks/include/ks/register.h ../../../ks/include/ks/avmodule.h

stdconne.obj: ../../../ks/src/stdconnectionmgr.cpp \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../ks/include/ks/connectionmgr.h ../../../ks/include/ks/time.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/event.h ../../../ks/include/ks/connection.h \
 ../../../ks/include/ks/rpcproto.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../plt/include/plt/alloc.h \
 ../../../ks/include/ks/result.h ../../../plt/include/plt/log.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h

string.obj: ../../../ks/src/string.cpp ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../plt/include/plt/alloc.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/rpc.h

svrbase.obj: ../../../ks/src/svrbase.cpp \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/xdr.h \
 ../../../plt/include/plt/rtti.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/rpc.h \
 ../../../ks/include/ks/rpcproto.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../plt/include/plt/alloc.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../ks/include/ks/connectionmgr.h ../../../ks/include/ks/time.h \
 ../../../plt/include/plt/time.h ../../../ks/include/ks/event.h \
 ../../../plt/include/plt/log.h ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/xdrtcpcon.h

time.obj: ../../../ks/src/time.cpp ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/time.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/time.h

xdr.obj: ../../../ks/src/xdr.cpp ../../../ks/include/ks/xdr.h \
 ../../../plt/include/plt/rtti.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/rpc.h

xdrmemst.obj: ../../../ks/src/xdrmemstream.cpp \
 ../../../ks/include/ks/xdrmemstream.h ../../../ks/include/ks/xdr.h \
 ../../../plt/include/plt/rtti.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/rpc.h

xdrtcpco.obj: ../../../ks/src/xdrtcpcon.cpp \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/xdr.h \
 ../../../plt/include/plt/rtti.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/rpc.h \
 ../../../ks/include/ks/rpcproto.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../plt/include/plt/alloc.h \
 ../../../ks/include/ks/result.h ../../../ks/include/ks/xdrmemstream.h \
 ../../../ks/include/ks/connectionmgr.h ../../../ks/include/ks/time.h \
 ../../../plt/include/plt/time.h ../../../ks/include/ks/event.h

xdrudpco.obj: ../../../ks/src/xdrudpcon.cpp \
 ../../../ks/include/ks/xdrudpcon.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/xdr.h \
 ../../../plt/include/plt/rtti.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../../ks/include/ks/rpc.h \
 ../../../ks/include/ks/rpcproto.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../plt/include/plt/alloc.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h ../../../ks/include/ks/time.h \
 ../../../plt/include/plt/time.h ../../../ks/include/ks/event.h

ksclient.obj: ../../source/libovks/ov_ksclient.c \
 ../../include/libovks/ov_ksclient.h \
 ../../include/libovks/ov_ksclient_xdr.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../../ks/include/ks/serverconnection.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h ../../../ks/include/ks/xdrudpcon.h \
 ../../../ks/include/ks/register.h ../../../ks/include/ks/avmodule.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

ksclient.obj: ../../source/libovks/ov_ksclient_xdr.c \
 ../../include/libovks/ov_ksclient_xdr.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h

ksserver.obj: ../../source/libovks/ov_ksserver.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_object.h \
 ../../include/libov/ov_scheduler.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_time.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_malloc.h \
 ../../../plt/include/plt/priorityqueue_impl.h \
 ../../../plt/include/plt/container_impl.h

ksserver.obj: \
 ../../source/libovks/ov_ksserver_createobject.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: \
 ../../source/libovks/ov_ksserver_deleteobject.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: ../../source/libovks/ov_ksserver_exgdata.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: \
 ../../source/libovks/ov_ksserver_getcanonicalpath.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: ../../source/libovks/ov_ksserver_getep.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: ../../source/libovks/ov_ksserver_gethist.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: ../../source/libovks/ov_ksserver_getpp.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: ../../source/libovks/ov_ksserver_getvar.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: ../../source/libovks/ov_ksserver_link.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: \
 ../../source/libovks/ov_ksserver_renameobject.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: ../../source/libovks/ov_ksserver_setvar.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: ../../source/libovks/ov_ksserver_unlink.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../../plt/include/plt/log.h ../../../ks/include/ks/server.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../plt/include/plt/alloc.h ../../../ks/include/ks/event.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/connection.h ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h \
 ../../../ks/include/ks/xdrmemstream.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_malloc.h

ksserver.obj: ../../source/libovks/ov_ksserver_xdr.c \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_memstack.h

server.obj: ../../source/server/ov_server.c \
 ../../include/libovks/ov_ksserver.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_element.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_result.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_time.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_malloc.h

kshist.obj: kshist.c kshist.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

historyt.obj: \
 ../../source/kshist/historytrackcontainment.c kshist.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

istimetr.obj: ../../source/kshist/istimetrackof.c kshist.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

kshistor.obj: ../../source/kshist/kshistoryro.c kshist.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

kssimple.obj: ../../source/kshist/kssimplehistoryeo.c kshist.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

kssimple.obj: ../../source/kshist/kssimplehistoryro.c kshist.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

kssimple.obj: ../../source/kshist/kssimplehistorytrack.c \
 kshist.h ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

kssimple.obj: ../../source/kshist/kssimplemsgtrack.c kshist.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

kssimple.obj: ../../source/kshist/kssimplesingletrack.c \
 kshist.h ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

kssimple.obj: ../../source/kshist/kssimplestatetrack.c \
 kshist.h ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

kssimple.obj: ../../source/kshist/kssimpletimetrack.c kshist.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

kssimple.obj: ../../source/kshist/kssimplevaluetrack.c \
 kshist.h ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

ovmsglog.obj: ../../source/kshist/ovmsglog.c kshist.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

represen.obj: ../../source/kshist/representation.c kshist.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/kshist.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynov.obj: dynov.c dynov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

assocacc.obj: ../../source/dynov/assocaccess.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynassoc.obj: ../../source/dynov/dynassociation.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynchild.obj: ../../source/dynov/dynchildrelationship.c \
 dynov.h ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynclass.obj: ../../source/dynov/dynclass.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynconst.obj: ../../source/dynov/dynconstruction.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynembed.obj: ../../source/dynov/dynembedment.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dyninher.obj: ../../source/dynov/dyninheritance.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynobjec.obj: ../../source/dynov/dynobject.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h \
 ../../include/libov/ov_object.h

dynopera.obj: ../../source/dynov/dynoperation.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynparen.obj: ../../source/dynov/dynparentrelationship.c \
 dynov.h ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynpart.obj: ../../source/dynov/dynpart.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynstruc.obj: ../../source/dynov/dynstructure.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dynvaria.obj: ../../source/dynov/dynvariable.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

isgetacc.obj: ../../source/dynov/isgetaccessor.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

islinkfn.obj: ../../source/dynov/islinkfnc.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

issetacc.obj: ../../source/dynov/issetaccessor.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

isunlink.obj: ../../source/dynov/isunlinkfnc.c dynov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/dynov.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

tasklib.obj: tasklib.c tasklib.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/tasklib.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

Schedule.obj: ../../source/tasklib/SchedulerTask.c tasklib.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/tasklib.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h \
 ../../include/libov/ov_scheduler.h ../../include/libov/ov_supervise.h

Task.obj: ../../source/tasklib/Task.c tasklib.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/tasklib.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h \
 ../../include/libov/ov_scheduler.h ../../include/libov/ov_supervise.h

tasklist.obj: ../../source/tasklib/tasklist.c tasklib.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ../../model/tasklib.ovf \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

dbdump.obj: ../../source/dbdump/dbdump.cpp ../../source/dbdump/dbdump.h \
 ../../../ks/include/ks/client.h ../../../ks/include/ks/rpc.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/register.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/serviceparams.h ../../../ks/include/ks/props.h \
 ../../../ks/include/ks/handle.h ../../../ks/include/ks/value.h \
 ../../../ks/include/ks/array.h ../../../plt/include/plt/array.h \
 ../../../ks/include/ks/propsv1.h ../../../ks/include/ks/list.h \
 ../../../plt/include/plt/list.h ../../../ks/include/ks/avmodule.h \
 ../../../ks/include/ks/clnrequest.h \
 ../../../ks/include/ks/serverconnection.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/event.h ../../../ks/include/ks/connection.h \
 ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h ../../../ks/include/ks/xdrudpcon.h \
 ../../../ks/include/ks/clntpath.h ../../../ks/include/ks/commobject.h \
 ../../../ks/include/ks/stdconnectionmgr.h \
 ../../../plt/include/plt/log.h ../../../plt/include/plt/comparable.h \
 ../../../plt/include/plt/priorityqueue.h \
 ../../../ks/include/ks/xdrmemstream.h \
 ../../../plt/include/plt/hashtable_impl.h \
 ../../../plt/include/plt/container_impl.h \
 ../../../plt/include/plt/handle_impl.h \
 ../../../plt/include/plt/list_impl.h \
 ../../../plt/include/plt/priorityqueue_impl.h dbdump_inst.h \
 ../../source/dbdump/fnmatch.h ../../include/libov/ov_version.h

fnmatch.obj: ../../source/dbdump/fnmatch.c ../../source/dbdump/fnmatch.h

dbparse1.obj: ../../source/dbparse/dbparse1.cpp \
 ../../source/dbparse/dbparse.h ../../../ks/include/ks/commobject.h \
 ../../../ks/include/ks/props.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../ks/include/ks/string.h \
 ../../../plt/include/plt/string.h ../../../ks/include/ks/xdr.h \
 ../../../ks/include/ks/handle.h ../../../ks/include/ks/value.h \
 ../../../ks/include/ks/array.h ../../../plt/include/plt/array.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/propsv1.h ../../../ks/include/ks/clntpath.h \
 ../../../ks/include/ks/avmodule.h \
 ../../../ks/include/ks/avsimplemodule.h \
 ../../../ks/include/ks/client.h ../../../ks/include/ks/register.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/serviceparams.h ../../../ks/include/ks/list.h \
 ../../../ks/include/ks/clnrequest.h \
 ../../../ks/include/ks/serverconnection.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/event.h ../../../ks/include/ks/connection.h \
 ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h ../../../ks/include/ks/xdrudpcon.h \
 ../../../ks/include/ks/objmgrparams.h ../../../ks/include/ks/path.h \
 ../../include/libov/ov_version.h db_y.h

dbparse.obj: ../../source/dbparse/dbparse.cpp \
 ../../../ks/include/ks/array.h ../../../plt/include/plt/array.h \
 ../../../plt/include/plt/container.h \
 ../../../plt/include/plt/handle.h ../../../plt/include/plt/alloc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../../ks/include/ks/xdr.h ../../../plt/include/plt/rtti.h \
 ../../../ks/include/ks/rpc.h ../../../ks/include/ks/client.h \
 ../../../ks/include/ks/register.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../ks/include/ks/time.h ../../../plt/include/plt/time.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/serviceparams.h ../../../ks/include/ks/props.h \
 ../../../ks/include/ks/handle.h ../../../ks/include/ks/value.h \
 ../../../ks/include/ks/propsv1.h ../../../ks/include/ks/list.h \
 ../../../plt/include/plt/list.h ../../../ks/include/ks/avmodule.h \
 ../../../ks/include/ks/clnrequest.h \
 ../../../ks/include/ks/serverconnection.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/event.h ../../../ks/include/ks/connection.h \
 ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h ../../../ks/include/ks/xdrudpcon.h \
 ../../../ks/include/ks/clntpath.h ../../../ks/include/ks/commobject.h \
 ../../../ks/include/ks/objmgrparams.h ../../../ks/include/ks/path.h \
 ../../source/dbparse/dbparse.h \
 ../../../ks/include/ks/avsimplemodule.h \
 ../../../plt/include/plt/hashtable_impl.h \
 ../../../plt/include/plt/container_impl.h \
 ../../../plt/include/plt/handle_impl.h \
 ../../../ks/include/ks/array_impl.h \
 ../../../plt/include/plt/array_impl.h \
 ../../../plt/include/plt/list_impl.h \
 ../../../plt/include/plt/priorityqueue_impl.h \
 ../../../plt/include/plt/priorityqueue.h dbparse_inst.h

db_lex.obj: db_lex.c ../../source/dbparse/dbparse.h \
 ../../../ks/include/ks/commobject.h ../../../ks/include/ks/props.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../ks/include/ks/xdr.h ../../../ks/include/ks/handle.h \
 ../../../ks/include/ks/value.h ../../../ks/include/ks/array.h \
 ../../../plt/include/plt/array.h ../../../ks/include/ks/time.h \
 ../../../plt/include/plt/time.h ../../../ks/include/ks/propsv1.h \
 ../../../ks/include/ks/clntpath.h ../../../ks/include/ks/avmodule.h \
 ../../../ks/include/ks/avsimplemodule.h \
 ../../../ks/include/ks/client.h ../../../ks/include/ks/register.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/serviceparams.h ../../../ks/include/ks/list.h \
 ../../../ks/include/ks/clnrequest.h \
 ../../../ks/include/ks/serverconnection.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/event.h ../../../ks/include/ks/connection.h \
 ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h ../../../ks/include/ks/xdrudpcon.h \
 ../../../ks/include/ks/objmgrparams.h ../../../ks/include/ks/path.h \
 db_y.h

db_y.obj: db_y.c ../../source/dbparse/dbparse.h \
 ../../../ks/include/ks/commobject.h ../../../ks/include/ks/props.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../ks/include/ks/string.h ../../../plt/include/plt/string.h \
 ../../../ks/include/ks/xdr.h ../../../ks/include/ks/handle.h \
 ../../../ks/include/ks/value.h ../../../ks/include/ks/array.h \
 ../../../plt/include/plt/array.h ../../../ks/include/ks/time.h \
 ../../../plt/include/plt/time.h ../../../ks/include/ks/propsv1.h \
 ../../../ks/include/ks/clntpath.h ../../../ks/include/ks/avmodule.h \
 ../../../ks/include/ks/avsimplemodule.h \
 ../../../ks/include/ks/client.h ../../../ks/include/ks/register.h \
 ../../../ks/include/ks/result.h \
 ../../../ks/include/ks/serviceparams.h ../../../ks/include/ks/list.h \
 ../../../ks/include/ks/clnrequest.h \
 ../../../ks/include/ks/serverconnection.h \
 ../../../ks/include/ks/svrbase.h ../../../ks/include/ks/avticket.h \
 ../../../ks/include/ks/event.h ../../../ks/include/ks/connection.h \
 ../../../ks/include/ks/rpcproto.h \
 ../../../ks/include/ks/connectionmgr.h \
 ../../../ks/include/ks/xdrtcpcon.h ../../../ks/include/ks/xdrudpcon.h \
 ../../../ks/include/ks/objmgrparams.h ../../../ks/include/ks/path.h

makmak.obj: ../../source/makmak/ov_makmak.c \
 ../../include/libov/ov_version.h

libinfo.obj: ../../source/libinfo/ov_libinfo.c \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_string.h ../../include/libov/ov_vector.h \
 ../../include/libov/ov_database.h ../../include/libov/ov_vendortree.h \
 ../../include/libov/ov_memstack.h ../../include/libov/ov_malloc.h

expressi.obj: ov_expression_parser.c \
 ../../include/libov/ov_time.h ov.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ../../include/libov/ov_structure.h \
 ../../include/libov/ov_class.h ../../include/libov/ov_association.h \
 ../../model/ov.ovt ../../include/libov/ov_variable.h \
 ../../include/libov/ov_part.h ../../include/libov/ov_operation.h \
 ../../model/ov.ovf ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h ../../source/example/ov_expression.h

expressi.obj: ov_expression_scanner.c \
 ../../source/example/ov_expression.h ov.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h ov_expression_parser.h

example.obj: example.c example.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../source/example/example.ovt ../../include/libovks/ov_ksclient.h \
 ../../include/libovks/ov_ksclient_xdr.h \
 ../../source/example/ov_expression.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_time.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h

example_.obj: ../../source/example/example_fnc.c example.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../source/example/example.ovt ../../include/libovks/ov_ksclient.h \
 ../../include/libovks/ov_ksclient_xdr.h \
 ../../source/example/ov_expression.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_time.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h

example_.obj: ../../source/example/example_getvarclient.c \
 ../../source/example/example_getvarclient.h example.h \
 ../../include/libov/ov_library.h ../../include/libov/ov_ov.h \
 ../../include/libov/ov_config.h ../../include/libov/ov_version.h \
 ../../../ks/include/ks/ks.h ../../../ks/include/ks/rpc.h \
 ../../../plt/include/plt/debug.h ../../../plt/include/plt/config.h \
 ../../include/libov/ov_debug.h ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../source/example/example.ovt ../../include/libovks/ov_ksclient.h \
 ../../include/libovks/ov_ksclient_xdr.h \
 ../../source/example/ov_expression.h \
 ../../include/libov/ov_scheduler.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_result.h ../../include/libov/ov_path.h \
 ../../include/libov/ov_macros.h ../../include/libov/ov_time.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h

example_.obj: ../../source/example/example_testassoc.c \
 example.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../source/example/example.ovt ../../include/libovks/ov_ksclient.h \
 ../../include/libovks/ov_ksclient_xdr.h \
 ../../source/example/ov_expression.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_time.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h

example_.obj: ../../source/example/example_testclass.c \
 example.h ../../include/libov/ov_library.h \
 ../../include/libov/ov_ov.h ../../include/libov/ov_config.h \
 ../../include/libov/ov_version.h ../../../ks/include/ks/ks.h \
 ../../../ks/include/ks/rpc.h ../../../plt/include/plt/debug.h \
 ../../../plt/include/plt/config.h ../../include/libov/ov_debug.h \
 ../../include/libov/ov_logfile.h ov.h \
 ../../include/libov/ov_structure.h ../../include/libov/ov_class.h \
 ../../include/libov/ov_association.h ../../model/ov.ovt \
 ../../include/libov/ov_variable.h ../../include/libov/ov_part.h \
 ../../include/libov/ov_operation.h ../../model/ov.ovf \
 ../../include/libov/ov_element.h \
 ../../include/libovks/ov_ksserver_xdr.h \
 ../../source/example/example.ovt ../../include/libovks/ov_ksclient.h \
 ../../include/libovks/ov_ksclient_xdr.h \
 ../../source/example/ov_expression.h ../../include/libov/ov_macros.h \
 ../../include/libov/ov_time.h ../../include/libov/ov_string.h \
 ../../include/libov/ov_vector.h ../../include/libov/ov_database.h \
 ../../include/libov/ov_vendortree.h ../../include/libov/ov_memstack.h \
 ../../include/libov/ov_malloc.h

