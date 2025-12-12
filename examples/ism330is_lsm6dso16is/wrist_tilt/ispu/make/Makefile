CC = stred-gcc
OBJ_COPY = stred-objcopy
OBJ_DUMP = stred-objdump
SIZE_REISC = stred-size
ISPU_GEN = ispu_gen

MKDIR = mkdir -p
RMDIR = rm -rf
RM = rm -f

ifeq ($(OS),Windows_NT)
    null =
    space = $(null) $(null)
    prep_path = $(subst $(1),$(space),$(subst $(space),?,$(subst \,/,$(PATH))))
    find_in_path = $(strip $(wildcard $(addsuffix /$(1),$(call prep_path,$(2)))))

    RM_LOC = $(call find_in_path,rm.exe,;)$(call find_in_path,rm.exe,:)
    ifeq ($(RM_LOC),)
        SHELL = cmd.exe
        RMDIR = rmdir /S /Q
        RM = del /F /Q
    endif

    MKDIR_LOC = $(call find_in_path,mkdir.exe,;)$(call find_in_path,mkdir.exe,:)
    ifeq ($(MKDIR_LOC),)
        SHELL = cmd.exe
        MKDIR = mkdir
    else
        NULL := $(shell mkdir -p 2> tmp)
        ifneq (,$(wildcard -p))
            MKDIR = mkdir
            NULL := $(shell $(RMDIR) "./-p")
        endif
        NULL := $(shell $(RM) tmp)
    endif
endif

CFLAGS = -mcpu=stredl -Os -specs=nano.specs -ffunction-sections -fdata-sections -Wl,--gc-sections -mfp32-format=ieee -Wall -Wextra -Wdouble-promotion -fno-strict-aliasing
INCS = -I../ispu_utils -I../lib
LDSCRIPT = ../ispu_utils/boot.ld
LDFLAGS = -L../lib -lMotionWT

SRC = ../ispu_utils/crt0.S ../src/main.c
SRC += ../ispu_utils/ispu_float_conv.S

IGFLAGS = -d imu_22 -s ../conf.txt -m ../meta.txt -sh ../shub.txt -n ispu_conf -nd

OUTDIR = bin
NAME = ispu
OUTPUT = $(OUTDIR)/$(NAME)

build: $(SRC) $(OUTDIR) Makefile
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC) $(LDFLAGS) -T $(LDSCRIPT) $(INCS)
	$(SIZE_REISC) $(OUTPUT)
	$(OBJ_COPY) -O srec $(OUTPUT) $(OUTPUT).srec
	$(ISPU_GEN) $(IGFLAGS) -c $(OUTPUT).srec > $(OUTPUT).h
	$(ISPU_GEN) $(IGFLAGS) $(OUTPUT).srec > $(OUTPUT).json

$(OUTDIR):
	$(MKDIR) $(OUTDIR)

clean:
ifneq (,$(wildcard $(OUTDIR)))
	$(RMDIR) $(OUTDIR)
endif

.PHONY: build clean

