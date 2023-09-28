skk_lib = static_library('skk', [
  'app.cpp',
  'fep.cpp',
  'keybind.cpp',
  'kkconv.cpp',
  'skklib.cpp',
  'keymap.cpp',
  'etc.cpp',
  'romkan.cpp',
  'dictionary.cpp',
  ],
  dependencies: [termpty_dep],
)
skk_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: skk_lib,
  )
