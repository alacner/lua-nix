require "mylib"

local pwd = mylib.pwd()
print(pwd)

local pid = mylib.getpid()
print(pid)

for i=1,5 do
    mylib.sleep(1)
    print(pid)
end
