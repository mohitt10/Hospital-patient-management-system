CC = gcc
CFLAGS = -Wall -Wextra -g
OBJ = user.o patient.o priority_queue.o menus.o main.o
TARGET = hospital

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)    
