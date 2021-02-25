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

  flags = [
    '--std=c++17',
    '-Wc++1z-extensions',
    '-Werror',
    '-pedantic',
    '-Weverything',
    '-fcolor-diagnostics',
    '-fno-exceptions',
    '-fstack-protector',
    '-Wformat-security',
    '-fvisibility=hidden',
    '-Wpointer-arith',
    '-Wformat-nonliteral',
    '-Winit-self',
    '-Wdouble-promotion',

    '-Wno-c++98-compat',
    '-Wno-unused-parameter',
    '-Wno-packed',
    '-Wno-old-style-cast',
    '-Wno-c99-extensions',
    '-Wno-missing-prototypes',
    '-Wno-covered-switch-default',
    '-Wno-padded',
    '-Wno-nested-anon-types',
    '-Wno-sometimes-uninitialized',
    '-Wno-zero-as-null-pointer-constant',
    '-Wno-format-pedantic'
  ]
  flags += sdl_flags

  link_flags = [
    '-Wl,-z,relro',
    '-Wl,-z,now'
  ]
  link_flags += sdl_libs

  conf.env.append_value('CXXFLAGS', flags)
  conf.env.append_value('INCLUDES', ['.', './src/'])
  conf.env.append_value('LINKFLAGS', link_flags)

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

  # Build libraries as shared libs if using LIVE_RELOAD
  lib_type = bld.shlib if LIVE_RELOAD else bld.stlib

  libraries = [
    'game',
    'machine',
    'utils'
  ]

  for library_name in libraries:
    lib_type(source = bld.path.ant_glob('src/{}/*.cpp'.format(library_name)),
             target = library_name)

  # consolls loader program
  bld.program(source = bld.path.ant_glob('src/loader/*.cpp'),
              target = APPNAME,
              use = ['game', 'machine', 'utils'])

  # compolls program
  bld.program(source = bld.path.ant_glob('src/compolls/*.cpp'),
              target = 'compolls',
              use = ['machine', 'utils'])


def init(ctx):
  from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext, ListContext

  for x in ('debug', 'release', 'profile'):
    for y in (BuildContext, CleanContext, InstallContext, UninstallContext, ListContext):
      name = y.__name__.replace('Context', '').lower()
      class tmp(y):
        cmd = name + '_' + x
        variant = x
