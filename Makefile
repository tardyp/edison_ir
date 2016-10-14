test: src/mcu_main.c fakemcu.c Makefile
	gcc -o test src/mcu_main.c fakemcu.c -Iinclude -w
