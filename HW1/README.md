# Discrete Mathematics HW1

## ITP 20002-01

> 21800412 Shin hee ju 

<hr>

### 1. HOW to EXECUTE

 - Enter ```$ make``` or ```$ make run``` to execute code.

 - You don't have to erase executable file. It will be done automatically.

- If you only want to create an executable file and don't want to run it, enter ```$ make exe```

-  ```$make clean``` command deletes executable file.

- If there is executable file created by the  ```$make exe``` command, enter  ```$make clean```before entering the  ```$make``` command


<hr>


### 2. What to Enter

- After the file is run, enter the file name to be used as input.

- Example

    ```
        make

        gcc NumberCross.c -o NumberCross

        ./NumberCross

        input          <-Enter here (filename)

    ```


<hr>


### 3. What to watch out for when creating input files

- Let the end of the line ends in numbers. Sometimes it doesn't work properly when it ends with blank.

    ```3 5 6 ```  (x)

    ```3 5 6```    (o)
