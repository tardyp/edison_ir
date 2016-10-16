CROSSCOMPILE=$(MCUSDK_PATH)/toolchain/$(MCUSDK_OS)/i686-elf-gcc/bin/i686-elf-
CC := $(CROSSCOMPILE)gcc
CPPFLAGS :=  -Iinclude

SRC = src/mcu_main.c
FAKESRC = testing/fakemcu.c
OBJ = $(patsubst %.c,%.o,$(SRC))

intel_mcu.bin: $(OBJ)
	./scripts/generate_mcu_bin.sh $+


# for test, we build with host
test: $(SRC) $(FAKESRC) Makefile
	gcc -o test  $(SRC) $(FAKESRC) -Iinclude -Werror
