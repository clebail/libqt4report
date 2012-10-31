CC = g++
CFLAGS = -g -Wall -I. `pkg-config --cflags QtCore QtGui libqt4report`
LDFLAGS = `pkg-config --libs QtCore QtGui libqt4report`
ALL = test

all: $(ALL)

$(ALL): test.o
	$(CC) -o $@ $< $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(ALL)
	rm -f *.o