skk_lib = static_library('skk', [
  'skk.cpp',
  'skk_keymap.cpp',

  'romkan.cpp',
  'romkan_hira.cpp',
  'romkan_kata.cpp',
  'codeinput.cpp',
  'zenkakualpha.cpp',

  'kkconv.cpp',
  'dictionary.cpp',

  'conversionmode.cpp',
  'inputmode.cpp',
  ],
  dependencies: [termpty_dep],
)
skk_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: skk_lib,
  )
