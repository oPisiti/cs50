-- Their names
   SELECT name
     FROM people p
    WHERE id IN (
          -- Ids of the actors also in Kevin Bacon's movies
             SELECT person_id
               FROM stars
              WHERE movie_id IN (
                    -- Kevin Bacon's movies
                       SELECT movie_id
                         FROM stars s
                        WHERE person_id IN (
                              -- Kevin Bacon's id
                                 SELECT id
                                   FROM people
                                  WHERE name = "Kevin Bacon"
                                    AND birth = 1958
                              )
                    )
          )
      AND name != "Kevin Bacon"
;
