project('skkfep', 'c')

cc = meson.get_compiler('c')
termcap_dep = cc.find_library('termcap', required: true)

executable('skkfep', [
  'fep.c',
  'readwrite.c',
  'keybind.c',
  'connsh.c',
  'kkconv.c',
  'version.c',
  'terms.c',
  'stty.c',
  'skklib.c',
  'connserv.c',
  'keymap.c',
  'etc.c',
  'romkan.c',
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
