set_defaultmode("debug")

target("my_redis")
    set_kind("binary")
    set_symbols("debug")

    add_includedirs("$(projectdir)")
    add_includedirs("third")
    add_includedirs("src")
    
    add_files("src/*.cpp")

    -- 添加静态库文件
    add_linkdirs("third/hiredis")
    add_links("libhiredis.a")

