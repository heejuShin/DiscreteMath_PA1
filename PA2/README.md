# 19_DiscreteMathmatics_PA2

## How to build and run our program
First, use make instruction to build the program
~~~
~@peace:~/19_DiscreteMathmatics$ make
gcc pa2_5.c -o converter
~~~
After that, Makefile builds the program

Run the program and enter the formula<br>
Then you can get a result
~~~
~@peace:~/19_DiscreteMathmatics_PA2$ ./converter
Enter a propositional formula in prefix notation: (or a1 (not (or (not (or a2 a3)) a4)))
1 2 3 
1 -4 
~~~

If you want to remove the execution file, use the instruction "make clean"
~~~
~@peace:~/19_DiscreteMathmatics$ make clean
rm converter
~~~

		
------------------------------------------------- 
**The input must fallow these rules.<br>**
- for n, which is natural number, an is a propositional formula.<br>
- for 𝜙𝑖, which is proposition formula, (and𝜙1𝜙2...𝜙𝑚) is a proposition formula.<br>
- for 𝜙𝑖, which is proposition formula, (or𝜙1𝜙2...𝜙𝑚) is a proposition formula.<br>
- for 𝜙𝑖, which is proposition formula, (not 𝜙) is a proposition formula.<br>
