project('skkfep', 'cpp', 'c', default_options: ['cpp_std=c++20'])
subdir('libtermpty')
subdir('skk')

executable('skkfep', [
    'main.cpp',
    'app.cpp',
  ],
  install: true,
  dependencies: [termpty_dep, skk_dep],
)

subdir('tests')

