project('skkfep', 'cpp', 'c', default_options: ['cpp_std=c++17'])
subdir('libtermpty')
subdir('skk')

executable('skkfep', 'main.cpp',
  install: true,
  dependencies: [termpty_dep, skk_dep],
)

subdir('tests')

