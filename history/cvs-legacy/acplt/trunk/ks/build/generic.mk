# generic unix makefile

CXX_LIBKS_SOURCES = \
	array.cpp \
	avticket.cpp \
	from_local.cpp \
	path.cpp \
	props.cpp \
	register.cpp \
	result.cpp \
	rpc.cpp \
	serviceparams.cpp \
	string.cpp \
	time.cpp \
	value.cpp \
	xdr.cpp \
	templates.cpp \
	mask.cpp \
	event.cpp \
	templates.cpp

CXX_LIBKSSVR_SOURCES = \
	inaddrset.cpp \
	manager.cpp \
	server.cpp \
	simpleserver.cpp \
	svrbase.cpp \
	svrobjects.cpp \
	svrrpcctx.cpp \
	svrsimpleobjects.cpp

CXX_LIBKSCLN_SOURCES= \
	avmodule.cpp \
	avsimplemodule.cpp \
	client.cpp \
	clntpath.cpp \
	commobject.cpp \
	package.cpp \
	sorter.cpp \
	variables.cpp

LIBKS_OBJECTS1 = \
	array$(O) \
	avticket$(O) \
	from_local$(O) \
	path$(O) \
	props$(O) \
	register$(O) \
	result$(O) \
	rpc$(O)

LIBKS_OBJECTS2 = \
	serviceparams$(O) \
	string$(O) \
	time$(O) \
	value$(O) \
	xdr$(O) \
	templates$(O) \
	mask$(O) \
	event$(O)

LIBKS_OBJECTS = $(LIBKS_OBJECTS1) $(LIBKS_OBJECTS2)

LIBKSSVR_OBJECTS = \
        inaddrset$(O) \
	manager$(O) \
	server$(O) \
	simpleserver$(O) \
	svrbase$(O) \
	svrobjects$(O) \
	svrrpcctx$(O) \
	svrsimpleobjects$(O)

LIBKSCLN_OBJECTS = \
	avmodule$(O) \
	avsimplemodule$(O) \
	client$(O) \
	clntpath$(O) \
	commobject$(O) \
	package$(O) \
	sorter$(O) \
	variables$(O)

CXX_EXAMPLES_SOURCES = \
	pmobile.cpp \
	pmobile_code.cpp \
	tmanager.cpp \
	tmanager1.cpp \
	tsclient.cpp \
	tsclient1.cpp \
	tserver.cpp \
	tserver1.cpp \
	tshell.cpp \
	ttree.cpp \
	ttree1.cpp

EXAMPLES_OBJECTS = \
	pmobile$(O) \
	pmobile_code$(O) \
	tmanager$(O) \
	tmanager1$(O) \
	tsclient$(O) \
	tsclient1$(O) \
	tserver$(O) \
	tserver1$(O) \
	tshell$(O) \
	ttree$(O) \
	ttree1$(O)

EXAMPLES = \
	pmobile$(EXE) \
	tmanager$(EXE) tserver$(EXE) tsclient$(EXE) \
	tshell$(EXE) \
	ttree$(EXE)

CXX_SOURCES = \
	$(CXX_LIBKS_SOURCES) \
	$(CXX_LIBKSSVR_SOURCES) \
	$(CXX_LIBKSCLN_SOURCES) \
	$(CXX_EXAMPLES_SOURCES)

all :	$(LIBKS)

examples :	$(EXAMPLES)

$(LIBKS) : $(LIBKS_OBJECTS)

$(LIBKSSVR) : $(LIBKSSVR_OBJECTS)

$(LIBKSCLN) : $(LIBKSCLN_OBJECTS)

tmanager$(EXE) : tmanager$(O) tmanager1$(O) $(LIBKSSVR) $(LIBKS)

tserver$(EXE) : tserver$(O) tserver1$(O) $(LIBKSSVR) $(LIBKS)

tsclient$(EXE) : tsclient$(O) tsclient1$(O) $(LIBKSSVR) $(LIBKS)

ttree$(EXE) : ttree$(O) ttree1$(O) $(LIBKSCLN) $(LIBKS)

tshell$(EXE) : tshell$(O) $(LIBKSCLN) $(LIBKS)

pmobile$(EXE) : pmobile$(O) pmobile_code$(O) $(LIBKSCLN) $(LIBKS)

