CFLAGS += -W -Wall -Wextra -Werror 
DEBUG   += -ggdb3 -fsanitize=address -DDEBUG
RELEASE += -O3 -fstack-protector-all -fPIE 


all: getwebserver getip

release: CFLAGS += $(RELEASE)
release: all

debug: CFLAGS +=  $(DEBUG)
debug: all

clean:
	$(RM) *~ getwebserver getip
