project('annkh', 'c')

cc = meson.get_compiler('c')

sources = [
    'src/layer.c',
    'src/mathutil.c',
    'src/net.c',
    'src/pool.c',
    'src/poollist.c',
    'src/errorc.c',
    'src/environment.c',
    'src/train/t_hebbian.c',
    'src/train/t_evstrat.c',
]

m_dep = cc.find_library('m', required: true)

library('annkh', sources, dependencies: m_dep)
