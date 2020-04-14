includes("EngineUtils")

static_module("Core", EngineVersion, "Core.cpp")
    on_load(function(target)
        print(target:get("version"))
    end)

shared_module("CoreModule", EngineVersion, "CoreModule.cpp")
    add_deps("Core", "SPA")