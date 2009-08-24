import os
import glob
import subprocess

def get_platform():
    # XXX: How do I have scons tell me the platform I'm running on?
    p = subprocess.Popen(['uname'], stdout=subprocess.PIPE)
    platform = p.communicate()[0]
    platform = platform.rstrip('\n')
    return platform

SOURCES = glob.glob('src/*.cpp') + glob.glob('src/engine/*.cpp')

env = Environment(ENV=os.environ)

env.ParseConfig('prereqs/bin/sdl-config --cflags')
env.ParseConfig('prereqs/bin/sdl-config --libs')

env.Append(CPPDEFINES="DEBUG")
env.Append(CPPPATH='prereqs/include')
env.Append(CCFLAGS = ['-ggdb', '-Wall' ])
env.Append(LINKFLAGS = [ '-rdynamic' ])

platform = get_platform();
if platform == "Darwin":
	#env.Append(CPPDEFINES="MACOSX")
    env['FRAMEWORKS'] = ['OpenGL', 'System']
    env.Append(LIBS = [ 'GLEW', 'IL', 'ILU', 'ILUT', 'SDL', 'SDLmain', 'SDL_mixer' ])
else:
    env.Append(LIBS = [ 'GL', 'GLU', 'GLEW', 'IL', 'ILU', 'ILUT', 'SDL', 'SDL_mixer' ])


env.Program(target = 'bin/arbarlith2', source = SOURCES)
