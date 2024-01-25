from website import create_app

app = create_app()

if __name__ == "__main__": # only runs the server if this file is ran, not if imported
    app.run(debug = True)