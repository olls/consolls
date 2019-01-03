APPNAME = 'consolls'
VERSION = '0.0.1'

LIVE_RELOAD = False

top = '.'
out = 'build'


def configure(conf):
  conf.load('clang++')

  conf.define('APP_NAME', APPNAME)
  conf.define('APP_VERSION', VERSION)
  conf.define('LIVE_RELOAD', LIVE_RELOAD)

  from subprocess import check_output
  sdl_flags = check_output(['sdl2-config', '--cflags']).split()
  sdl_libs  = check_output(['sdl2-config', '--libs'  ]).split()

  conf.env.append_value('CXXFLAGS', ['--std=c++17', '-Werror', '-fcolor-diagnostics'] + sdl_flags)
  conf.env.append_value('INCLUDES', ['.'])
  conf.env.append_value('LINKFLAGS', sdl_libs)

  base_env = conf.env.derive()

  conf.setenv('debug', env=base_env)
  conf.env.append_value('CXXFLAGS', ['-ggdb', '-O0'])
  conf.env.append_value('DEFINES', ['_DEBUG'])

  conf.setenv('release', env=base_env)
  conf.env.append_value('CXXFLAGS', ['-Ofast'])
  conf.env.append_value('DEFINES', ['NDEBUG'])

  release_env = conf.env.derive()

  conf.setenv('profile', env=release_env)
  conf.env.append_value('CXXFLAGS', ['-g'])


def build(bld):
  if not bld.variant:
    bld.fatal('Must use `{0}_debug` or `{0}_release or {0}_profile`'.format(bld.cmd))

  main_src = 'src/main.cpp'
  compiler_src = 'src/compiler-frontend.cpp'

  lib_type = bld.shlib if LIVE_RELOAD else bld.stlib

  lib_type(source = bld.path.ant_glob('src/**/*.cpp', excl=[main_src, compiler_src]),
           target = 'game')

  bld.program(source = bld.path.ant_glob(main_src, excl=compiler_src),
              target = APPNAME,
              use = 'game')

  bld.program(source = bld.path.ant_glob('src/**/*.cpp', excl=main_src),
              target = 'compolls')


def init(ctx):
  from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext, ListContext

  for x in ('debug', 'release', 'profile'):
    for y in (BuildContext, CleanContext, InstallContext, UninstallContext, ListContext):
      name = y.__name__.replace('Context', '').lower()
      class tmp(y):
        cmd = name + '_' + x
        variant = x
