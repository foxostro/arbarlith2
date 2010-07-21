# vim:ts=4:sw=4:expandtab
import os
import glob
import platform

SOURCES = glob.glob('src/*.cpp') + glob.glob('src/engine/*.cpp')

env = Environment(ENV=os.environ)

env['CC'] = "gcc"
env['CXX'] = "g++"

env.ParseConfig('redist/bin/sdl-config --cflags')
env.ParseConfig('redist/bin/sdl-config --libs')

env.Append(CPPDEFINES="DEBUG")
env.Append(CPPPATH='redist/include')
env.Append(CCFLAGS = ['-ggdb', '-Wall' ])
env.Append(LINKFLAGS = [ '-rdynamic' ])

if platform.uname()[0] == "Darwin":
	#env.Append(CPPDEFINES="__MACOSX__")
    env['FRAMEWORKS'] = ['OpenGL', 'System']
    env.Append(LIBS = [ 'GLEW', 'IL', 'ILU', 'ILUT', 'SDL', 'SDLmain', 'SDL_mixer' ])
else:
    env.Append(LIBS = [ 'GL', 'GLU', 'GLEW', 'IL', 'ILU', 'ILUT', 'SDL', 'SDL_mixer' ])


os.system("gcc src/ListenForDevices.c -o redist/share/arbarlith2/ListenForDevices -framework IOkit -framework CoreFoundation");

env.Program(target = 'redist/bin/arbarlith2', source = SOURCES)
