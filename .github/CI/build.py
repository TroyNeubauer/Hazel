import sys
import os

if len(sys.argv) != 4:
	raise Exception('Build script must have three args! Args are ' + str(sys.argv))


osName = sys.argv[1]
compiler = sys.argv[2]
buildConfiguration = sys.argv[3]

print('osName ' + osName)
print('compiler ' + compiler)
print('buildConfiguration ' + buildConfiguration)

if (compiler == 'emcc'):
	os.system('source emsdk-master/emsdk_env.sh')
os.system('cp -r .github/CI/CI\\ Project/* ..')
#move to the new root of the project
os.chdir('..')


premakeCommand = ''
if osName == 'linux':
	premakeCommand += 'premake5 '

elif osName == 'windows':
	premakeCommand += 'Hazel/vendor/premake/bin/premake5.exe '

elif osName == 'macosx':
	print('osx is unsupported for now!')
	sys.exit(1)

else:
	print('Unknown OS! ' + osName)
	sys.exit(1)



if compiler == 'msvc':
	premakeCommand += '--os=windows --compiler=msc vs2017'

elif compiler == 'gcc':
	premakeCommand += '--os=' + osName + ' --compiler=gcc gmake2'

elif compiler == 'clang':
	premakeCommand += '--os=' + osName + ' --compiler=gcc gmake2'

elif compiler == 'emcc':
	premakeCommand += '--os=emscripten --scripts=Hazel/vendor/premake/scripts gmake2'

else:
	print('Unknown compiler! ' + compiler)
	sys.exit(1)

print('running premake: \"' + premakeCommand + '\"')
os.system(premakeCommand)

#We dont need these vars. The makefile runs the right compiler anyway
#unset CC
#unset CXX
#unset AR
#$PREMAKE_CMD
#- |
#if [ $TRAVIS_OS_NAME == "linux" ]
#then
#make -j2 config=$BUILD_CONFIG
#elif [ $TRAVIS_OS_NAME == "windows" ]
#then
#msbuild /p:Configuration=$BUILD_CONFIG
#fi

#        - PREMAKE_CMD='" --os=emscripten --scripts=Hazel/vendor/premake/scripts gmake2"'
#        - PREMAKE_CMD='"Hazel\vendor\premake\bin\premake5.exe --os=windows --compiler=msc vs2017"'
#        - PREMAKE_CMD='"premake5 --os=linux --compiler=gcc gmake2"'
#        - PREMAKE_CMD='"premake5 --os=linux --compiler=clang gmake2"'