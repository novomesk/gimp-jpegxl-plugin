project('gimp-jpegxl-plugin',
  'c',
  license : 'GPL3',
  meson_version: '>=0.50.0',
  default_options: [
    'c_std=gnu11',
    'cpp_std=gnu++14',
    'warning_level=1',
  ],
)

warning_cflags_common = [
  '-fdiagnostics-show-option',
  '-fno-common',

  '-Wformat',
  '-Wformat-security',
  '-Winit-self',
  '-Wlogical-op',
  '-Wmissing-declarations',
  '-Wmissing-format-attribute',
  '-Wpointer-arith',
  '-Wreturn-type',
  '-Wtype-limits',
]
warning_cflags_c = [
  '-Wabsolute-value',
  '-Wdeclaration-after-statement',
  '-Wenum-conversion',
  '-Wliteral-conversion',
  '-Wno-strict-prototypes',
  '-Wold-style-definition',
  '-Wparentheses-equality',
  '-W#pragma-messages',
  '-Wsometimes-uninitialized',
  '-Wtautological-unsigned-enum-zero-compare',
  '-Wunneeded-internal-declaration',
  '-Wunused-function',
  '-Wunused-value',

  '-Werror=implicit-function-declaration',
]

cc        = meson.get_compiler('c')
compiler_args = []
compiler_args += cc.get_supported_arguments('-fPIE')
compiler_args += cc.get_supported_arguments(warning_cflags_common)
add_project_arguments(cc .get_supported_arguments(warning_cflags_c),   language: 'c')

jpegxl_minver    = '0.6.0'

libjxl           = dependency('libjxl',         version: '>='+jpegxl_minver)
libjxl_threads   = dependency('libjxl_threads', version: '>='+jpegxl_minver)

gimp             = dependency('gimp-3.0', version: '>=2.99.7')
gimpplugindir    = gimp.get_pkgconfig_variable('gimplibdir')

libgimpui        = dependency('gimpui-3.0')

gtk3             = dependency('gtk+-3.0')

gegl             = dependency('gegl-0.4')


subdir('src')
