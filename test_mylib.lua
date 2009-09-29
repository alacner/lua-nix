require "mylib"

local pwd = mylib.pwd()
print(pwd)

local path = '/root/a/b/c/d.html'
local a,dn,bn,ext = mylib.pathinfo(path)
print(path)
print(a)
print(dn)
print(bn)
print(ext)

local pid = mylib.getpid()
print(pid)

for i=1,5 do
    mylib.sleep(1)
    print(pid)
end
