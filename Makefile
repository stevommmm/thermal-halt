CC = gcc
CFLAGS = -g -Wall -Wuninitialized -O1 -std=c11
ifdef SANATIZE_ADDRESS
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif
ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif
BINARY = thermal-halt

all: $(BINARY)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINARY): $(wildcard src/*.c)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean uninstall

install: $(BINARY)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 555 $(BINARY) $(DESTDIR)$(PREFIX)/bin/
	install -m 444 thermal-halt.service /etc/systemd/system/
	@systemctl daemon-reload
	@echo "Run systemctl {enable,start} thermal-halt.service"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BINARY) \
	  /etc/systemd/system/thermal-halt.service
	@systemctl daemon-reload

clean:
	$(RM) */*.o *.o $(BINARY)
