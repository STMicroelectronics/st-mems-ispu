CC = stred-gcc
OBJ_COPY = stred-objcopy
OBJ_DUMP = stred-objdump
SIZE = stred-size
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

DEFINE = -DEXPORT_RUNTIME_LITE_APIS -DHAVE_NETWORK_INFO=1

INCS = -I../ispu_utils -I../inc -I../inc/ai

CFLAGS = -mcpu=stredl -mfp32-format=ieee
CFLAGS += -Os -ffunction-sections -fdata-sections -fno-strict-aliasing -Wall -Wextra -Wdouble-promotion
CFLAGS += $(DEFINE) $(INCS)

LDFLAGS = -T ../ispu_utils/boot.ld -Wl,--gc-sections -specs=nano.specs
LDFLAGS += -L../lib/ai

LDLIBS = -lNetworkRuntime_ISPU_GCC

SRC = ../ispu_utils/crt0.S ../src/main.c ../src/network_data.c ../src/network.c ../src/network_utils.c
SRC += ../ispu_utils/ispu_float_conv.S

IGFLAGS = -d imu_22 -s ../conf.txt -m ../meta.txt -sh ../shub.txt -n ispu_conf -nd

OUTDIR = bin
NAME = ispu
OUTPUT = $(OUTDIR)/$(NAME)

build: $(SRC) $(OUTDIR) Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(OUTPUT) $(LDLIBS)
	$(SIZE) $(OUTPUT)
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

