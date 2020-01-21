import sys
import os
import subprocess
import shutil
import distutils.file_util

def run(command, in_env = None):
	process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=in_env)

	# Poll process for new output until finished
	while True:
		nextline = process.stdout.readline()
		if nextline == '' and process.poll() is not None:
			break
		sys.stdout.write(nextline)
		sys.stdout.flush()

	exitCode = process.returncode

	if exitCode != 0:
		print('Command \'' + command + '\' failed!')
		print('Expected error code 0, got ' + str(exitCode))
		sys.exit(1)

def copyALlInDir(src, dest):
	for root, dirs, files in os.walk(src, topdown=False):
		for name in dirs:
			print('loop dir ' + root + ' ' + name)
			os.mkpath(os.path.join(root, name))
		for name in files:
			print('loop file ' + root + ' ' + name)
			file_util.copy_file(os.path.join(root, name), os.path.join(dest, name))


if len(sys.argv) != 4:
	raise Exception('Build script must have three args! Args are ' + str(sys.argv))


osName = sys.argv[1]
compiler = sys.argv[2]
buildConfiguration = sys.argv[3]

print('osName ' + osName)
print('compiler ' + compiler)
print('buildConfiguration ' + buildConfiguration)

copyALlInDir('.github/CI/CI Project/', '..')
#move to the new root of the project
os.chdir('..')
run('ls')

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
	command = 'make -j2 config=' + buildConfiguration

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
