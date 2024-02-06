.DEFAULT_GOAL := run

strategy := first
symbol := second
n := third


run: main.out
	@./main.out $(strategy) $(symbol) $(n)

main.out: main.cpp
	@g++ main.cpp -o main.out

clean:
	@rm -f *.out