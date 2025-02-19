### CapsLockSwitcher

Makes CapsLock change language instead of SHOUTING

*MinGW*
windres capslock_switcher.rc -o resource.o
g++ capslock_switcher.cpp resource.o -o CapsLockSwitcher.exe -mwindows -static
