import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        name = request.form.get("name", None)
        bday = request.form.get("date", None)
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?);", name, int(bday[5:7]), int(bday[8:10]))      

        return redirect("/")

    else:

        birthday_data = db.execute('SELECT * FROM birthdays;')
        return render_template("index.html", birthday_data=birthday_data)



