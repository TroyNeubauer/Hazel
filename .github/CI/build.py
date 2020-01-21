import sys
import os

if len(sys.argv) != 5:
	raise Exception('Build script must have four args! Args are ' + str(sys.argv))


premakeCommand = sys.argv[1]
osName = sys.argv[2]
compiler = sys.argv[3]
buildConfiguration = sys.argv[4]

print('premake command ' + premakeCommand)
print('osName ' + osName)
print('compiler ' + compiler)
print('buildConfiguration ' + buildConfiguration)

if (compiler == 'emcc'):
	os.system('source emsdk-master/emsdk_env.sh')
os.system('cp -r .github/CI/CI\\ Project/* ..')
os.system('ls')
os.system('pwd')

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

