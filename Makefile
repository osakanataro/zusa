SHELL	= /bin/sh
TARGET	= zusa
OBJS	= zusa.o
CC	= gcc
CCFLAGS	= 
LIB	= -lncurses
#LIB	= -lcurses
FILES	= $(TARGET).c Makefile
TMP	= /tmp
TMPZUSA	= $(TMP)/$(TARGET)


$(TARGET) : $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $(OBJS) $(LIB)

tarball:
	@mkdir $(TMPZUSA)
	@chown nobody.nobody $(TMPZUSA)
	@for f in $(FILES) ; \
		do cp $$f $(TMPZUSA)/ ; \
		chown nobody.nobody $(TMPZUSA)/$$f ; \
	done
	@(cd $(TMP); tar zcf $(TMP)/$(TARGET).tgz ./$(TARGET))
	@rm -rf $(TMPZUSA)

clean :
	rm -f *core *.o $(TARGET) *~
