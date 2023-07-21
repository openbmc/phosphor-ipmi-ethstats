project(
  'phosphor-ipmi-ethstats',
  'cpp',
  version: '1.0',
  default_options: [
    'buildtype=debugoptimized',
    'cpp_std=c++23',
    'warning_level=3',
    'werror=true',
  ],
  meson_version: '>=1.1.1',
)

conf_data = configuration_data()
conf_data.set('ENABLE_GOOGLE', get_option('google_oen').allowed().to_int())
configure_file(
  output: 'config.h',
  configuration: conf_data,
)

ipmid_dep = dependency('libipmid')

ethstatscmd_common_lib = static_library(
  'ethstatscmd_common',
  'ethstats.cpp',
  'handler.cpp',
  dependencies: [
    ipmid_dep,
  ],
)

ethstatscmd_common_dep = declare_dependency(
  link_with: ethstatscmd_common_lib,
)

ethstatscmd_lib = library(
  'ethstatscmd',
  'main.cpp',
  dependencies: [
    ethstatscmd_common_dep,
    ipmid_dep,
  ],
  override_options: [ 'b_lundef=false' ],
  version: meson.project_version(),
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers',
)

if get_option('tests').allowed()
  subdir('test')
endif
