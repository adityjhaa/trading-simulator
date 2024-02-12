.DEFAULT_GOAL := run

strategy := def
symbol := def
n := 0
x := 0
start_date := def
end_date := def
p:= 0
max_hold_days := 0
c1 := 0
c2 := 0
oversold_threshold := 0
overbought_threshold := 0
adx_threshold := 0
train_start_date := def
train_end_date := def
symbol1 := def
symbol2 := def
threshold := 0
stop_loss_threshold := 0


run: main.out
	@./main.out $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date) $(p) $(max_hold_days) $(c1) $(c2) $(oversold_threshold) $(overbought_threshold) $(adx_threshold) $(train_start_date) $(train_end_date) $(symbol1) $(symbol2) $(threshold) $(stop_loss_threshold)
	@rm -f *.out

main.out: main.cpp
	@g++ main.cpp -o main.out

clean:
	@rm -f *.csv *.txt
