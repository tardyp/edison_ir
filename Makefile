test: src/mcu_main.c fakemcu.c
	gcc -o test src/mcu_main.c fakemcu.c -Iinclude
