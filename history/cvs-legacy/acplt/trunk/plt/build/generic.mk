# generic part

CXX_LIBPLT_SOURCES = \
	$(SRCDIR)array.cpp \
	$(SRCDIR)container.cpp \
	$(SRCDIR)debug.cpp \
	$(SRCDIR)handle.cpp \
	$(SRCDIR)hashtable.cpp \
	$(SRCDIR)list.cpp \
	$(SRCDIR)log.cpp \
	$(SRCDIR)priorityqueue.cpp \
	$(SRCDIR)rtti.cpp \
	$(SRCDIR)string.cpp \
	$(SRCDIR)time.cpp

LIBPLT_OBJECTS1 = \
	array$(O) \
	container$(O) \
	debug$(O) \
	handle$(O) \
	hashtable$(O)

LIBPLT_OBJECTS2 = \
	list$(O) \
	log$(O) \
	priorityqueue$(O) \
	rtti$(O) \
	string$(O) \
	time$(O)

LIBPLT_OBJECTS = $(LIBPLT_OBJECTS1) $(LIBPLT_OBJECTS2)

CXX_TEST_SOURCES = \
	$(TESTSRCDIR)tarray.cpp \
	$(TESTSRCDIR)tdebug.cpp \
	$(TESTSRCDIR)thandle.cpp\
	$(TESTSRCDIR)thashtable.cpp \
	$(TESTSRCDIR)thtname.cpp \
	$(TESTSRCDIR)tlist.cpp \
	$(TESTSRCDIR)tlog.cpp \
	$(TESTSRCDIR)tpriorityqueue.cpp \
	$(TESTSRCDIR)trtti.cpp \
	$(TESTSRCDIR)tstring.cpp \
	$(TESTSRCDIR)ttime.cpp

CXX_SOURCES = $(CXX_LIBPLT_SOURCES) $(CXX_TEST_SOURCES)

TESTS = tdebug$(EXE) \
	trtti$(EXE) \
	thandle$(EXE) \
	tlog$(EXE) \
	tstring$(EXE) \
	ttime$(EXE) \
	tarray$(EXE) \
	tlist$(EXE) \
	tpriorityqueue$(EXE) \
	thashtable$(EXE) \
	thtname$(EXE)

LIBPLT = libplt$(A)

all : $(LIBPLT)

tests : $(TESTS)

$(TESTS) : $(LIBPLT)

