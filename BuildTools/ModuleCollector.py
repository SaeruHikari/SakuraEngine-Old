'''
@Author: your name
@Date: 2020-02-16 14:02:58
@LastEditTime: 2020-04-15 01:22:16
@LastEditors: Please set LastEditors
@Description: In User Settings Edit
@FilePath: /JustPlug-master/home/saeruhikari/Coding/SakuraEngine/SPA/StaticModuleCollector.py
'''
#This is a collector of modules in SakuraEngine
# Created by SaeruHikari, with the MIT License Copyright.
import glob
import os
import sys 
import json

jsonDict = dict()
pathDict = dict()
tokenDict = dict()
versionDic = dict()
MainModuleName=str()
header = str()

class FError(Exception):
    pass

def deal_header(json_name, file_path):
    global header
    global tokenDict
    try:
        EntryJson=jsonDict[json_name]
        if tokenDict[EntryJson['name']] == True:
            return
        tokenDict[EntryJson['name']] = True
        if EntryJson['linking'] == 'static' :
            pa = pathDict[EntryJson['name']].replace( \
            file_path.rstrip('/')+'/', '', 1)
            pa = os.path.dirname(pa) +'/'+EntryJson['name']+'.h'
            header += '#include "' + pa + '"\n'
        for dependency in EntryJson['dependencies']:
            if jsonDict[dependency['name']]['version'] == dependency['version']:
                deal_header(dependency['name'], file_path)
            else:
                raise FError("VERSION ERROR")
    except FError as e:
        print(json_name+': Module version of '+dependency['name']+' not matching')
    except KeyError as ek:
        print(ek)
    return

def deal_header_main(file_path):
    global header
    header = '//A header file genereate by Sakura Module tool\n'
    header += '//Contains the header of static modules of Sakura Engine\n'
    header += '//With the MIT License Copyright!\n'
    header += '#pragma once\n'
    global MainModuleName
    mainEntryJson=jsonDict[MainModuleName]
    for dependency in mainEntryJson['dependencies']:
        try:
            if jsonDict[dependency['name']]['version'] == dependency['version']:
                deal_header(dependency['name'], file_path)
            else:
                raise FError("VERSION ERROR")
        except FError as e:
            print('MainModule: Module version of '+dependency['name']+' not matching')
        except KeyError as ek:
            print(ek)
    return

def collect_meta(filereg='meta', file_path=os.getcwd(), engineApi='0.1.0'):
    if os.path.isfile(file_path):
        if file_path.find(filereg+'.json') >= 0:
            f=open(file_path, encoding='utf-8')
            user_dic=json.load(f)
            if file_path.find('main'+filereg+'.json') >= 0:
                global MainModuleName
                MainModuleName += user_dic['name']
            jsonDict[user_dic['name']]=user_dic
            pathDict[user_dic['name']]=file_path
            tokenDict[user_dic['name']]=False
            versionDic[user_dic['name']]=user_dic['version']
            if user_dic['api'] != engineApi:
                try:
                    raise FError("API ERROR")
                except FError as e:
                    print('Engine api version of '+user_dic['name']+' not matching')
    else:           
        for file_ls in os.listdir(file_path):
            collect_meta(filereg, os.path.join(file_path,file_ls), engineApi) 
    return

try:
    global fw
    if(len(sys.argv) == 1):
        collect_meta()
    elif(len(sys.argv) == 2):
        collect_meta(sys.argv[1])
    elif(len(sys.argv) == 3):
        fw = open(sys.argv[2]+'/SPAModules.generated.h', 'w')
        collect_meta(sys.argv[1], sys.argv[2])
    elif(len(sys.argv) == 4):
        fw = open(sys.argv[2]+'/SPAModules.generated.h', 'w')
        collect_meta(sys.argv[1], sys.argv[2], sys.argv[3])
    deal_header_main(os.getcwd())
    fw.write(header)
except KeyError as ek:
    print(ek)