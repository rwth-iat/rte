# generic unix makefile

CXX_LIBKS_SOURCES = \
	array.cpp \
	avmodule.cpp \
	avticket.cpp \
	client.cpp \
	clntpath.cpp \
	commobject.cpp \
	from_local.cpp \
	package.cpp \
	path.cpp \
	props.cpp \
	register.cpp \
	result.cpp \
	rpc.cpp \
	serviceparams.cpp \
	sorter.cpp \
	string.cpp \
	time.cpp \
	value.cpp \
	variables.cpp \
	xdr.cpp \
	mask.cpp \
	event.cpp \
	templates.cpp

CXX_LIBKSSVR_SOURCES = \
	manager.cpp \
	server.cpp \
	simpleserver.cpp \
	svrbase.cpp \
	svrobjects.cpp \
	svrrpcctx.cpp \
	svrsimpleobjects.cpp

LIBKS_OBJECTS1 = \
	array$(O) \
	avticket$(O) \
	clntpath$(O) \
	event$(O) \
	from_local$(O)

LIBKS_OBJECTS2 = \
	package$(O) \
	path$(O) \
	props$(O) \
	register$(O) \
	result$(O) \
	rpc$(O) \
	serviceparams$(O)

LIBKS_OBJECTS3 = \
	mask$(O)

LIBKS_OBJECTS4 = \
	time$(O) \
	value$(O) \
	xdr$(O) \
	templates$(O)

LIBKS_OBJECTS5 = \
	string$(O) \
	sorter$(O) \
	avmodule$(O) \
	client$(O) \
	commobject$(O) \
	variables$(O)

LIBKS_OBJECTS = $(LIBKS_OBJECTS1) $(LIBKS_OBJECTS2) $(LIBKS_OBJECTS3) \
	$(LIBKS_OBJECTS4) $(LIBKS_OBJECTS5)

LIBKSSVR_OBJECTS = \
	manager$(O) \
	server$(O) \
	simpleserver$(O) \
	svrbase$(O) \
	svrobjects$(O) \
	svrrpcctx$(O) \
	svrsimpleobjects$(O)

CXX_EXAMPLES_SOURCES = \
	tmanager.cpp \
	tmanager1.cpp \
	tsclient.cpp \
	tsclient1.cpp \
	tserver.cpp \
	tserver1.cpp \
	ttree.cpp \
	ttree1.cpp

EXAMPLES_OBJECTS = \
	tmanager$O \
	tmanager1$O \
	tsclient$O \
	tsclient1$O \
	tserver$O \
	tserver1$O \
	ttree$O \
	ttree1$O

TESTS = tmanager$(EXE) tserver$(EXE) tsclient$(EXE) ttree$(EXE)

CXX_SOURCES = $(CXX_LIBKS_SOURCES) $(CXX_EXAMPLES_SOURCES)

all :	$(LIBKS)

tests :	$(TESTS)

$(LIBKS) : $(LIBKS_OBJECTS)

tmanager$(EXE) : tmanager$(O) tmanager1$(O) $(LIBKS)

tserver$(EXE) : tserver$(O) tserver1$(O) $(LIBKS)

tsclient$(EXE) : tsclient$(O) tsclient1$(O) $(LIBKS)

ttree$(EXE) : ttree$(O) ttree1$(O) $(LIBKS)



