project(
  'phosphor-ipmi-ethstats',
  'cpp',
  version: '0.1',
  meson_version: '>=0.61.0',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
    'werror=true',
  ]
)

root_inc = include_directories('.')

conf_data = configuration_data()

if not get_option('enable_google').disabled()
  conf_data.set('ENABLE_GOOGLE', 1)
else
  conf_data.set('ENABLE_GOOGLE', 0)
endif

conf_h = configure_file(
  output: 'config.h',
  configuration: conf_data
)

meson.get_compiler('cpp').has_header_symbol(
  'ipmid/api.h',
  'ipmid_get_sd_bus_connection')

ethstats_pre = declare_dependency(
  include_directories: root_inc
)

ethstats_lib = static_library(
  'ethstats',
  'ethstats.cpp',
  'handler.cpp',
  conf_h,
  implicit_include_directories: false,
  dependencies: ethstats_pre)

ethstats_dep = declare_dependency(
  link_with: ethstats_lib,
  dependencies: ethstats_pre)

shared_module(
  'ethstatscmd',
  'main.cpp',
  implicit_include_directories: false,
  dependencies: [
    dependency('libipmid'),
    ethstats_dep,
  ],
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers'
)

if not get_option('tests').disabled()
  subdir('test')
endif
