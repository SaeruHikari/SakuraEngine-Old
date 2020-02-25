'''
@This File is Part of Sakura by SaeruHikari: 
@Description: Copyright SaeruHikari
@Version: 0.1.0
@Autor: SaeruHikari
@Date: 2020-02-25 18:32:17
@LastEditors: SaeruHikari
@LastEditTime: 2020-02-25 23:35:44
'''
import urllib.request
import os
import zipfile

def UnPack(workdir, file):
    print("Unpack zip file: " + workdir + file)
    with zipfile.ZipFile(file, 'r') as zip:
        zip.extractall(workdir+"\\_internal")
        with open(os.getcwd() + "\\_internal\\CMakeLists.txt", "w") as CMakeLists:
            CMakeLists.write("add_subdirectory("+"SDL2-2.0.10"+")\n")
    return

   
if os.access(os.getcwd()+"\\_internal" + "\\SDL2-2.0.10", os.F_OK):
    print("SDL2 Loaded!")
    with open(os.getcwd() + "\\_internal\\CMakeLists.txt", "w") as CMakeLists:
        CMakeLists.write("add_subdirectory("+"SDL2-2.0.10"+")\n")
else:
    if os.access(os.getcwd() + "\\SDL2.zip", os.F_OK):
        print("SDL2 Already Exists!")
        UnPack(os.getcwd(), os.getcwd()+"\\SDL2.zip")
        print("SDL2 Loaded!")
    else:
        print("Download SDL2: " + os.getcwd() + "\\SDL2-2.0.10.zip")
        url = 'http://www.libsdl.org/release/SDL2-2.0.10.zip'
        f = urllib.request.urlopen(url) 
        data = f.read() 
        with open(os.getcwd() + "\\SDL2.zip", "wb") as code:     
            code.write(data)
            UnPack(os.getcwd(), os.getcwd()+"\\SDL2.zip")
            print("SDL2 Loaded!")
