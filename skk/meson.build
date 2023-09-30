skk_lib = static_library('skk', [
  'app.cpp',
  'signal_setup.cpp',
  'kkconv.cpp',
  'etc.cpp',
  'romkan.cpp',
  'dictionary.cpp',
  'skk.cpp',
  'skk_keymap.cpp',
  ],
  dependencies: [termpty_dep],
)
skk_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: skk_lib,
  )
