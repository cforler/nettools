CFLAGS += -W -Wall -Wextra -Werror 
DEBUG   += -ggdb3 -fsanitize=address -DDEBUG
RELEASE += -O3 -fstack-protector-all -fPIE 


all: getwebserver getip getifaddr

release: CFLAGS += $(RELEASE)
release: all

debug: CFLAGS +=  $(DEBUG)
debug: all

getip: netaux.c

getifaddr: netaux.c queue.c


clean:
	$(RM) *~ getwebserver getip getiffadr
