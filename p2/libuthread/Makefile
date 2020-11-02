# Target library
lib 	:= libuthread.a
objs 	:= queue.o

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

$(lib): $(objs)
	@echo "create library"
	$(Q)ar rcs $(lib) $(objs)

%.o: %.c 
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<	

clean: 
		@echo "CLEAN"
		$(Q)rm -f $(lib) $(objs) $(deps)