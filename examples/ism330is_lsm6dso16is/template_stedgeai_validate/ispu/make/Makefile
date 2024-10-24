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
        NULL := $(shell $(RMDIR) -p)
    endif
    NULL := $(shell $(RM) tmp)
endif

CFLAGS = -mcpu=reiscl -Os -specs=nano.specs -ffunction-sections -fdata-sections -Wl,--gc-sections -mfp32-format=ieee -Wall -Wextra -Wdouble-promotion -fno-strict-aliasing -DEXPORT_RUNTIME_LITE_APIS -DHAVE_NETWORK_INFO=1
INCS = -I../ispu_utils -I../inc -I../inc/ai
LDFLAGS = -L../lib/ai -lNetworkRuntime_ISPU_GCC

SRC = ../ispu_utils/crt0.S ../src/main.c ../src/network_data.c ../src/network.c

OUTDIR = bin
NAME = ispu

LAYER_START_DEFINE = _STAI_NETWORK_EVENT_NODE_START_CB(_node_id, _buffers_size, ...)
define LAYER_START
(*((volatile uint8_t *)(0x685C))) = 2u; \
if (net_ctx->_callback) { \
  const stai_event_node_start_stop _start_event = { \
    .node_id=(_node_id), \
    .buffers={ \
      .size=(_buffers_size), \
      .data=(stai_ptr const*)(const stai_ptr[_buffers_size])STAI_PACK(__VA_ARGS__) \
    } \
  }; \
  net_ctx->_callback(net_ctx->_callback_cookie, STAI_EVENT_NODE_START, (const void*)&_start_event); \
}
endef

LAYER_STOP_DEFINE = _STAI_NETWORK_EVENT_NODE_STOP_CB(_node_id, _buffers_size, ...)
define LAYER_STOP
(*((volatile uint8_t *)(0x685C))) = 0u; \
if (net_ctx->_callback) { \
  const stai_event_node_start_stop _stop_event = { \
    .node_id=(_node_id), \
    .buffers={ \
      .size=(_buffers_size), \
      .data=(stai_ptr const*)(stai_ptr[_buffers_size])STAI_PACK(__VA_ARGS__) \
    } \
  }; \
  net_ctx->_callback(net_ctx->_callback_cookie, STAI_EVENT_NODE_STOP, (const void*)&_stop_event); \
}
endef

build: $(SRC) $(OUTDIR) Makefile
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(NAME) $(SRC) $(LDFLAGS) -T ../ispu_utils/boot.ld $(INCS) -D'$(LAYER_START_DEFINE)=$(LAYER_START)' -D'$(LAYER_STOP_DEFINE)=$(LAYER_STOP)'
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

