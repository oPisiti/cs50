import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    owned_stocks = get_owned_stocks()
    current_prices = {stock: lookup(stock)["price"] for stock in owned_stocks.keys()}
    cash = get_cash()
    owned_stocks_value = sum([owned_stocks[stock] * current_prices[stock] for stock in owned_stocks.keys()])

    return render_template("index.html", owned_stocks = owned_stocks, prices = current_prices, cash = get_cash(), stocks_value = owned_stocks_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        # Getting the stock's info
        stock_symbol = request.form.get("symbol")
        if stock_symbol == "": return apology("Stock's symbol required")

        stock_info = lookup(stock_symbol)
        if stock_info is None: return apology("Stock does not exist")

        # Checking amount to buy
        try:
            amount_to_buy = int(request.form.get("shares"))
        except ValueError as e:
            return apology("Invalid amount")

        if amount_to_buy <= 0: return apology("Amount must be bigger than 0")

        # Getting the stock's current price
        stock_price = stock_info["price"]
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session.get("user_id"))[0]["cash"]

        # Checking if buy is possible
        purchase_price = stock_price * amount_to_buy
        if purchase_price > user_cash: return apology("Not enough cash in account")

        # Making the transaction
        update_cash(purchase_price, "buy")

        # Storing the transaction's data in the database
        make_transaction(stock_symbol, amount_to_buy, purchase_price)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("""
    SELECT TYPE,
       stock_symbol,
       total_cost / amount price,
       amount,
       time_stamp
    FROM transactions t
    WHERE user_id = ?;
    """,
    session.get("user_id"))

    return render_template("history.html", history=history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods = ["GET", "POST"])
@login_required
def quote():
    """ Get stock quote """
    if request.method == "POST":
        stock_info = lookup(request.form.get("symbol"))
        if stock_info is None: return apology("Invalid stock symbol")
        return render_template("quoted.html", stock_info = stock_info)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """ Register user """
    if request.method == "POST":
        username     = request.form.get("username")
        password     = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Checking if input is valid
        if username == "" or password == "" or confirmation == "":
            return apology("Blank field")
        if password != confirmation:
            return apology("Passwords do not match")

        # Checking if username is already taken
        if db.execute("SELECT COUNT(*) username_count FROM users WHERE username = ?;", username)[0]["username_count"] != 0:
            return apology("Username already taken")

        # Storing the user's information
        hashed_password = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", username, hashed_password)

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Getting owned stocks and amount owned
    session["owned_stocks_amount"] = get_owned_stocks()

    if request.method == "POST":
        # Checking the stock to be sold
        chosen_stock_symbol = request.form.get("symbol")
        if chosen_stock_symbol == "": return apology("Invalid Stock")
        if chosen_stock_symbol not in session["owned_stocks_amount"]: return apology("Unowned stock")

        # Checking the amount of shares to be sold
        try:
            amount_to_sell = int(request.form.get("shares"))
        except ValueError as e:
            return apology("Please input the amount of stocks to be sold")

        if amount_to_sell <= 0:    return apology("Invalid Amount")

        # Amount of owned stocks of said company
        if amount_to_sell > session["owned_stocks_amount"][chosen_stock_symbol]:
            return apology(f"Insufficient amount of owned '{chosen_stock_symbol}' stocks")

        # Transaction's price
        sell_price = amount_to_sell * lookup(chosen_stock_symbol)["price"]

        # Completing the transaction
        make_transaction(chosen_stock_symbol, amount_to_sell, -sell_price, transaction_type = "sell")

        return redirect("/")

    else:
        return render_template("sell.html", owned_stocks=session["owned_stocks_amount"])


def get_owned_stocks() -> dict:
    """ Gets the symbols and amount owned of every stock of current user """

    owned = db.execute(
        """
        SELECT DISTINCT (stock_symbol) sym, TOTAL(amount) amount_owned
        FROM transactions t
        WHERE user_id = ?
        GROUP BY stock_symbol;
        """,
        session.get("user_id")
    )

    # Cleaning the query's result
    res = dict()
    for item in owned: res[item["sym"]] = item["amount_owned"]

    return res


def update_cash(price, transaction_type) -> None:
    """ Updates cash """

    if transaction_type not in ("buy", "sell"):
        raise ValueError(f"Transaction type '{transaction_type}' not supported")

    db.execute(
        """
        UPDATE users
        SET cash = (
            SELECT cash - ?
            FROM users
            WHERE id = ?
        )
        WHERE id = ?;
        """,
        abs(price) * (-1 if transaction_type == "sell" else 1),
        session.get("user_id"),
        session.get("user_id"))


def make_transaction(sym: str, amount: int, price: float, transaction_type = "buy") -> None:
    """ Updates database following a transaction """

    db.execute(
        """
        INSERT INTO transactions (user_id, 'type', stock_symbol, amount, total_cost) 
        VALUES (?, ?, ?, ?, ?);
        """,
        session.get("user_id"),
        transaction_type,
        sym.lower(),
        amount,
        price)


def get_cash() -> float:
    return db.execute(
    """
    SELECT cash
    FROM users
    WHERE id = ?; 
    """,
    session.get("user_id"))[0]["cash"]