--[[
 * limit the process numbers
 * this process is only can use in linux system and with mylib
 * author: alacner zhang <alacner@gmail.com>

 example:
   process_limit(argv, 3)
   process_limit(__FILE__)
   process_limit(__FILE__, 1, true) -- display debug info
--]]

require "print_r"
require "mylib"

function process_limit(...)
    local process, number, debug, path = ...
    local args = {number = 1, debug = false, path = '/tmp/'}

    args.process = type(process) == "table" and process[0] .. '-' .. table.concat(process, '-') or process
    args.match = type(process) == "table" and process[0] .. '.*' .. table.concat(process, '.*') or process
    args.number = number or args.number
    args.debug = debug or args.debug
    args.path = path or args.path

    local a = {}
    
    for i=1,#args.process do
        table.insert(a, string.byte(args.process, i))
    end
    args.process = table.concat(a, '~')
    --print_r(args)
    -- get os from shell
    function is_linux()
        local os_tmp_file = args.path .. 'LUA_OS'
        local f = io.open(os_tmp_file)
        if not f then
            os.execute('uname | xargs > ' .. os_tmp_file)
            local f = io.open(os_tmp_file)
        end
        if f then 
            local lua_os = f:read("*a")
            f:close()
            lua_os = lua_os:lower() 
            return lua_os:match('linux')
        else
            return false
        end
    end
    -- check os is linux 
    if not is_linux() then
        print("This function only use in linux system!");
        os.exit()
    end
    -- split
    -- Compatibility: Lua-5.1
    function split(str, pat)
        local t = {}  -- NOTE: use {n = 0} in Lua-5.0
        local fpat = "(.-)" .. pat
        local last_end = 1
        local s, e, cap = str:find(fpat, 1)
        while s do
            if s ~= 1 or cap ~= "" then
                table.insert(t,cap)
            end
            last_end = e+1
            s, e, cap = str:find(fpat, last_end)
        end
        if last_end <= #str then
            cap = str:sub(last_end)
            table.insert(t, cap)
        end
        return t
    end
    -- debug message    
    function debug(str)
        if args.debug then
            print(str)
        end
    end

    local tpids = {}
    local pfile = args.path .. "process_lua_" .. args.process
    local f = io.open(pfile)
    if f then
        local pf = f:read("*a")
        tpids = split(pf, "|")
    end
    
    local pids = {}
    for i,p in ipairs(tpids) do
        local proc = string.format('/proc/%d/cmdline', p)
        local f = io.open(proc)
        if f then
            local cmdline = f:read('*a')
            if cmdline:match(args.match) then
                table.insert(pids, p)
            end
        end
    end
    
    if #pids > 0 then
        debug(string.format("process list: %s", table.concat(pids, ',')))
    end
    
    local this_pid = mylib.getpid()
    table.insert(pids, this_pid)  
    debug(string.format("current pid: %d", this_pid))
    if args.number >= #pids then
        debug(string.format("status: %d/%d, continue ...", #pids, args.number))
    else
        debug(string.format("ready runing: %d, exit now!", args.number))
        os.exit()
    end
    local f = io.open(pfile, 'wb')
    f:write(table.concat(pids, "|"))
    f:close()
end

--process_limit(arg[0])
--process_limit(arg, 2)
--mylib.sleep(2200000)
