includes("EngineUtils")

static_module("Core", EngineVersion, "Core.cpp")

shared_module("CoreModule", EngineVersion, "CoreModule.cpp")
    add_deps("Core", "SPA")