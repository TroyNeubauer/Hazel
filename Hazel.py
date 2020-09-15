#!/usr/bin/env python3

import argparse
import shutil
import sys
import os
import subprocess
import time
import threading
import multiprocessing
import platform
import zipfile
import shutil
import urllib.request
import os.path
import ssl
import getpass
import traceback, types

script_dir = os.path.dirname(os.path.realpath(__file__)) + os.sep


if platform.system() == "Windows":
	targetName = "windows"
elif platform.system() == "Linux":
	targetName = "linux"
elif platform.system() == "Darwin":
	targetName = "macosx"
else:
	print ("Error: Unknown system: " + platform.system())
	os.exit(-1)


def sizeof_fmt(num, suffix='B'):
	for unit in ['','Ki','Mi','Gi','Ti','Pi','Ei','Zi']:
		if abs(num) < 1024.0:
			return "%3.1f%s%s" % (num, unit, suffix)
		num /= 1024.0
	return "%.1f%s%s" % (num, 'Yi', suffix)

def downloadFile(url, dest_file):
	sys.stdout.write("Downloading: " + url)
	sys.stdout.flush()
	
	ctx = ssl.create_default_context()
	ctx.check_hostname = False
	ctx.verify_mode = ssl.CERT_NONE
	url_file = urllib.request.urlopen(url, context=ctx)
	
	term_size = shutil.get_terminal_size((0, 0))
	total_size = int(url_file.getheader('Content-Length').strip())
	if term_size.columns == 0:
		bytes_per_character = 999999999999#Use an arbutaraly large value to prevent printing
	else:
		bytes_per_character = total_size / term_size.columns
	sys.stdout.write(" - " + sizeof_fmt(total_size))
	sys.stdout.flush()
	print("")#Print newline

	f = open(dest_file, 'wb')

	total = 0
	sub_total = 0
	while 1:
		data = url_file.read(4096)
		total += len(data)
		sub_total += len(data)
		if (sub_total > bytes_per_character):
			sys.stdout.write("#")
			sys.stdout.flush()
			sub_total -= bytes_per_character

		if not data:
			break
		f.write(data)
	f.close()
	print("")
	print("Downloaded " + str(total_size) + " in " + " seconds")

def unzipFile(zip_file, dest_dir):
	print("Extracting " + zip_file + " to " + dest_dir, flush=True)
	with zipfile.ZipFile(zip_file, 'r') as zip_ref:
		zip_ref.extractall(dest_dir)

def run(command, env = None, cwd = None, inputStr=""):
	print("Executing: (\"" + command + "\")" + ((" From " + cwd) if cwd else ""), flush=True)
	process = subprocess.Popen(command, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True, env=env, cwd=cwd)
	if (len(inputStr) > 0):
		time.sleep(2)
		print("\tProviding program with input: \"" + inputStr + "\"")
		process.stdin.write(inputStr)
	
	# Poll process for new output until finished
	while True:
		nextline = process.stdout.readline()
		if process.poll() is not None:
			break
		sys.stdout.write(nextline)
		sys.stdout.flush()

	
#	print("\tProviding program with input: \"" + inputStr + "\"")
#	out, err = process.communicate(inputStr)
#	sys.stdout.write(out)
#	sys.stdout.flush()
	exitCode = process.returncode


	if exitCode != 0:
		print('Command \'' + command + '\' failed!', flush=True)
		print('Expected error code 0, got ' + str(exitCode), flush=True)
		os._exit(1)

def run_advanced(command, env = None, cwd = None):
	print("Executing: (\"" + command + "\")" + ((" From " + cwd) if cwd else ""), flush=True)
	process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=env, cwd=cwd)

	output = []
	# Poll process for new output until finished
	while True:
		nextline = process.stdout.readline()
		if process.poll() is not None:
			break
		for c in nextline.decode("utf-8"):
			output.append(c)

	exitCode = process.returncode
	return (exitCode, ''.join(output))



def updateSubmodules(parentDir):

	modulesFile = os.path.join(parentDir, ".gitmodules")
	if not os.path.exists(modulesFile):
		return
	for line in open(modulesFile).read().splitlines():
		#try:
		modules = []
		if ("path = " in line.strip()):
			parts = line.split("=")
			if (len(parts) != 2):
				print("Submodule " + fullTargetPath + " has malformed .gitmodules file! Line: " + line + " expected url = ... Got " + str(parts) + " instead")
				sys.exit(1)
			
			modules.append(parts[1].strip())
		for path in modules:
			if False:
				threading.Thread(target=updateSubmodulesHelper, args=(parentDir, path))
			else:
				updateSubmodulesHelper(parentDir, path)
			


def updateSubmodulesHelper(parentDir, modulePath):
	fullTargetPath = os.path.join(parentDir, modulePath)
	if (len(os.listdir(fullTargetPath)) > 0):
		print("Skipping " + fullTargetPath)
	else:
		depth = 1
		while True:
			exit_code, output = run_advanced("git submodule update --init --depth " + str(depth) + " " + modulePath, cwd=parentDir)
			if exit_code != 0:
				if "Direct fetching of that commit failed" in output:
					depth *= 2
				elif "could not lock config file" in output:
					print("Temp failure from " + fullTargetPath)
					continue
				else:
					print("Fatal git error: " + output)
					os._exit(1)
			else:
				break

	modulesFile = os.path.join(fullTargetPath, ".gitmodules")
	if not os.path.exists(modulesFile):
		return

	modules = []

	for line in open(modulesFile).read().splitlines():

		if ("path = " in line.strip()):
			parts = line.split("=")
			if (len(parts) != 2):
				print("Submodule " + fullTargetPath + " has malformed .gitmodules file! Line: " + line + " expected url = ... Got " + str(parts) + " instead")
				os._exit(1)
			
			modules.append(parts[1].strip())

	if len(modules) == 1:
		updateSubmodulesHelper(fullTargetPath, modules[0])
	else:
		threads = []
		for path in modules:
			new_thread = threading.Thread(target=updateSubmodulesHelper, args=(fullTargetPath, path))
			new_thread.start()
			threads.append(new_thread)

		for thread in threads:
			thread.join()
			




parser = argparse.ArgumentParser(description='This is a script for generating project files')
parser.add_argument('--compiler', '-c', help='Determines which compiler is used when generating buildscripts [clang, gcc, msc, emcc]')
parser.add_argument('--target', '-t', help='Specifies to premake which build system to target [gmake2, vs2019, xcode, android, etc]')
parser.add_argument('--premakepath', help='The path to a premake5 executable')
parser.add_argument('--server', '-s', help='Runs a HTTP server to allow for running the results of emscripten builds', action='store_true')
parser.add_argument('--clean', help='Deletes all project files and binaries', action='store_true')
parser.add_argument('--rebuild_premake', '-rbp', help='Forces a rebuild of premake before generating buildfiles', action='store_true')
parser.add_argument('--build', '-b', help='Runs the build with the given configuration after generating the build files', choices=("debug", "release"), default="default", const='debug', nargs='?',)
parser.add_argument('--abi', help='Specifies which ABI to build for (x86, x64, armv7, all, etc.), requires --build', nargs='+')
parser.add_argument('--test', help='Runs tests (implies --build)', action='store_true')
parser.add_argument('--coverage', '-cov', help='Builds the workspace with code coverage enabled, runs the tests, and uploads the results (implies --build and --test)', action='store_true')
parser.add_argument('--clone-submodules', '-clone', help='Uses a custom downloader script to clone submodules and then exits. Does nothing is submodules already exist', action='store_true')

results = parser.parse_args()



args_len = 0
for value in vars(results).values():
	if value:
		args_len += 1

if results.clone_submodules:
	print("Cloning")
	start = time.time()
	updateSubmodules(script_dir)
	print("Cloning took: " + str((time.time() - start)))
	sys.exit(0)


if results.server:
	print("Starting server", flush=True)
	if platform.system() == "Windows":
		run("python -m http.server")
	else:
		run("python3 -m http.server")

else:

	if platform.system() == "Windows":
		hasPremake = os.path.exists(script_dir + "vendor/premake-core/bin/release/premake5.exe")
	else:
		hasPremake = os.path.exists(script_dir + "vendor/premake-core/bin/release/premake5")

	if results.rebuild_premake or not hasPremake:
		if platform.system() == "Windows":
			run("Bootstrap.bat", cwd=(script_dir + "vendor/premake-core"))
		elif platform.system() == "Linux":
			run("make -f Bootstrap.mak linux", cwd=(script_dir + "vendor/premake-core"))
		else:
			run("make -f Bootstrap.mak osx", cwd=(script_dir + "vendor/premake-core"))

	all_abis = []
	guessedABI = False

	if results.abi == None:
		results.abi = [ "x86" if sys.maxsize < 2**32 else "x64" ]
		guessedABI = True

	if results.target == None:
		if platform.system() == "Windows":
			target = "vs2019"
		elif platform.system() == "Linux":
			target = "gmake2"
		elif platform.system() == "Darwin":
			target = "gmake2" #"xcode4"

	elif results.target == "windows":
		if platform.system() == "Windows":
			target = "vs2019"
		
		elif platform.system() == "Linux":
			target = "gmake2"
			results.compiler = "mingw"
			print("Forcing mingw cross compile")
		elif platform.system() == "Darwin":
			print("Cross compiling to windows from OSX is not supported!")
			sys.exit(1)
	else:
		target = results.target

	if results.compiler == None:
		if platform.system() == "Windows":
			compiler = "msc"
		elif platform.system() == "Linux":
			compiler = "gcc"
		elif platform.system() == "Darwin":
			compiler = "clang"
	else:
		compiler = results.compiler


	if results.premakepath == None:
		premakepath = script_dir + "vendor/premake-core/bin/release/premake5"
		if platform.system() == "Windows":
			premakepath = premakepath + ".exe"
	else:
		premakepath = results.premakepath

	if platform.system() == "Windows":
		premakepath = "call " + premakepath
		all_abis.append("x86")
		all_abis.append("x86_64")

	if compiler == "emcc":
		command = premakepath + " --os=emscripten gmake2"
		target = "gmake2"
		#TODO ABI's?

	elif target == "xcode4":
		command = premakepath + " " + target
		all_abis.append("x86")
		all_abis.append("x86_64")

	elif target == "osx-xcode":
		command = premakepath + " --os=macosx xcode4"
		all_abis.append("x86_64")

	elif target == "osx":
		command = premakepath + " --os=macosx gmake2"
		all_abis.append("x86_64")

	elif target == "ios-xcode":
		command = premakepath + " --os=ios xcode4"
		all_abis.append("x86")
		all_abis.append("arm")

	elif target == "ios":
		command = premakepath + " --os=ios gmake2"
		all_abis.append("x86")
		all_abis.append("arm")
		if guessedABI:
			results.abi = [ "x86" ]

	elif target == "android":
		command = premakepath + " androidmk"
		all_abis.append("armeabi-v7a")
		all_abis.append("arm64-v8a")
		all_abis.append("x86")
		all_abis.append("x86_64")

	elif platform.system() == "Linux":
		command = premakepath + " --os=" + targetName + " --compiler=" + compiler + " " + target
		all_abis.append("x86")
		all_abis.append("x86_64")

	else:
		command = premakepath + " --compiler=" + compiler + " " + target
		all_abis.append("x86_64")
		print("Assuming x64 ABI")

	if results.clean:
		command = premakepath + " clean"

	if platform.system() == "Windows":
		command = command.replace("/", "\\")

	if (results.coverage):
		command += " --coverage"
		results.test = True

	if results.test:
		if not results.build or results.build == "default":
			results.build = "debug"


	run(command, cwd=os.path.join(script_dir, ".."))
	results.abi = list(map(lambda abi: abi if abi != "x64" else "x86_64", results.abi))

	if results.build != "default":
		if len(results.abi) == 1 and results.abi[0] == "all":
			results.abi = all_abis

		msBuildDir = None
		if targetName == "windows":
			exitCode, result = run_advanced("reg.exe query \"HKLM\\SOFTWARE\\Microsoft\\MSBuild\\ToolsVersions\\4.0\" /v MSBuildToolsPath")
			array = result.split("REG_SZ")
			if len(array) == 2:
				msBuildDir = array[1].strip()
				print("Got msbuild location: " + msBuildDir)
			else:
				print("Failed to get ms build location! \"" + result + "\" does not contain the string \"REG_SZ\" exactly once")
				sys.exit(1)

		for abi in results.abi:
			if not abi in all_abis:
				print("Invalid ABI: " + abi + " for target")
				sys.exit(1)

			if target == "gmake2" or "-make" in target:

				cpu_string = "-j" + str(int(os.cpu_count() * 5 / 6))
				print("Compiling project using " + cpu_string + " cores")
				t0 = time.time()
				run("make " + cpu_string +" config=" + results.build, cwd=os.path.join(script_dir, ".."))
				print("Compilation took " + str(time.time() - t0) + " seconds")

			elif "vs" in target:
				print("Compiling windows project using msbuild.exe")
				args = []
				args.append("/m")
				args.append("/property:Configuration=" + results.build.capitalize() + " /property:Platform=x64")
				args.append("/p:VisualStudioVersion=12.0")
				
				env = os.environ.copy()
				env["PATH"] = msBuildDir + os.pathsep + env["PATH"]
				#run("MSBuild.exe " + " ".join(args), cwd=script_dir, env=env)
				#run("cmd.exe /c \"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat\"",
				#	inputStr="MSBuild " + script_dir + "\r\n")
				

	if results.test:
		#There is only one folder
		if len(results.abi) == 0:
			print("Cannot run tests with no ABI!")
			sys.exit(1)

		run(script_dir + "bin/" + results.build.capitalize() + "-" + targetName + "-" + results.abi[0] + "/Test", cwd=script_dir + "Test")

	if results.coverage:
		print("gathering code coverage...")

