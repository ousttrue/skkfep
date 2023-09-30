skk_lib = static_library('skk', [
  'app.cpp',
  'etc.cpp',
  'skk.cpp',
  'skk_keymap.cpp',

  'romkan.cpp',
  'codeinput.cpp',

  'kkconv.cpp',
  'dictionary.cpp',
  ],
  dependencies: [termpty_dep],
)
skk_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: skk_lib,
  )
