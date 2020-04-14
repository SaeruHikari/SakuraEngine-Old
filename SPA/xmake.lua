shared_lib("SPA", EngineVersion, "src/*.cpp", "include/whereami/src/*.c")
    on_load(function(target)
        print("SPA"..target:get("version"))
    end)
