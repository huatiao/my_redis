add_rules("mode.debug")

target("my_redis")
    set_kind("binary")

    add_includedirs("third")
    add_files("src/*.cpp")

    -- 添加静态库文件
    add_linkdirs("third/hiredis")
    add_links("libhiredis.a")

