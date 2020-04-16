lfs = require "lfs"
json = require "BuildTools/json"
-- arg[1] prefix
-- arg[2] script-path 
-- arg[3] root-path
JsonFileModify = nil
for file in lfs.dir(arg[2]) do
    local ff = arg[2].."/"..file
    if file == arg[1]..".json" or file == "main"..arg[1]..".json"then
        local attr = lfs.attributes(ff)
        JsonFile = ff
        JsonFileModify = attr.modification
    end
    if file == arg[1]..".h" then
        local attr = lfs.attributes(ff)
        MetaHeader = ff
        MetaHeaderModify = attr.modification
    end
end



if JsonFileModify == nil then
    print("No meta.json in the dir "..arg[2].." of Plugin.")
elseif MetaHeaderModify == nil or MetaHeaderModify <= JsonFileModify then
    print(arg[2].." Refreshing meta header.")
    JsF = io.open(JsonFile, "r")
    ModuleMeta = json.decode(JsF:read("*a"))
    HdrF = io.open(arg[2].."/"..arg[1]..".h", "w")
    io.output(HdrF)
    io.write("//A header file genereate by Sakura J2H tool\n")
    io.write("//Contains the infomation of a module of Sakura Engine\n")
    io.write("//With the MIT License Copyright!\n")
    io.write("#pragma once\n")
    io.write("#include <string>\n")
    io.write("#include <cstddef>\n\n")
    io.write("const std::string sp_meta = \nR\"(")  
    io.write(JsF:read("*a")..")\";\n")
    io.write("inline const char* __GetMetaData(void)\n{\n    return sp_meta.c_str();\n}\n")
    io.write("public:\nvirtual const char* GetMetaData(void) override\n")
    io.write("{return __GetMetaData();}\n")
    io.close(JsF)
    io.close(HdrF)
-- include static module files
    GenHeader = io.open(arg[3].."/".."Modules.generated.h"..".h", "a") 
    io.output(GenHeader)
    io.write("//A header file genereate by Sakura J2H tool\n")
end
