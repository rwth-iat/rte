# generic unix makefile

CXX_LIBKS_SOURCES = \
	array.cpp \
	avticket.cpp \
	event.cpp \
	from_local.cpp \
	manager.cpp \
	mask.cpp \
	objecttree.cpp \
	props.cpp \
	register.cpp \
	result.cpp \
	simpleserver.cpp \
	serviceparams.cpp \
	string.cpp \
	svrbase.cpp \
	svrobjects.cpp \
	svrsimpleobjects.cpp \
	templates.cpp \
	time.cpp \
	value.cpp \
	xdr.cpp

LIBKS_OBJECTS = \
	array$(O) \
	avticket$(O) \
	event$(O) \
	from_local$(O) \
	manager$(O) \
	mask$(O) \
	path$(O) \
	props$(O) \
	register$(O) \
	result$(O) \
	server$(O) \
	serviceparams$(O) \
	string$(O) \
	simpleserver$(O) \
	svrbase$(O) \
	svrobjects$(O) \
	svrsimpleobjects$(O) \
	templates$(O) \
	time$(O) \
	value$(O) \
	xdr$(O)


CXX_TEST_SOURCES = \
	tmanager.cpp \
	tmanager1.cpp \
	tserver.cpp \
	tserver1.cpp
	
#	thandle.cpp \
#	tpath.cpp \
#	tregister.cpp \
#	tstring.cpp \
#	txdr.cpp \
#	tsvrsimpleobjects.cpp \
#	txdr2.cpp
#	tvalue.cpp \
#	tvalue2.cpp \
#	tvalue3.cpp \
#	tserviceparams.cpp \
#	tserviceparams2.cpp \
#	tserviceparams3.cpp \
#	tserviceparams4.cpp \
#	tmanager2 \
#	tarray.cpp

TESTS = tmanager$(EXE) tserver$(EXE)

CXX_SOURCES = $(CXX_LIBKS_SOURCES) $(CXX_TEST_SOURCES)

all :	$(LIBKS)

tests :	$(TESTS)

$(LIBKS) : $(LIBKS_OBJECTS)

tmanager$(EXE) : tmanager$(O) tmanager1$(O) $(LIBKS)

tserver$(EXE) : tserver$(O) tserver1$(O) $(LIBKS)



