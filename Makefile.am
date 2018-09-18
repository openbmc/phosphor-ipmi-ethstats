AM_DEFAULT_SOURCE_EXT = .cpp

libethstatscmddir = ${libdir}/ipmid-providers
libethstatscmd_LTLIBRARIES = libethstatscmd.la
libethstatscmd_la_SOURCES = main.cpp

libethstatscmd_la_LDFLAGS = $(SYSTEMD_LIBS) \
	$(PHOSPHOR_DBUS_INTERFACES_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS) \
	-lstdc++fs \
	-version-info 0:0:0 -shared

libethstatscmd_la_CXXFLAGS = $(SYSTEMD_CFLAGS) \
	$(PHOSPHOR_DBUS_INTERFACES_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS)

SUBDIRS = .
