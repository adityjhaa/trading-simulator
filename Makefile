all:
	@pip install -r requirements.txt
	@python3 app.py


clean:
	@rm -rf instance
	@rm -rf website/__pycache__

