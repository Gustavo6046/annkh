project('annkh', 'c')


cc = meson.get_compiler('c')

# Build annkh
sources = [
    'src/layer.c',
    'src/mathutil.c',
    'src/net.c',
    'src/pool.c',
    'src/poollist.c',
    'src/errorc.c',
    'src/environment.c',
    'src/rng.c',
    'src/boxmuller.c',

    'src/train/t_hebbian.c',
    'src/train/t_evstrat.c',

    'src/environments/e_classifier.c',
]

m_dep = cc.find_library('m', required: true)

munit_proj = subproject('munit')
munit_dep = munit_proj.get_variable('munit_dep')

libannkh = library('annkh', sources, dependencies: [m_dep, munit_dep], include_directories: ['src', 'subprojects'])

annkh_dep = declare_dependency(
    include_directories: ['src'],
    link_with: libannkh,
    dependencies: [m_dep]
)

# Build tests
tests = [
    'net_layer',
    'poolalloc'
]

foreach testname : tests
    testexe = executable('test_' + testname, 'tests/' + testname + '.c', dependencies: [annkh_dep, munit_dep], include_directories: 'subprojects')
    test(testname, testexe)
endforeach

# Build examples
examples = [
    'xor'
]

example_include = include_directories([ 'src' ])

foreach ename : examples
    exe = executable(
        'example_' + ename,
        'example/' + ename + '.c',
        dependencies: [annkh_dep],
        include_directories: example_include
    )
endforeach
