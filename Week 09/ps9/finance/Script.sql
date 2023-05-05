CREATE TABLE IF NOT EXISTS transactions(
    id INTEGER UNIQUE PRIMARY KEY autoincrement NOT NULL , 
    user_id INTEGER NOT NULL, 
    'type' TEXT NOT NULL DEFAULT "buy",
    stock_symbol TEXT NOT NULL,
    amount INTEGER NOT NULL,
    total_cost REAL NOT NULL,
    time_stamp TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES users(id))
;

CREATE UNIQUE INDEX transaction_id ON transactions (id);
CREATE INDEX user_id ON transactions(id);
CREATE INDEX transaction_type ON transactions('type');

INSERT INTO transactions (user_id, stock_symbol, amount, total_cost) 
VALUES (5, "ddd", 7, 50.14);

SELECT TOTAL(amount)
  FROM transactions t2
 WHERE stock_symbol IN (
        SELECT DISTINCT (stock_symbol)
          FROM transactions t
       );

SELECT DISTINCT (stock_symbol) sym, TOTAL(amount) amount_owned
  FROM transactions t
  GROUP BY stock_symbol;
          

INSERT INTO transactions (user_id, 'type', stock_symbol, amount, total_cost) 
VALUES (5,"sell", "ddd", 7, -50.14);


SELECT TYPE,
       stock_symbol,
       total_cost / amount price,
       amount,
       time_stamp
  FROM transactions t
 WHERE user_id = 6;



SELECT DISTINCT (stock_symbol) sym,
       total(amount) amount_owned
  FROM transactions t
 WHERE user_id = 6
 GROUP BY stock_symbol;


SELECT COUNT(*) username_count
  FROM users
 WHERE username = "j";
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 





















          
          
          
          
          
          