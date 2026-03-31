CC = stred-gcc
AR = stred-gcc-ar
SIZE = stred-size

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

INCS = -Iinc -I../ispu_utils

CFLAGS = -mcpu=stredlp -mfp32-format=alternative
CFLAGS += -O3 -flto -ffunction-sections -fdata-sections -fno-strict-aliasing -Wall -Wextra -Wdouble-promotion
CFLAGS += $(DEFINE) $(INCS)

BUILD_DIR = build
OUTDIR = lib
NAME = ispu_dsp

SRC = $(sort $(wildcard src/*/*.c) $(wildcard src/*/*.S))
OBJ = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(SRC)))
DEP = $(addprefix $(BUILD_DIR)/, $(addsuffix .d, $(SRC)))

$(OUTDIR)/lib$(NAME).a: $(OBJ)
	$(MKDIR) $(OUTDIR)
	$(AR) rsD $@ $(OBJ)
	$(SIZE) -t $(OUTDIR)/lib$(NAME).a

$(BUILD_DIR)/%.c.o: %.c Makefile
	$(MKDIR) $(dir $@)
	$(CC) -frandom-seed=$< $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.S.o: %.S Makefile
	$(MKDIR) $(dir $@)
	$(CC) -frandom-seed=$< $(CFLAGS) -MMD -MP -c $< -o $@

clean:
ifneq (,$(wildcard $(BUILD_DIR)))
	$(RMDIR) $(BUILD_DIR)
endif

DOC_DIR = doc

doc:
	doxygen $(DOC_DIR)/doxygen_conf

doc_clean:
ifneq (,$(wildcard $(DOC_DIR)/html))
	$(RMDIR) $(DOC_DIR)/html
endif

.PHONY: clean doc doc_clean

-include $(DEP)

