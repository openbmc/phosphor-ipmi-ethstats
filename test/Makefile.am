AM_CPPFLAGS = \
	-I$(top_srcdir)/ \
	$(GTEST_CFLAGS) \
	$(GMOCK_CFLAGS)
AM_LDFLAGS = \
	$(GTEST_LIBS) \
	$(GMOCK_LIBS) \
	-lgmock_main \
	$(OESDK_TESTCASE_FLAGS)

check_PROGRAMS =
TESTS = $(check_PROGRAMS)

check_PROGRAMS += ethstats_unittest
ethstats_unittest_SOURCES = ethstats_unittest.cpp
ethstats_unittest_LDADD = $(top_builddir)/libethstatscmd.la

