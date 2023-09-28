project('skkfep', 'cpp', 'c', default_options: [])
subdir('libtermpty')
subdir('skk')

executable('skkfep', 'main.cpp',
  install: true,
  dependencies: [termpty_dep, skk_dep],
)
