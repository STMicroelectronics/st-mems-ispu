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

FAST_FLOAT = 0 # enable all optional optimizations for float operations
FAST_SQRTF = 1 # enable hw accelerated sqrtf (does not validate input)
FAST_DIVF = 0 # enable optimized float division (do not use if 100% accurate results are needed)
FAST_FLOAT_CMP = 0 # enable optimized comparison between float numbers (does not handle NaN)

DEFINE =  -DFAST_FLOAT=$(FAST_FLOAT)
DEFINE += -DFAST_SQRTF=$(FAST_SQRTF)
DEFINE += -DFAST_DIVF=$(FAST_DIVF)
DEFINE += -DFAST_FLOAT_CMP=$(FAST_FLOAT_CMP)

INCS = -I../ispu_utils -I../ispu_dsp/inc -I../inc

CFLAGS = -mcpu=stredlp -mfp32-format=alternative
CFLAGS += -O3 -flto -ffunction-sections -fdata-sections -fno-strict-aliasing -Wall -Wextra -Wdouble-promotion
CFLAGS += $(DEFINE) $(INCS)

LDFLAGS = -T ../ispu_utils/boot.ld -Wl,--gc-sections -Wl,--sort-section=alignment -specs=nano.specs
LDFLAGS += -L../ispu_dsp/lib

LDLIBS = -lispu_dsp

SRC = ../ispu_utils/crt0.S ../src/main.c ../src/dot.S
SRC += ../ispu_utils/ispu_float.c ../ispu_utils/ispu_float_conv.S

IGFLAGS = -d iis3dwb10is -s ../conf.txt -m ../meta.txt -n ispu_conf -nd

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

