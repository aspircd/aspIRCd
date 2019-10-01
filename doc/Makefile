# $Id: Makefile.in 3376 2007-04-03 11:37:39Z nenolod $
CC		= gcc
INSTALL		= /usr/bin/install -c
INSTALL_BIN	= ${INSTALL}
INSTALL_DATA	= ${INSTALL} -m 644
INSTALL_SUID	= ${INSTALL} -o root -m 4755
RM		= /bin/rm
LEX		= flex
LEXLIB		= 
CFLAGS		=  -O0 -Wall -std=gnu99 -g -g -O2  -DIRCD_PREFIX=\"/home/matty/ircd\"
LDFLAGS		=  -Wl,-export-dynamic -Wl,-rpath=${libdir}
MKDEP		= ${CC} -MM
MV		= /bin/mv
RM		= /bin/rm
CP		= /bin/cp
TOUCH		= /usr/bin/touch

prefix		= /home/matty/ircd
exec_prefix	= ${prefix}
exec_suffix	= @exec_suffix@
bindir		= ${exec_prefix}/bin
libexecdir	= ${bindir}
confdir		= /home/matty/ircd/etc
localstatedir	= ${prefix}
# Change this later! -- adrian
moduledir	= /home/matty/ircd/modules
automoduledir	= /home/matty/ircd/modules/autoload

# Local to the etc Makefile
mandir          = ${prefix}/share/man/man8
MANPAGES        = ircd.8

CONFS		= example.conf opers.conf.example

SSL_LIBS	=  -lcrypto
SSL_INCLUDES	= 

IRCDLIBS	= -ldl -lcrypt  $(SSL_LIBS)

INCLUDES	= -I../include $(SSL_INCLUDES)
CPPFLAGS	= ${INCLUDES} 

all: build
 
install-mkdirs:
	-@if test ! -d $(DESTDIR)$(confdir); then \
		echo "mkdir -p $(confdir)"; \
		mkdir -p $(DESTDIR)$(confdir); \
	fi

	-@if test ! -d $(DESTDIR)$(mandir); then \
		echo "mkdir -p $(mandir)"; \
		mkdir -p $(DESTDIR)$(mandir); \
	fi

install: install-mkdirs build
	@echo "ircd: installing example config files ($(CONFS))"
	@for i in $(CONFS); do \
		if test -f $(DESTDIR)$(confdir)/$$i; then \
			$(MV) $(DESTDIR)$(confdir)/$$i $(DESTDIR)$(confdir)/$$i.old; \
		fi; \
		$(INSTALL_DATA) $$i $(DESTDIR)$(confdir); \
	done

	-@if test ! -f $(DESTDIR)$(confdir)/ircd.motd; then \
		echo "ircd: installing motd file (ircd.motd)"; \
		$(INSTALL_DATA) ircd.motd $(DESTDIR)$(confdir); \
	fi

	-@if test -f $(DESTDIR)$(confdir)/links.txt; then \
		$(RM) $(DESTDIR)$(confdir)/links.txt; \
	fi

	@echo "ircd: installing manpage"
	@for i in $(MANPAGES); do \
		if test ! -f $(DESTDIR)$(mandir)/$$i; then \
			$(INSTALL_DATA) $$i $(DESTDIR)$(mandir); \
		fi; \
	done

build:

clean:

depend:

lint:

distclean:
	${RM} -f Makefile
