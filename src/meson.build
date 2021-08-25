common_plugins_list = [
]

if libjxl.found()
  common_plugins_list += { 'name': 'file-jpegxl',
    'deps': [ gtk3, gegl, libjxl, libjxl_threads, ],
  }
endif

foreach plugin : common_plugins_list
  plugin_name = plugin.get('name')
  plugin_sources = [ plugin.get('sources', plugin_name + '.c') ]
  plugin_deps = plugin.get('deps', [ gtk3, gegl, ])
  plugin_cflags = plugin.get('cflags', [])

  executable(plugin_name,
    plugin_sources,
    dependencies: [ plugin_deps, libgimpui, ],
    c_args: plugin_cflags,
    install: true,
    install_dir: gimpplugindir / 'plug-ins' / plugin_name,
  )
endforeach
