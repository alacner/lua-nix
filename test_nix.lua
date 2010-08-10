require "nix"

local pwd = nix.pwd()
print(pwd)

local path = '/root/a/b/c/d.html'
local dn,bn,ext = nix.pathinfo(path)
print(path)
print(dn)
print(bn)
print(ext)

local pid = nix.getpid()
print(pid)

for i=1,5 do
    nix.sleep(1)
    print(pid)
end
print(nix.microtime())
