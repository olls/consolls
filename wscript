APPNAME = 'consolls'
VERSION = '0.0.1'

LIVE_RELOAD = True

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


def build(bld):
  if not bld.variant:
    bld.fatal('Must use `{0}_debug` or `{0}_release`'.format(bld.cmd))

  lib_type = bld.shlib if LIVE_RELOAD else bld.stlib

  lib_type(source = bld.path.ant_glob('src/**/*.cpp'),
           target = 'game')

  bld.program(source = bld.path.ant_glob('src/main.cpp'),
              target = APPNAME,
              use = 'game')


def init(ctx):
  from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext, ListContext

  for x in 'debug release'.split():
    for y in (BuildContext, CleanContext, InstallContext, UninstallContext, ListContext):
      name = y.__name__.replace('Context', '').lower()
      class tmp(y):
        cmd = name + '_' + x
        variant = x
