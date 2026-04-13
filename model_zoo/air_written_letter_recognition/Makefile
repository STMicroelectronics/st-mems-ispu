THIS_DIR = $(subst $() $(),\ ,$(subst /Makefile,,$(abspath $(lastword $(MAKEFILE_LIST)))))

USER_CONFIG = training/user_config.yaml
OUTPUT_DIR = output

MODEL_DIR = models/lsm6dso16is_ISPU_reference_dataset
MODEL_PATH = $(MODEL_DIR)/training_output/cnn_8x8x8.keras

DEPLOY_TEMPLATE = application
DEPLOY_DIR = $(MODEL_DIR)/application

RUN_FW = ../../host_firmware/nucleo_ispu_test_header/nucleo-f401re

CP = cp -r
RMDIR = rm -rf

ifeq ($(OS),Windows_NT)
    null =
    space = $(null) $(null)
    prep_path = $(subst $(1),$(space),$(subst $(space),?,$(subst \,/,$(PATH))))
    find_in_path = $(strip $(wildcard $(addsuffix /$(1),$(call prep_path,$(2)))))

    RM_LOC = $(call find_in_path,rm.exe,;)$(call find_in_path,rm.exe,:)
    ifeq ($(RM_LOC),)
        SHELL = cmd.exe
        RMDIR = powershell rm -r -fo
    endif
    CP_LOC = $(call find_in_path,cp.exe,;)$(call find_in_path,cp.exe,:)
    ifeq ($(CP_LOC),)
        SHELL = cmd.exe
        CP = powershell cp -r
    endif
endif

train:
	python training/train.py $(USER_CONFIG)

validate_host:
	stedgeai validate --target ispu --device imu_22 --mode host -m $(MODEL_PATH) --no-workspace \
		-o $(MODEL_DIR)/validation_output -vi $(MODEL_DIR)/training_output/validation_data.npz

validate_target:
	stedgeai validate --target ispu --device imu_22 --mode target -m $(MODEL_PATH) \
		--no-workspace -o $(MODEL_DIR)/validation_output -vi $(MODEL_DIR)/training_output/validation_data.npz

generate:
ifneq ("$(wildcard $(DEPLOY_DIR))","")
	$(RMDIR) $(DEPLOY_DIR)
endif
	$(CP) $(DEPLOY_TEMPLATE) $(DEPLOY_DIR)
	stedgeai generate --target ispu --device imu_22 -m $(MODEL_PATH) --no-workspace --no-report -o $(DEPLOY_DIR)/ispu
	$(CP) $(MODEL_DIR)/training_output/ispu_wand_params.h $(DEPLOY_DIR)/ispu/inc
	$(CP) $(MODEL_DIR)/training_output/conf.txt $(DEPLOY_DIR)/ispu
	$(CP) $(MODEL_DIR)/training_output/meta.txt $(DEPLOY_DIR)/ispu
	make -C $(DEPLOY_DIR)/ispu/make

run:
	make -C "$(RUN_FW)" ISPU_DIR="$(THIS_DIR)/$(DEPLOY_DIR)/ispu"
ifeq ($(OS),Windows_NT)
	$(RUN_FW)/flash.bat $(PROBE_INDEX)
else
	$(RUN_FW)/flash.sh $(PROBE_INDEX)
endif

.PHONY: training validate_host validate_target generate run

