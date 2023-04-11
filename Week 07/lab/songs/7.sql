SELECT AVG(songs.energy) FROM songs JOIN artists WHERE songs.artist_id IN (SELECT id FROM artists WHERE name = "Drake");
