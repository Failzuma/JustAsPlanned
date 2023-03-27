import ctypes
from ctypes import wintypes

PROCESS_ALL_ACCESS = (0x000F0000 | 0x00100000 | 0xFFFF)
PAGE_EXECUTE_READWRITE = 0x40
TH32CS_SNAPMODULE = 0x8
TH32CS_SNAPMODULE32 = 0x10
TH32CS_SNAPPROCESS = 0x2
INVALID_HANDLE_VALUE = -1

class PROCESSENTRY32(ctypes.Structure):
    _fields_ = [('dwSize', ctypes.wintypes.DWORD),
                ('cntUsage', ctypes.wintypes.DWORD),
                ('th32ProcessID', ctypes.wintypes.DWORD),
                ('th32DefaultHeapID', ctypes.POINTER(ctypes.wintypes.ULONG)),
                ('th32ModuleID', ctypes.wintypes.DWORD),
                ('cntThreads', ctypes.wintypes.DWORD),
                ('th32ParentProcessID', ctypes.wintypes.DWORD),
                ('pcPriClassBase', ctypes.wintypes.LONG),
                ('dwFlags', ctypes.wintypes.DWORD),
                ('szExeFile', ctypes.c_char * 260)]
    

class MODULEENTRY32(ctypes.Structure):
    _fields_ = [('dwSize', ctypes.wintypes.DWORD),
                ('th32ModuleID', ctypes.wintypes.DWORD),
                ('th32ProcessID', ctypes.wintypes.DWORD),
                ('GlblcntUsage', ctypes.wintypes.DWORD),
                ('ProccntUsage', ctypes.wintypes.DWORD),
                ('modBaseAddr', ctypes.POINTER(ctypes.wintypes.BYTE)),
                ('modBaseSize', ctypes.wintypes.DWORD),
                ('hModule', ctypes.wintypes.HMODULE),
                ('szModule', ctypes.c_char * 256),
                ('szExePath', ctypes.c_char * 260)]