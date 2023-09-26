cc = meson.get_compiler('c')
termcap_dep = cc.find_library('termcap', required: true)

executable('skkfep', [
  'fep.cpp',
  'readwrite.cpp',
  'keybind.cpp',
  'connsh.cpp',
  'kkconv.cpp',
  'version.cpp',
  'terms.cpp',
  'stty.cpp',
  'skklib.cpp',
  'keymap.cpp',
  'etc.cpp',
  'romkan.cpp',
  ],
  install: true,
  dependencies: [termcap_dep],
  c_args: [
    '-DUSE_LOCALE',
    '-Wno-implicit-function-declaration', 
    '-Wno-implicit-int',
    '-Wno-return-type',

    '-Wno-missing-braces',
    '-Wno-comment',
    '-Wno-builtin-declaration-mismatch',
    '-Wno-unused-variable',
    '-Wno-unused-but-set-variable',
    '-Wno-format',
    '-Wno-switch',
    '-Wno-char-subscripts',
    '-Wno-unused-function',
  ],
)

