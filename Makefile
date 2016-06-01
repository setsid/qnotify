TARGET := qnotify

CFLAGS := -O2 -Wall -Wextra -Werror -fPIC
CFLAGS += $(shell pkg-config --cflags Qt5Widgets)

LDFLAGS := $(shell pkg-config --libs Qt5Widgets)

all:
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	rm $(TARGET)

install:
	install -D --mode=0755 $(TARGET) $(DESTDIR)/usr/bin/$(TARGET)
