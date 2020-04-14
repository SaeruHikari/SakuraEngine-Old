target("PrevOp")
    on_load(function(target)
        os.exec("python3 SPA/anyToheader.py meta .json SPAUnitTest/")
        os.exec("python3 SPA/ModuleCollector.py meta SPAUnitTest/")
    end)
    

includes("plugin_0", "plugin_1", "plugin_main")

target("UnitTestSPA")   
    set_kind("binary")
    add_files("UnitTestSPA.cpp")
    add_deps("SPA", "plugin_0", "plugin_1", "plugin_main")