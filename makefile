CC = gcc
TARGET1 = Sudoku
TARGET2 = FillaPix
TARGET3 = Numbrix

puzzles : $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1) :
	$(CC) Sudoku.c -o $(TARGET1)
	./$(TARGET1)

$(TARGET2) : 
	$(CC) FillaPix.c -o $(TARGET2)
	./$(TARGET2)

$(TARGET3) :
	$(CC) Numbrix.c -o $(TARGET3)
	./$(TARGET3)

puzzle :
		$(CC) Sudoku.c -o $(TARGET1) 
		$(CC) FillaPix.c -o $(TARGET2)
		$(CC) Numbrix.c -o $(TARGET3)
erase:
	rm $(TARGET1) $(TARGET2) $(TARGET3)