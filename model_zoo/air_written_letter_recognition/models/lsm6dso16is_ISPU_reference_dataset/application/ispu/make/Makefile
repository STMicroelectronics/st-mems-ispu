CC = reisc-gcc
OBJ_COPY = reisc-objcopy
OBJ_DUMP = reisc-objdump
SIZE_REISC = reisc-size
ISPU_GEN = ispu_gen

CP = cp
MKDIR = mkdir -p
RMDIR = rm -rf
RM = rm -f

ifeq ($(OS),Windows_NT)
    ifeq (,$(shell where rm 2> tmp))
        RMDIR = rmdir /S /Q
        RM = del /F /Q
        CP = copy
    endif
    NULL := $(shell mkdir -p 2> tmp)
    ifneq (,$(wildcard -p))
        MKDIR = mkdir
        NULL := $(shell $(RMDIR) "./-p")
    endif
    NULL := $(shell $(RM) tmp)
endif

CFLAGS = -mcpu=reiscl -Os -specs=nano.specs -ffunction-sections -fdata-sections -Wl,--gc-sections -mfp32-format=ieee -Wall -Wextra -Wdouble-promotion -fno-strict-aliasing -DEXPORT_RUNTIME_LITE_APIS -DHAVE_NETWORK_INFO=1
INCS = -I../ispu_utils -I../inc -I../inc/ai
LDFLAGS = -L../lib/ai -lNetworkRuntime_ISPU_GCC

SRC = ../ispu_utils/crt0.S ../src/main.c ../src/network_data.c ../src/network.c ../src/network_utils.c ../src/iir2.c

EXTRA_FILES= -s ../conf.txt -m ../meta.txt -sh ../shub.txt

OUTDIR = bin
EXPORTDIR = ../../output
NAME = ispu

build: $(SRC) $(OUTDIR) $(EXPORTDIR) Makefile
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(NAME) $(SRC) $(LDFLAGS) -T ../ispu_utils/boot.ld $(INCS)
	$(SIZE_REISC) $(OUTDIR)/$(NAME)
	$(OBJ_COPY) -O srec $(OUTDIR)/$(NAME) $(OUTDIR)/$(NAME).srec
	$(ISPU_GEN) -c $(EXTRA_FILES) -d imu_22 -n ispu_conf -nd $(OUTDIR)/$(NAME).srec > $(OUTDIR)/$(NAME).h
	$(ISPU_GEN) $(EXTRA_FILES) -d imu_22 -n ispu_conf -nd $(OUTDIR)/$(NAME).srec > $(OUTDIR)/$(NAME).json
	$(CP) $(OUTDIR)/$(NAME).h $(EXPORTDIR)/ispu_wand.h
	$(CP) $(OUTDIR)/$(NAME).json $(EXPORTDIR)/ispu_wand.json

$(OUTDIR):
	$(MKDIR) $(OUTDIR)

$(EXPORTDIR):
	$(MKDIR) $(EXPORTDIR)

clean:
ifneq (,$(wildcard $(OUTDIR)))
	$(RMDIR) $(OUTDIR)
endif
ifneq (,$(wildcard $(EXPORTDIR)))
	$(RMDIR) $(EXPORTDIR)
endif

.PHONY: build clean

