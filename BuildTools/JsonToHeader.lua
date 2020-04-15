lfs = require "lfs"
json = require "BuildTools/json"

JsonFileModify = nil
for file in lfs.dir(arg[1]) do
    local ff = arg[1].."/"..file
    if file == arg[2]..".json" then
        local attr = lfs.attributes(ff)
        JsonFile = ff
        JsonFileModify = attr.modification
    end
    if file == arg[2]..".h" then
        local attr = lfs.attributes(ff)
        MetaHeader = ff
        MetaHeaderModify = attr.modification
    end
end
if JsonFileModify == nil then
    print("No meta.json in the dir "..arg[1].." of Plugin.")
elseif MetaHeaderModify == nil or MetaHeaderModify <= JsonFileModify then
    print(arg[1].." Refreshing meta header.")
    JsF = io.open(JsonFile, "r")
    HdrF = io.open(arg[1].."/"..arg[2]..".h", "w")
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
end
