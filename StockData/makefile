.DEFAULT_GOAL := run

SYMBOL := first
num_years := second

requirements: requirements.txt
	@pip install -r requirements.txt
	@echo "All requirements downloaded!"

run: main.py requirements
	@python3 main.py $(SYMBOL) $(num_years)

clean:
	@rm SBIN.*
