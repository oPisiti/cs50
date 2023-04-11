SELECT title
  FROM movies m
 INNER JOIN (
        SELECT r.movie_id,
               rating
          FROM ratings r
         INNER JOIN (
                SELECT movie_id
                  FROM stars s
                 WHERE person_id = (
                        SELECT id
                          FROM people
                         WHERE name = "Chadwick Boseman"
                       )
               ) s
            ON r.movie_id = s.movie_id
       ) a
    ON m.id = a.movie_id
 ORDER BY rating DESC
 LIMIT 5