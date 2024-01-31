from flask import Blueprint, render_template, request, flash, redirect, url_for
from .models import User
from werkzeug.security import generate_password_hash, check_password_hash
from . import db
# this is why we needed UserMixin
from flask_login import login_user, login_required, logout_user, current_user
# Used for UI, and have all the URLs

auth = Blueprint('auth', __name__)

@auth.route('/register', methods = ['GET', 'POST'])
def register():
    if request.method == 'POST':
        # getting in the details
        username = request.form.get('username')
        password1 = request.form.get('password1')
        password2 = request.form.get('password2')
        hashed_password = generate_password_hash(password1, method='pbkdf2:sha256')

        # now we want to check if the info is valid
        user = User.query.filter_by(username = username).first()
        if user:
            flash('Username already exists.', category='error')
            return redirect(url_for('auth.login'))
        elif len(username) < 4:
            # message flashing
            flash('Username must be greater than 3 characters.', category='error')
        elif password1 != password2:
            flash('Passwords must match.', category='error')
        elif len(password1) < 7:
            flash('Password must be greater than 6 characters.', category='error')
        else:
            # add user to database
            new_user = User(username=username, password=hashed_password)          
            db.session.add(new_user)
            db.session.commit()
            login_user(new_user, remember=True)

            flash('Registration successful!', category='success')
            return redirect(url_for('views.home'))

    return render_template('register.html', user = current_user)

@auth.route('/logout')
@login_required
def logout():
    # using flask_login, to logout the user
    logout_user()
    flash('Logged out successfully!', category='success')
    return redirect(url_for('auth.login'))

# by default method accepted are only GET, but now, we can accept both GET and POST requests
@auth.route('/login', methods=['GET', 'POST'])
def login():
    # we only want to store info in a POST request
    if request.method == 'POST':
        username = request.form.get('username') # names
        password = request.form.get('password')
        
        user = User.query.filter_by(username = username).first()
        if user:
            if check_password_hash(user.password, password):
                flash('Logged in successfully!', category='success')
                # this remembers the fact that the user is logged in
                login_user(user, remember=True)
                return redirect(url_for('views.home'))    
            else:
                flash('Incorrect password, try again.', category='error')
        else:
            flash('Username does not exist.', category='error')
        
    return render_template("login.html", user = current_user)