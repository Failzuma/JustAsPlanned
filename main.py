import ctypes, utility
from ctypes import wintypes
from consts import *

kernel32 = ctypes.windll.kernel32

pid = utility.GetProcId("MuseDash.exe")
handle = kernel32.OpenProcess(PROCESS_ALL_ACCESS, 0, ctypes.wintypes.DWORD(pid))
base_address = utility.GetModuleBaseAddress(pid, "GameAssembly.dll")

# Feature
features = [
    (ctypes.c_ulonglong(base_address + 0x130B580), ctypes.c_int(0x0001B848)),
    (ctypes.c_ulonglong(base_address + 0x130BC80), ctypes.c_int(0xDE4785B8)), 
    (ctypes.c_ulonglong(base_address + 0x223C020), ctypes.c_int(0x0001B848)), 
]
# Write to memory
for address, value in features:
    kernel32.WriteProcessMemory(handle, address, ctypes.byref(value), ctypes.sizeof(value), None)
print("Succesfully unlocked everything!")
kernel32.CloseHandle(handle) 

