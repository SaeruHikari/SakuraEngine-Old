import glob
import os

file_list = list() 

def json2header(file_path):
    f=open(file_path, encoding='utf-8')
    content=f.read()
    f.close()
    fw = open( os.path.dirname(file_path).rstrip('/') + '/meta.h', 'w')
    header = '//A header file genereate by Sakura J2H tool\n'
    header += '//Contains the infomation of a module of Sakura Engine\n'
    header += '//With the MIT LicenseLicense Copyright!\n'
    header += '#pragma once\n'
    header += '#include <tuple>\n'
    header += '#include <cstddef>\n\n'
    header += 'const char sp_meta[] = {\n'  
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
    header += 'const std::size_t sp_meta_size = '
    header += str(len(content.encode()))
    header += ';\n\n'
    header += 'inline std::tuple<const char*, std::size_t> GetMeta(void)\n'
    header += '{\n'
    header += '    return std::tuple<const char*, std::size_t>'
    header += '(sp_meta, sp_meta_size);\n'
    header += '}\n'
    header += 'inline std::size_t GetMetaSize(void)\n{\n    return sp_meta_size;\n}\n'
    header += 'inline const char* GetMetaData(void)\n{\n    return sp_meta;\n}\n'
    fw.write(header)  
    return
def find_meta(file_path):
    if os.path.isfile(file_path):
        if file_path.find('meta.json') >= 0:
            json2header(file_path)
        else:
            print(file_path)
    else:           
        for file_ls in os.listdir(file_path):
            find_meta(os.path.join(file_path,file_ls))
    return file_list

find_meta(os.getcwd()) 
