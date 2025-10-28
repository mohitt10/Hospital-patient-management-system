CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = hospital

OBJS = main.o patient.o doctor.o fileio.o heap.o 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

rmexe:
	rm -f $(TARGET)

run: 
	./$(TARGET)