USER_CONFIG = training/user_config.yaml
OUTPUT_DIR = output

MODEL_DIR = models/lsm6dso16is_ISPU_reference_dataset
MODEL_PATH = $(MODEL_DIR)/training_output/cnn_8x8x8.h5
MODEL_DEPLOY_DIR = $(MODEL_DIR)/application

VALIDATION_DIR = ../../host_firmware/nucleo_ispu_stedgeai_validate
VALIDATION_FW = $(VALIDATION_DIR)/binary/nucleo_f401re_ispu_stedgeai_validate.bin
VALIDATION_TEMPLATE = ../../examples/ism330is_lsm6dso16is/template_stedgeai_validate/ispu

DEPLOY_DIR = application
CP = cp -R
RM = rm -rf

ifeq ($(OS),Windows_NT)
    ifeq (,$(shell where rm 2> tmp))
		RM := powershell rm -rf
	endif
    ifeq (,$(shell where cp 2> tmp))
		CP := powershell cp -R
	endif
endif

train:
	python training/train.py $(USER_CONFIG)

validate_host:
	stedgeai validate --target ispu --mode host --model $(MODEL_PATH) --allocate-inputs --allocate-outputs --no-workspace \
		--output $(MODEL_DIR)/validation_output --valinput $(MODEL_DIR)/training_output/validation_data.npz

validate_target:
	stedgeai generate --target ispu --model $(MODEL_PATH) --allocate-inputs --allocate-outputs --no-workspace --output $(VALIDATION_TEMPLATE)
	make -C $(VALIDATION_TEMPLATE)/make
	mkdir -p $(MODEL_DIR)/validation_output
	stedgeai validate --target ispu --mode target --model $(MODEL_PATH) --ucf $(VALIDATION_TEMPLATE)/make/bin/ispu.ucf \
		--allocate-inputs --allocate-outputs --no-workspace --output $(MODEL_DIR)/validation_output

generate:
ifneq ("$(wildcard $(MODEL_DEPLOY_DIR))","")
	$(RM) $(MODEL_DEPLOY_DIR)
endif
	$(CP) $(DEPLOY_DIR) $(MODEL_DEPLOY_DIR)
	make -C $(MODEL_DEPLOY_DIR)/ispu/make clean
	stedgeai generate --target ispu --model $(MODEL_PATH) --allocate-inputs --allocate-outputs --no-workspace --no-report --output $(MODEL_DEPLOY_DIR)/ispu
	$(CP) $(MODEL_DIR)/training_output/ispu_wand_params.h $(MODEL_DEPLOY_DIR)/ispu/inc
	$(CP) $(MODEL_DIR)/training_output/conf.txt $(MODEL_DEPLOY_DIR)/ispu
	make -C $(MODEL_DEPLOY_DIR)/ispu/make
	$(CP) $(MODEL_DIR)/training_output/ispu_wand.json $(MODEL_DEPLOY_DIR)/output


.PHONY: training validate_host validate_target generate
