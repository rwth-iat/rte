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

LIBKS_OBJECTS1 = \
	array$(O) \
	avticket$(O) \
	event$(O) \
	from_local$(O) \
	manager$(O) \
	mask$(O)

LIBKS_OBJECTS2 = \
	path$(O) \
	props$(O) \
	register$(O) \
	result$(O) \
	rpc$(O) \
	server$(O) \
	serviceparams$(O)

LIBKS_OBJECTS3 = \
	string$(O) \
	simpleserver$(O) \
	svrbase$(O) \
	svrobjects$(O)

LIBKS_OBJECTS4 = \
	svrsimpleobjects$(O) \
	svrrpcctx$(O) \
	templates$(O) \
	time$(O) \
	value$(O) \
	xdr$(O)

LIBKS_OBJECTS = $(LIBKS_OBJECTS1) $(LIBKS_OBJECTS2) $(LIBKS_OBJECTS3) \
	$(LIBKS_OBJECTS4)


CXX_EXAMPLES_SOURCES = \
	tmanager.cpp \
	tmanager1.cpp \
	tsclient.cpp \
	tsclient1.cpp \
	tserver.cpp \
	tserver1.cpp

TESTS = tmanager$(EXE) tserver$(EXE) tsclient$(EXE)

CXX_SOURCES = $(CXX_LIBKS_SOURCES) $(CXX_EXAMPLES_SOURCES)

all :	$(LIBKS)

tests :	$(TESTS)

$(LIBKS) : $(LIBKS_OBJECTS)

tmanager$(EXE) : tmanager$(O) tmanager1$(O) $(LIBKS)

tserver$(EXE) : tserver$(O) tserver1$(O) $(LIBKS)

tsclient$(EXE) : tsclient$(O) tsclient1$(O) $(LIBKS)


