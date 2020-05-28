# Règles générales

Play-My-Life: obj/main.o obj/Carte.o obj/Entite.o
	g++ obj/main.o obj/Carte.o obj/Entite.o -o Play-My-Life -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm obj/*
	rm Play-My-Life

lineCount:
	git ls-files | xargs wc -l


# Règles de compilations de fichiers spécifiques

obj/%.o: src/%.cpp
	g++ -c $< -o $@
