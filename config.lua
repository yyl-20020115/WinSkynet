root = "./"
thread = 8
logger = nil
harbor = 1
address = "127.0.0.1:2527"
master = "127.0.0.1:2016"
start = "main"	--main script
bootstrap = "snlua bootstrap"	--The service for bootstrap
standalone = "0.0.0.0:2013"
luaservice = root.."service/?.lua;"..root.."test/?.lua;"..root.."examples/?.lua"
lualoader = "lualib/loader.lua"
snax = root.."examples/?.lua;"..root.."test/?.lua"
cpath = root.."cservice/?.dll"
