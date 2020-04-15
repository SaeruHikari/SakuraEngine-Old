shared_lib("SPA", EngineVersion, "src/*.cpp", "../Extern/include/whereami/src/*.c")
    --before_build(function(target)
    --    print("SPA "..target:get("version"))
    --end)
