# CHECK SET ENV
ifndef ENV_OK
$(error "Please enter '. setenv.sh' first")
endif

all: mking_dir src_default
	@echo -e "\033[31;1m"
	@echo
	@echo "               * * * * * * * * * * * * * * * * * * * *"
	@echo "               *              EBI DK                 *"
	@echo "               *           Build Complete            *"
	@echo "               * * * * * * * * * * * * * * * * * * * *"
	@echo
	@echo -e "\033[0m"

mking_dir:
	@echo "$(EBI_TOP_DIR)"
	@echo "mkdir -p $(EBI_IMAGE_DIR)"
	@mkdir -p $(EBI_IMAGE_DIR)

src_default:
	@for i in $(APPS_MAKE)    ; do make -C $$i || exit $? ; done
	@echo
	@echo "SRC Build Complete"
	@echo
