SET PATH=C:\Keil\ARM\BIN40;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\Microsoft\Web Platform Installer\;C:\Program Files (x86)\Microsoft ASP.NET\ASP.NET Web Pages\v1.0\;C:\Program Files (x86)\Windows Kits\8.0\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;F:\Program Files (x86)\MATLAB\R2012b\runtime\win64;F:\Program Files (x86)\MATLAB\R2012b\bin;C:\Program Files (x86)\IDM Computer Solutions\UltraCompare\
SET ARMCC41INC=C:\Keil\ARM\RV31\Inc
SET ARMCC41LIB=C:\Keil\ARM\RV31\Lib
SET CPU_TYPE=LPC1768
SET CPU_VENDOR=NXP (founded by Philips)
SET UV2_TARGET=UCOS_II_V289
SET CPU_CLOCK=0x00B71B00
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\core_cm3.__i"
"C:\Keil\ARM\BIN40\ArmAsm" --Via ".\output\obj\startup_lpc17xx._ia"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\system_lpc17xx.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\hello.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_tmr.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_core.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_flag.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_mbox.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_mem.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_mutex.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_q.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_sem.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_task.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_time.__i"
"C:\Keil\ARM\BIN40\ArmAsm" --Via ".\output\obj\os_cpu_a._ia"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\output\obj\os_cpu_c.__i"
"C:\Keil\ARM\BIN40\ArmLink" --Via ".\Output\obj\UCOS_II_V289.lnp"
C:\Keil\ARM\BIN40\fromelf.exe ".\Output\obj\UCOS_II_V289.axf" --i32combined --output ".\Output\obj\UCOS_II_V289.hex"
