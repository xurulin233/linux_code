# CHECK SET ENV
ifndef ENV_OK
$(error "Please enter '. setenv.sh' first")
endif

LIB_MAKE = 
APPS_MAKE = 
MODULES_MAKE =

include Makefile.inc

ALL_MAKE = $(LIB_MAKE) $(APPS_MAKE) $(MODULES_MAKE)

src_default:
	@for i in $(LIB_MAKE)     ; do make -C $$i || exit $? ; done
	@for i in $(APPS_MAKE)    ; do make -C $$i || exit $? ; done
	@for i in $(MODULES_MAKE) ; do make -C $$i || exit $? ; done
	@echo
	@echo "SRC Build Complete"
	@echo

clean:
	@for i in $(ALL_MAKE) ; do make -C $$i clean || exit $? ; done
