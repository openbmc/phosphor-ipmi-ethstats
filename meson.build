project(
  'phosphor-ipmi-ethstats',
  'cpp',
  version: '1.0',
  default_options: [
    'cpp_std=c++23',
    'warning_level=3',
    'werror=true',
  ],
  meson_version: '>=1.1.1')

ethstats_pre = declare_dependency(
  include_directories: include_directories('.'),
  dependencies: [
    dependency('libipmid'),
    dependency('stdplus'),
  ])

ethstats_lib = static_library(
  'ethstatscmd_common',
  'ethstats.cpp',
  'handler.cpp',
  implicit_include_directories: false,
  dependencies: ethstats_pre)

ethstats_dep = declare_dependency(
  dependencies: ethstats_pre,
  link_with: ethstats_lib)

conf_data = configuration_data()
conf_data.set10('ENABLE_GOOGLE', get_option('google_oen'))

shared_library(
  'ethstatscmd',
  configure_file(output: 'config.h', configuration: conf_data),
  'main.cpp',
  implicit_include_directories: false,
  dependencies: ethstats_dep,
  override_options: 'b_lundef=false',
  version: meson.project_version(),
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers')

if get_option('tests').allowed()
  subdir('test')
endif
