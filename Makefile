# GNU Make solution makefile autogenerated by Premake
# Usage: make [ config=config_name ]
# Where {config_name} is one of: "debug", "release".

ifndef config
  config=debug
endif
export config

PROJECTS := arbarlith2

.PHONY: all clean $(PROJECTS)

all: $(PROJECTS)

ifeq ($(config),debug)
  DEPENDENCIES := 
endif
ifeq ($(config),release)
  DEPENDENCIES := 
endif

arbarlith2: ${DEPENDENCIES}
	@echo ==== Building arbarlith2 ====
	@${MAKE} --no-print-directory -C . -f arbarlith2.make

clean:
	@${MAKE} --no-print-directory -C . -f arbarlith2.make clean
