require "nix"

local pwd = nix.pwd()
print(pwd)

local path = '/root/a/b/c/d.html'
local dn,bn,ext = nix.pathinfo(path)
print(path)
print(dn)
print(bn)
print(ext)

print(nix.microtime())

print(nix.md5(12345));
print(nix.md5(12345, 1));

print(nix.crc32(12345));
print(nix.crc32(12345, 1));

local pid = nix.getpid()
print(pid)

for i=1,3 do
    nix.sleep(1)
    print(pid)
end
