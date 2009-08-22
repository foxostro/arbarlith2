import glob

SOURCES = glob.glob('src/*.cpp') + glob.glob('src/engine/*.cpp')
import SCons.Util

env = Environment()
env.ParseConfig('prereqs/bin/sdl-config --cflags')
env.ParseConfig('prereqs/bin/sdl-config --libs')
env .Append(CPPDEFINES="DEBUG")
env .Append(CPPPATH='prereqs/include')
env.Append(LDFLAGS = [ '-rdynamic' ])
env.Append(CCFLAGS = ['-ggdb', '-Wall'])
env.Append(LIBS = [ 'GL', 'GLU', 'GLEW', 'IL', 'ILU', 'ILUT', 'SDL', 'SDL_mixer' ])
env.Program(target = 'bin/arbarlith2', source = SOURCES)

