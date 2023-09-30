cc = meson.get_compiler('c')
termcap_dep = cc.find_library('termcap', required: true)

termpty_lib = static_library('termpty', [
  'stty.cpp',
  'terms.cpp',
  'connsh.cpp',
  'statusline.cpp',
  'termsize.cpp',
  'terminal.cpp',
  'signal_setup.cpp',
  ])

termpty_dep = declare_dependency(
  link_with: termpty_lib,
  include_directories: include_directories('.'),
  dependencies: termcap_dep,
  )
