project('annkh', 'c')

cc = meson.get_compiler('c')

sources = [
    'src/layer.c',
    'src/softmax.c',
    'src/net.c',
    'src/trainer.c',
    'src/pool.c',
]

m_dep = cc.find_library('m', required: true)

library('annkh', sources, dependencies: m_dep)
