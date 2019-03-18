AM_DEFAULT_SOURCE_EXT = .cpp

libethstatscmddir = ${libdir}/ipmid-providers
libethstatscmd_LTLIBRARIES = libethstatscmd.la
libethstatscmd_la_SOURCES = main.cpp ethstats.cpp handler.cpp

libethstatscmd_la_LDFLAGS = \
	$(LIBIPMID_LIBS) \
	-lstdc++fs \
	-version-info 0:0:0 -shared

libethstatscmd_la_CXXFLAGS = \
	$(LIBIPMID_CFLAGS) \
	-flto

SUBDIRS = . test
