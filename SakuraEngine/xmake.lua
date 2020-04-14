-- add modes: debug and release
add_rules("mode.debug", "mode.release")

set_languages("c99", "c++20")

add_defines("SAKURA_COMPILER_MSVC")

add_includedirs("../Extern/include")
add_includedirs("./")

-- Generate Engine Module Metas
function ConfigureEngine()
    target("Sakura")
        on_load(function()
            os.exec("python3 ../SPA/anyToheader.py meta .json ../SakuraEngine")
            os.exec("python3 ../SPA/ModuleCollector.py meta ../SakuraEngine")
        end)
end

ConfigureEngine()

includes("Core")

