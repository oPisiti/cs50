-- Returning the names of the stars
SELECT name 
FROM people 
WHERE id 
IN
    (
    -- Getting the id of the people who starred in the movie
    SELECT person_id 
    FROM stars 
    WHERE movie_id
    IN
        (
        -- Getting the movie's id
        SELECT id
        FROM movies
        WHERE title = "Toy Story"
        )
    )
;