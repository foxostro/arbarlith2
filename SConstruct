# SConstruct defines the build process for SCons.
# vim:ts=4:sw=4:et:filetype=python

import glob, os, platform

SOURCES = glob.glob('src/*.cpp') + glob.glob('src/engine/*.cpp')

env = Environment(ENV=os.environ)

env['CC'] = "gcc"
env['CXX'] = "g++"

# Use the system sdl-config if the bootstrap step was not performed.
sdl_config = 'sdl-config'
if os.path.exists('redist/bin/sdl-config'):
    sdl_config = 'redist/bin/sdl-config'

env.ParseConfig(sdl_config + ' --cflags')
env.ParseConfig(sdl_config + ' --libs')

# If the bootstrap step installed headers, use them.
if os.path.exists('redist/include'):
    env.Append(CPPPATH='redist/include')

env.Append(CPPDEFINES='DEBUG')
env.Append(CCFLAGS = ['-ggdb', '-Wall' ])
env.Append(LINKFLAGS = [ '-rdynamic' ])

if platform.uname()[0] == 'Darwin':
    env['FRAMEWORKS'] = ['OpenGL', 'System']
    env.Append(LIBS = [ 'GLEW', 'IL', 'ILU', 'ILUT', 'SDL', 'SDLmain', 'SDL_mixer' ])
else:
    env.Append(LIBS = [ 'GL', 'GLU', 'GLEW', 'IL', 'ILU', 'ILUT', 'SDL', 'SDL_mixer' ])

env.Program(target = 'redist/bin/arbarlith2', source = SOURCES)

