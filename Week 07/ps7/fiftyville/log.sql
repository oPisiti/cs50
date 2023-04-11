-- Only one of the reports mentions a theft of the cs50 duck.
-- 1. "Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery."
-- Three witnesses were interviewed in the same day. All mentioned a bakery.
SELECT *
  FROM crime_scene_reports csr
 WHERE "year" = 2021
   AND "month" = 7
   AND "day" = 28
   AND street = 'Humphrey Street' ;

-- The three interviews' reports:
-- 2. "Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame."
-- 3. "I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money."
-- 4. "As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket."
SELECT *
  FROM interviews i
 WHERE "year" = 2021
   AND "month" = 7
   AND "day" = 28
   AND transcript LIKE "%bakery%" ;

-- Investigating 2.
-- People who got out of the parking lot in that morning:
SELECT id
  FROM people p
 WHERE license_plate in (
        SELECT license_plate
          FROM bakery_security_logs bsl
         WHERE "year" = 2021
           AND "month" = 7
           AND "day" = 28
           AND "hour" < 12
           AND activity = "exit"
       );   
   
-- Investigating 3.
-- Ids of people who withdrew money on Leggett Street's ATM on that morning
SELECT id
  FROM people p
 WHERE id in (
        SELECT person_id
          FROM (
                SELECT *
                  FROM atm_transactions
                 WHERE "year" = 2021
                   AND "month" = 7
                   AND "day" = 28
                   AND atm_location = "Leggett Street"
                   AND transaction_type = "withdraw"
               ) atm_t
         INNER JOIN bank_accounts ba
            ON atm_t.account_number = ba.account_number
       ) ;

-- Investigating 4.
-- Ids of the people who took the first flight out of Fiftyville the next morning and called someone during less than 60 seconds
SELECT fl_people_id.id
  FROM (
        SELECT id
          FROM people p2
         WHERE passport_number in (
                SELECT passport_number
                  FROM passengers p
                 INNER JOIN (
                        SELECT *
                          FROM flights f
                         WHERE "year" = 2021
                           AND "month" = 7
                           AND "day" = 29
                           AND origin_airport_id = (
                                SELECT id
                                  FROM airports a
                                 WHERE city = "Fiftyville"
                               )
                         ORDER BY "hour",
                                  "minute" ASC
                         LIMIT 1
                       ) fl
                    ON p.flight_id = fl.id
               )
       ) fl_people_id
 INNER JOIN (
        SELECT id
          FROM people p3
         WHERE phone_number IN (
                SELECT caller
                  FROM phone_calls pc
                 WHERE "year" = 2021
                   AND "month" = 7
                   AND "day" = 28
                   AND duration < 60
               )
       ) call_people_id
    ON fl_people_id.id = call_people_id.id;
   
-- Taking into account 2., 3. and 4.
CREATE TABLE on_parking_lot(
    id  INT
);

CREATE TABLE withdrew_money(
    id  INT
);

CREATE TABLE flew_away(
    id  INT
);

INSERT INTO on_parking_lot VALUES (210245);
INSERT INTO on_parking_lot VALUES (221103);
INSERT INTO on_parking_lot VALUES (243696);
INSERT INTO on_parking_lot VALUES (282425);
INSERT INTO on_parking_lot VALUES (360948);
INSERT INTO on_parking_lot VALUES (379932);
INSERT INTO on_parking_lot VALUES (384013);
INSERT INTO on_parking_lot VALUES (396669);
INSERT INTO on_parking_lot VALUES (398010);
INSERT INTO on_parking_lot VALUES (449774);
INSERT INTO on_parking_lot VALUES (467400);
INSERT INTO on_parking_lot VALUES (486361);
INSERT INTO on_parking_lot VALUES (514354);
INSERT INTO on_parking_lot VALUES (542503);
INSERT INTO on_parking_lot VALUES (560886);
INSERT INTO on_parking_lot VALUES (604497);
INSERT INTO on_parking_lot VALUES (620297);
INSERT INTO on_parking_lot VALUES (686048);
INSERT INTO on_parking_lot VALUES (704850);
INSERT INTO on_parking_lot VALUES (713341);
INSERT INTO on_parking_lot VALUES (768248);
INSERT INTO on_parking_lot VALUES (809194);
INSERT INTO on_parking_lot VALUES (837455);
INSERT INTO on_parking_lot VALUES (864400);


INSERT INTO withdrew_money VALUES (395717);
INSERT INTO withdrew_money VALUES (396669);
INSERT INTO withdrew_money VALUES (438727);
INSERT INTO withdrew_money VALUES (449774);
INSERT INTO withdrew_money VALUES (458378);
INSERT INTO withdrew_money VALUES (467400);
INSERT INTO withdrew_money VALUES (514354);
INSERT INTO withdrew_money VALUES (686048);


INSERT INTO flew_away VALUES (395717);
INSERT INTO flew_away VALUES (398010);
INSERT INTO flew_away VALUES (449774);
INSERT INTO flew_away VALUES (560886);
INSERT INTO flew_away VALUES (686048);


-- Thief's name
SELECT *
  FROM people p
 WHERE id in (
        SELECT p_wn.id
          FROM (
                SELECT wm.id
                  FROM (
                        SELECT *
                          FROM on_parking_lot
                       ) p
                 INNER JOIN (
                        SELECT *
                          FROM withdrew_money
                       ) wm
                    ON p.id = wm.id
               ) p_wn
         INNER JOIN (
                SELECT *
                  FROM flew_away
               ) f
            ON p_wn.id = f.id
       );
       
-- Thief: Bruce (id: 686048)
-- The above query points to two possible people: Taylor (id: 449774) and the aforementioned Bruce
-- Bruce was seen leaving the bakery just 2 minutes after the theft.
-- Taylor, however, left at 10:35, 20 minutes after the fact. 
-- A thief would not have stayed there for 20 minutes.
   
-- The city he escaped TO
SELECT city
  FROM airports a
 WHERE id in (
        SELECT destination_airport_id
          FROM flights f
         WHERE id in (
                SELECT flight_id
                  FROM passengers p
                 WHERE passport_number = 5773159633
               )
           AND "year" = 2021
           AND "month" = 7
           AND "day" = 29
       );
   

-- The accomplice
SELECT name
  FROM people p3
 WHERE phone_number IN (
        SELECT receiver
          FROM phone_calls pc
         WHERE "year" = 2021
           AND "month" = 7
           AND "day" = 28
           AND duration < 60
           AND caller = "(367) 555-5533"
       );
   
   
   
   