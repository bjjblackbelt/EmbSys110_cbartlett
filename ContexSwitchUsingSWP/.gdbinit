target remote localhost:3333
show remotetimeout
set remotetimeout 180
monitor arm7_9 dcc_downloads enable
monitor arm7_9 fast_memory_access enable
monitor reset init
monitor wait_halt
monitor poll
monitor flash probe 0
monitor flash erase_sector 0 0 26
monitor reset init
monitor wait_halt
monitor poll
monitor flash write_image erase /home/ubu/Projects/EmbSys110_cbartlett/ContexSwitchUsingSWP/ContexSwitchUsingSWP.hex 0 ihex
symbol-file ContexSwitchUsingSWP.elf
monitor soft_reset_halt
set remotetimeout 10
break main
continue
