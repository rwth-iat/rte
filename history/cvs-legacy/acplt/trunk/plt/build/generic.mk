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

CXX_SOURCES = $(CXX_LIBPLT_SOURCES)

LIBPLT = libplt$(A)

all : $(LIBPLT)

