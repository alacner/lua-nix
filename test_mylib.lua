require "mylib"

local pid = mylib.getpid()
print(pid)

local pwd = mylib.pwd()
print(pwd)

for i=1,5 do
    mylib.sleep(1)
    print(pid)
end
