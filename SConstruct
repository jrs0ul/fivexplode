import os
import sys

OBJ_DIR = os.path.join(os.getcwd(), 'obj/');
env = Environment(ENV=os.environ,
                  CCFLAGS=['-O3','-Wall'],
                  OBJPREFIX = OBJ_DIR)


code = ["src/mainer.cpp",
        "src/Xml.cpp",
        "src/gui/Text.cpp",
        "src/TextureLoader.cpp",
        "src/Image.cpp",
        "src/Vectors.cpp",
        "src/gui/Button.cpp",
        "src/Utils.cpp",
        "src/Extensions.cpp",
        "src/Matrix.cpp",
        "src/FindPath.cpp",
        "src/gui/MenuStruct.cpp",
        "src/gui/SelectMenu.cpp",
        "src/gui/YesNoDialog.cpp",
        "src/CppSingleton.cpp",
        "src/audio/OggStream.cpp",
        "src/audio/SoundSystem.cpp",
        "src/gui/Scrollbar.cpp",
        "src/externals.cpp",
        "src/HighScore.cpp",
        "src/SysConfig.cpp",
        "src/SDLVideo.cpp",
        "src/gui/OnScreenBoard.cpp",
        "src/Threads.cpp",
        "src/VertexObject.cpp",
        "src/Grid.cpp"

        ]

#Windblows
if sys.platform == 'win32':
    Tool('mingw')(env)
    env.Append(LIBS = ['mingw32','SDLmain', 'SDL', 'OpenGL32',
                       'openal32', 'vorbisfile', 'ogg'])
else: #Mac OS X
    if sys.platform == 'darwin':
        env['FRAMEWORKS'] = ['Cocoa', 'SDL', 'OPENGL', 'OpenAL','vorbis', 'Ogg', 'libcurl']
        env.Append(LIBS = ['iconv', 'pthread'])
        env.Append(CCFLAGS = ['-arch', 'i386'])
        env.Object('src/osx/SDLMain.m', OBJPREFIX = OBJ_DIR,
                   CCFLAGS = ['-arch', 'i386', '-I/Library/Frameworks/SDL.framework/Headers/'])
        code.append('obj/SDLMain.o')
        env.Append(LINKFLAGS = ['-arch', 'i386'])
    else: #Linux
        env.Append(LIBS = ['SDL','GL', 'openal', 'vorbisfile',
                           'ogg', 'pthread', 'libcurl'])


env.Program(target = "fiveXplode", 
            source = code
            )

