### CapsLockSwitcher

Makes CapsLock change language instead of SHOUTING

*MinGW*  
replace icon.ico with your own, then  
`windres capslock_switcher.rc -o resource.o`  
`g++ capslock_switcher.cpp resource.o -o CapsLockSwitcher.exe -mwindows -static`  
