USER_CONFIG = training/user_config.yaml
OUTPUT_DIR = output

MODEL_DIR = models/lsm6dso16is_ISPU_reference_dataset
MODEL_PATH = $(MODEL_DIR)/training_output/cnn_8x8x8.h5

VALIDATE_TEMPLATE = ../../examples/ism330is_lsm6dso16is/template_stedgeai_validate/ispu
VALIDATE_DIR = $(MODEL_DIR)/validation_application

DEPLOY_TEMPLATE = application
DEPLOY_DIR = $(MODEL_DIR)/application

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
	stedgeai validate --target ispu --mode host -m $(MODEL_PATH) --no-workspace \
		-o $(MODEL_DIR)/validation_output -vi $(MODEL_DIR)/training_output/validation_data.npz

validate_target:
ifneq ("$(wildcard $(VALIDATE_DIR))","")
	$(RM) $(VALIDATE_DIR)
endif
	$(CP) $(VALIDATE_TEMPLATE) $(VALIDATE_DIR)
	stedgeai generate --target ispu -m $(MODEL_PATH) --no-workspace -o $(VALIDATE_DIR)
	make -C $(VALIDATE_DIR)/make
	mkdir -p $(MODEL_DIR)/validation_output
	stedgeai validate --target ispu --mode target -m $(MODEL_PATH) --ucf $(VALIDATE_DIR)/make/bin/ispu.ucf \
		--no-workspace -o $(MODEL_DIR)/validation_output -vi $(MODEL_DIR)/training_output/validation_data.npz

generate:
ifneq ("$(wildcard $(DEPLOY_DIR))","")
	$(RM) $(DEPLOY_DIR)
endif
	$(CP) $(DEPLOY_TEMPLATE) $(DEPLOY_DIR)
	stedgeai generate --target ispu -m $(MODEL_PATH) --no-workspace --no-report -o $(DEPLOY_DIR)/ispu
	$(CP) $(MODEL_DIR)/training_output/ispu_wand_params.h $(DEPLOY_DIR)/ispu/inc
	$(CP) $(MODEL_DIR)/training_output/conf.txt $(DEPLOY_DIR)/ispu
	$(CP) $(MODEL_DIR)/training_output/meta.txt $(DEPLOY_DIR)/ispu
	make -C $(DEPLOY_DIR)/ispu/make

.PHONY: training validate_host validate_target generate

