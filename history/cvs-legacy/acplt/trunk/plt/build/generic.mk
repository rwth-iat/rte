# generic part

CXX_LIBPLT_SOURCES = \
	$(SRCDIR)alloc.cpp \
	$(SRCDIR)array.cpp \
	$(SRCDIR)container.cpp \
	$(SRCDIR)debug.cpp \
	$(SRCDIR)handle.cpp \
	$(SRCDIR)hashtable.cpp \
	$(SRCDIR)list.cpp \
	$(SRCDIR)log.cpp \
	$(SRCDIR)logstream.cpp \
	$(SRCDIR)priorityqueue.cpp \
	$(SRCDIR)rtti.cpp \
	$(SRCDIR)string.cpp \
	$(SRCDIR)time.cpp

LIBPLT_OBJECTS1 = \
	alloc$(O) \
	array$(O) \
	container$(O) \
	debug$(O) \
	handle$(O) \
	hashtable$(O) \
	list$(O)

LIBPLT_OBJECTS2 = \
	log$(O) \
	logstream$(O) \
	priorityqueue$(O) \
	rtti$(O) \
	string$(O) \
	time$(O)

LIBPLT_OBJECTS = $(LIBPLT_OBJECTS1) $(LIBPLT_OBJECTS2)

CXX_SOURCES = $(CXX_LIBPLT_SOURCES)

LIBPLT = libplt$(A)

all : $(LIBPLT)

