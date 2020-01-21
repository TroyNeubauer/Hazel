import sys
import os

def run(command):
	ret = os.system(command)
	if ret != 0:
		print('Command ' + command + ' failed')
		print('Expected error code 0, got ' + ret)
		sys.exit(1)

if len(sys.argv) != 4:
	raise Exception('Build script must have three args! Args are ' + str(sys.argv))


osName = sys.argv[1]
compiler = sys.argv[2]
buildConfiguration = sys.argv[3]

print('osName ' + osName)
print('compiler ' + compiler)
print('buildConfiguration ' + buildConfiguration)

if (compiler == 'emcc'):
	run('source emsdk-master/emsdk_env.sh')
run('cp -r .github/CI/CI\\ Project/* ..')
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
run(premakeCommand)



if osName == 'windows':
	command = 'msbuild /p:Configuration=' + buildConfiguration
else:
	command = 'make -j' + os.cpu_count() + ' config=' + buildConfiguration

print('running: ' + command)	
print('compiling...')
run(command)

#We dont need these vars. The makefile runs the right compiler anyway
#unset CC
#unset CXX
#unset AR
#$PREMAKE_CMD
#- |
#if [ $TRAVIS_OS_NAME == "linux" ]
#then
#
#elif [ $TRAVIS_OS_NAME == "windows" ]
#then
#$BUILD_CONFIG
#fi
