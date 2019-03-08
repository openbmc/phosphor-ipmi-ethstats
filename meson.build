project('phosphor-ipmi-ethstats', 'cpp',
        version: '0.1', meson_version: '>=0.49.0',
        default_options: [
          'warning_level=3',
          'werror=true',
          'cpp_std=c++17'
        ])

conf_data = configuration_data()

enable_google = get_option('google')
if enable_google
  conf_data.set('ENABLE_GOOGLE', 1)
else
  conf_data.set('ENABLE_GOOGLE', 0)  
endif

libipmid = dependency('libipmid')

compiler = meson.get_compiler('cpp')
if not compiler.has_header('filesystem')
  if compiler.has_header('experimental/filesystem')
    warning('Your compiler lacks <filesystem> (but provides <experimental/filesystem>)')
  else
    warning('Your compiler lacks <filesystem>')
  endif
endif

add_project_arguments('-flto', language: 'cpp')

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
  install_dir: join_paths(get_option('libdir'), 'ipmid-providers')
)
