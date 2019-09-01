local ROOT_DIR = path.getdirectory(_SCRIPT) .. "/"

-- Load the OpenSSL extension
include "modules/openssl/openssl.lua"
local openssl = premake.extensions.openssl

-- Configure the OpenSSL build
openssl_config = {
	src_dir = ROOT_DIR .. "openssl_tarball/",
	include_dir = ROOT_DIR .. "include/",
	excluded_libs = {
		"jpake",
		"rc5",
		"md2",
		"store",
		"engine",
	},
}

--
-- Generate public OpenSSL header files
-- 
if _ACTION == "openssl_headers" then
	print "Generating header files"
	premake.extensions.openssl.copy_public_headers(openssl_config)
	os.exit(0)
end

--
-- Generate a solution with crypto/ssl Static Library projects
--

project "crypto"
	language "C"
	kind "StaticLib"

	location (ROOT_DIR .. ".build/projects/")
	objdir (ROOT_DIR .. ".build/obj/")

	configuration {"Debug"}
		targetdir (ROOT_DIR .. "lib/debug/")

	configuration {"Release"}
		optimize "Speed"
		inlining "auto"
		targetdir (ROOT_DIR .. "lib/release/")
	configuration {"Dist"}
		optimize "Speed"
		inlining "auto"
		targetdir (ROOT_DIR .. "lib/release/")

	openssl.crypto_project(openssl_config)
	includedirs
	{
		ROOT_DIR .. "include/"
	}

project "ssl"

	language "C"
	kind "StaticLib"

	location (ROOT_DIR .. ".build/projects/")
	objdir (ROOT_DIR .. ".build/obj/")

	configuration {"Debug"}
		targetdir (ROOT_DIR .. "lib/debug/")

	configuration {"Release"}
		optimize "Speed"
		inlining "auto"
		targetdir (ROOT_DIR .. "lib/release/")

	configuration {"Dist"}
		optimize "Speed"
		inlining "auto"
		targetdir (ROOT_DIR .. "lib/release/")

	openssl.ssl_project(openssl_config)
	includedirs
	{
		ROOT_DIR .. "include/"
	}
