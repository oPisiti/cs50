SELECT title, rating
FROM (
    movies JOIN ratings 
    ON movies.id = ratings.movie_id
    AND movies.YEAR = 2010)
AS movies_complete    
ORDER BY movies_complete.rating DESC, title;

