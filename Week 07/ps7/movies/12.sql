-- Names of common movies
SELECT title
     FROM movies
    WHERE id IN (
    		-- Ids of common movies
             SELECT movie_id
               FROM stars
              WHERE person_id IN (
                    -- Ids of Johnny Depp and Helena Bonham Carter
                       SELECT id
                         FROM people
                        WHERE name = "Johnny Depp"
                           OR name = "Helena Bonham Carter"
                    )
           GROUP BY movie_id
             HAVING COUNT(movie_id) >= 2
          )
;