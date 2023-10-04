skk_lib = static_library('skk', [
  'skk.cpp',

  'input/inputmode.cpp',
  'input/romkan.cpp',
  'input/romkan_hira.cpp',
  'input/romkan_kata.cpp',
  'input/codeinput.cpp',
  'input/zenkakualpha.cpp',

  'conversion/conversionmode.cpp',
  'conversion/kkconv.cpp',
  'conversion/dictionary.cpp',
  ],
  dependencies: [termpty_dep],
)
skk_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: skk_lib,
  )
