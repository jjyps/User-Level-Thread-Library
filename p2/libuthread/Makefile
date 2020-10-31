# Target library
lib 	:= libuthread.a
objs 	:= 

CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror -MMD
CFLAGS	+= -g

ifneq ($(V),1)
Q = @
endif

all: $(lib)
## TODO: Phase 1
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)


clean: 
		@echo "CLEAN"