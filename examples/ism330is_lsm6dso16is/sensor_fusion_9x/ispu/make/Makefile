CC = reisc-gcc
OBJ_COPY = reisc-objcopy
OBJ_DUMP = reisc-objdump
SIZE_REISC = reisc-size
ISPU_GEN = ispu_gen

MKDIR = mkdir -p
RMDIR = rm -rf
RM = rm -f

ifeq ($(OS),Windows_NT)
    ifeq (,$(shell where rm 2> tmp))
        RMDIR = rmdir /S /Q
        RM = del /F /Q
    endif
    NULL := $(shell mkdir -p 2> tmp)
    ifneq (,$(wildcard -p))
        MKDIR = mkdir
        NULL := $(shell $(RMDIR) "./-p")
    endif
    NULL := $(shell $(RM) tmp)
endif

CFLAGS = -mcpu=reiscl -Os -specs=nano.specs -ffunction-sections -fdata-sections -Wl,--gc-sections -mfp32-format=ieee -Wall -Wextra -Wdouble-promotion -fno-strict-aliasing
INCS = -I../ispu_utils -I../lib
LDFLAGS = -L../lib -lMotionMC -lMotionFX

SRC = ../ispu_utils/crt0.S ../src/main.c

OUTDIR = bin
NAME = ispu

build: $(SRC) $(OUTDIR) Makefile
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(NAME) $(SRC) $(LDFLAGS) -T ../ispu_utils/boot.ld $(INCS)
	$(SIZE_REISC) $(OUTDIR)/$(NAME)
	$(OBJ_COPY) -O srec $(OUTDIR)/$(NAME) $(OUTDIR)/$(NAME).srec
	$(ISPU_GEN) -b -c -s ../conf.txt -d imu_22 -n ispu_conf $(OUTDIR)/$(NAME).srec > $(OUTDIR)/$(NAME).h
	$(ISPU_GEN) -b -s ../conf.txt -d imu_22 -n ispu_conf $(OUTDIR)/$(NAME).srec > $(OUTDIR)/$(NAME).ucf

$(OUTDIR):
	$(MKDIR) $(OUTDIR)

clean:
ifneq (,$(wildcard $(OUTDIR)))
	$(RMDIR) $(OUTDIR)
endif

.PHONY: build clean

