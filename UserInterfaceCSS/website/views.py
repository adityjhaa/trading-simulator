# this file handels the frontend that is the home page
from flask import Blueprint, render_template
from flask_login import login_required, current_user

views = Blueprint('views', __name__)

@views.route('/', methods = ['GET', 'POST'])
@login_required
def home():
    return render_template("home.html", user = current_user)