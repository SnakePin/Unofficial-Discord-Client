# Downloads SDL2.0.10 and extracts it into the correct folders.

import requests, sys, zipfile, os, shutil, tarfile

msvc_url = 'https://www.libsdl.org/release/SDL2-devel-2.0.10-VC.zip'
msvc_filename = 'SDL2-devel-2.0.10-VC.zip'
exfolder = 'SDL2-2.0.10'
msvc_lib = 'SDL2-2.0.10/lib'
msvc_include = 'SDL2-2.0.10/include'

mingw_url = 'https://www.libsdl.org/release/SDL2-devel-2.0.10-mingw.tar.gz'
mingw_filename = 'SDL2-devel-2.0.10-mingw.tar.gz'
mingw_lib32 = 'SDL2-2.0.10/i686-w64-mingw32/lib'
mingw_lib64 = 'SDL2-2.0.10/x86_64-w64-mingw32/lib'
mingw_include = 'SDL2-2.0.10/x86_64-w64-mingw32/include'

try:
	input = raw_input
except:
	pass

def download_file(url, local_filename):
	# NOTE the stream=True parameter below
	with requests.get(url, stream=True) as r:
		r.raise_for_status()
		with open(local_filename, 'wb') as f:
			for chunk in r.iter_content(chunk_size=8192): 
				if chunk: # filter out keep-alive new chunks
					f.write(chunk)
					# f.flush()
	return local_filename

def msvc_procedure():
	download_file(msvc_url, msvc_filename)
	
	try:
		shutil.rmtree(exfolder)
	except FileNotFoundError:
		pass
		
	with zipfile.ZipFile(msvc_filename, 'r') as f:
		f.extractall()
		
	os.remove(msvc_filename)
	
	shutil.copytree(msvc_lib, 'external/sdl2-precompiled/msvc/libs')
	shutil.copytree(msvc_include, 'external/sdl2-precompiled/msvc/include/SDL2')
	
	try:
		shutil.rmtree(exfolder)
	except FileNotFoundError:
		pass

def mingw_procedure():
	download_file(mingw_url, mingw_filename)
	
	try:
		shutil.rmtree(exfolder)
	except FileNotFoundError:
		pass
		
	with tarfile.open(mingw_filename, 'r:gz') as f:
		f.extractall()
	
	os.remove(mingw_filename)
	
	shutil.copytree(mingw_lib32, 'external/sdl2-precompiled/mingw/libs/x86')
	shutil.copytree(mingw_lib64, 'external/sdl2-precompiled/mingw/libs/x64')
	shutil.copytree(mingw_include, 'external/sdl2-precompiled/mingw/include')
	
	try:
		shutil.rmtree(exfolder)
	except FileNotFoundError:
		pass

if len(sys.argv) != 2:
	compiler = input('MSVC or MinGW? ').lower()
else:
	compiler = sys.argv[1]

if not compiler in ['msvc', 'mingw']:
	print('Please enter "msvc" or "mingw"')
	sys.exit(1)

if compiler == 'msvc':
	msvc_procedure()
elif compiler == 'mingw':
	# Important note: MSVC files are required even if the compiler is mingw because mingw releases doesn't provide a DLL for dynamic linking
	msvc_procedure()
	mingw_procedure()

print('Done!')
