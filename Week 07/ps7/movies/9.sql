-- Names of the stars
SELECT name
FROM people 
WHERE id
IN
    (
    -- ids of people who starred in such movies
    SELECT person_id
    FROM stars 
    WHERE movie_id 
    IN
        (
        -- ids of movies released in 2004
        SELECT id
        FROM movies 
        WHERE YEAR = 2004
        )
    )
ORDER BY birth
;