'''
@Author: your name
@Date: 2020-02-15 21:10:01
@LastEditTime: 2020-02-20 13:10:28
@LastEditors: Please set LastEditors
@Description: In User Settings Edit
@FilePath: /JustPlug-master/home/saeruhikari/Coding/SakuraEngine/SPA/anyToheader.py
'''
#Json To Header is a header file generator that 
#hard codes json file to c++ header as const char[]
#Created by SaeruHikari, with the MIT License Copyright.
import glob
import os
import sys 

def json2header(filereg, file_path):
    f=open(file_path, encoding='utf-8')
    content=f.read()
    f.close()
    fw = open( os.path.dirname(file_path).rstrip('/') + '/' + filereg +'.h', 'w')
    header = '//A header file genereate by Sakura J2H tool\n'
    header += '//Contains the infomation of a module of Sakura Engine\n'
    header += '//With the MIT License Copyright!\n'
    header += '#pragma once\n'
    header += '#include <tuple>\n'
    header += '#include <cstddef>\n\n'
    header += 'inline static const char sp_meta[] = {\n'  
    counter = 0
    for _byte in content.encode():
        if counter >= 12:
            header += '\n' 
            counter = 0 
        header += hex(_byte)
        header += ', '
        counter=counter+1
    header = header.rstrip()
    header = header.rstrip(',')
    header += '\n};\n\n'
    header += 'inline static const std::size_t sp_meta_size = '
    header += str(len(content.encode()))
    header += ';\n\n'
    header += 'inline static std::size_t __GetMetaSize(void)\n{\n    return sp_meta_size;\n}\n'
    header += 'inline static const char* __GetMetaData(void)\n{\n    return sp_meta;\n}\n'
    header += 'public:\nvirtual const char* GetMetaData(void) override\n'
    header += '{return __GetMetaData();}\n'
    header += 'public:\nvirtual std::size_t GetMetaSize(void) override\n'
    header += '{return __GetMetaSize();}\n'
    fw.write(header)  
    return
    
def find_meta(filereg='meta', tailname='.json', file_path=os.getcwd()):
    if os.path.isfile(file_path):
        if file_path.find(filereg+tailname) >= 0:
            json2header(filereg, file_path)
    else:           
        for file_ls in os.listdir(file_path):
            find_meta(filereg, tailname, os.path.join(file_path,file_ls))
    return 

if(len(sys.argv) == 1):
    find_meta()
elif(len(sys.argv) == 2):
    find_meta(sys.argv[1])
elif(len(sys.argv) == 3):
    find_meta(sys.argv[1], sys.argv[2])
elif(len(sys.argv) == 4):
    find_meta(sys.argv[1], sys.argv[2], sys.argv[3])


