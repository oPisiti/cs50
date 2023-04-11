-- Names of the directors
SELECT name
FROM people 
WHERE id 
IN
    (
    -- ids of the directors
    SELECT DISTINCT person_id 
    FROM directors
    WHERE movie_id 
    IN
        (
        -- id of movies rated 9 or more
        SELECT id
        FROM movies 
            JOIN ratings
                ON movies.id = ratings.movie_id 
                AND ratings.rating >= 9
        )
    )
;