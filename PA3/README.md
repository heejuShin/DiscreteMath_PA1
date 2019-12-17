# Discrete Mathematics
>* Q: How to build and run the programs?
>
> - A: Follow this step
> - Step 1. Go to the directory named "example"
> - Step 2. Type "make clean" and then type "make" to build a program.
> - Step 3. Then two program named "count" and "calculate" will be appeared. So type "./count" and then type "./calculate <filename>".
> - Then, the program will classify each of messages in the file whether it is negative message or not. Each of messages is distinguished by ENTER.

- Example: As an example, I want to get a prediction for file named "text".

input:    
    
    i love you

Step 1:

    s21600558@peace:/DiscreteMath/PA3$ ls
    data example include lib libstemmer_c 
    
    s21600558@peace:/DiscreteMath/PA3$ cd example

    s21600558@peace:~/DiscreteMath/PA3/example$
 
Step2 2:

    s21600558@peace:/DiscreteMath/PA3/example$ make clean
    rm count
    rm calculate

    s21600558@peace:/DiscreteMath/PA3/example$ make 
    gcc -o count `pkg-config --cflags --libs glib-2.0` count.c `pkg-config --libs glib-2.0` ../lib/libstemmer.o -lm
    gcc -o calculate `pkg-config --cflags --libs glib-2.0` calculate.c `pkg-config --libs glib-2.0` ../lib/libstemmer.o -lm

Step 3:

    s21600558@peace:/DiscreteMath/PA3/example$ ./calculate text

output: 

    NON-NEGATIVE_PREDICTED, 0.593056

