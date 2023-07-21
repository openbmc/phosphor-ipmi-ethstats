gtest_dep = dependency('gtest', main: true)
gmock_dep = dependency('gmock')

test(
  'test_ethstats',
  executable(
    'test-ethstats',
    'ethstats_unittest.cpp',
    include_directories: [
      '..',
    ],
    dependencies: [
      ethstatscmd_common_dep,
      gmock_dep,
      gtest_dep,
      ipmid_dep,
    ],
  ),
)
