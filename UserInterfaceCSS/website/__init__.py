from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager

db = SQLAlchemy()
DB_NAME = "database.db"

def create_app():
    app = Flask(__name__)
    app.config['SECRET_KEY'] = "Starlord_Love_hurts_but_42_doesn't"
    app.config['SQLALCHEMY_DATABASE_URI'] = f'sqlite:///{DB_NAME}'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    db.init_app(app)
    
    from .views import views
    from .auth import auth
    
    app.register_blueprint(views)
    app.register_blueprint(auth)
    
    # to check if database is created
    from .models import User
    
    with app.app_context():
        db.create_all()
        
    login_manager = LoginManager()
    login_manager.login_view = 'auth.register' # where do we go if not logged in
    login_manager.init_app(app) # telling the login manager, which app we are using
    
    # this is telling flask how we load a user
    # and by default, it looks for the primary key, which is the int version of the id
    @login_manager.user_loader
    def load_user(id):
        return User.query.get(int(id))
    
    return app