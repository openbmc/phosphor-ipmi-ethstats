project('phosphor-ipmi-ethstats', 'cpp',
        version: '0.1', meson_version: '>=0.49.0',
        default_options: [
          'warning_level=3',
          'werror=true',
          'cpp_std=c++17'
        ])

conf_data = configuration_data()
conf_data.set10('ENABLE_GOOGLE', get_option('google'))

libipmid = dependency('libipmid')

includes = include_directories('.')

libethstatscmd = shared_library(
  'libethstatscmd',
  [
    'main.cpp',
  ],
  soversion : '0.0.0',
  version : '0.0.0',
  dependencies : [libipmid],
  link_args : ['-lstdc++fs'],
  include_directories: includes,
  implicit_include_directories: false,
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers'
)
