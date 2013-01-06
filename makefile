all:
	g++ -Iinclude/ -c src/coatl_apple.cpp -o obj/coatl_apple.o
	g++ -Iinclude/ -c src/coatl_gen.cpp -o obj/coatl_gen.o
	g++ -Iinclude/ -c src/coatl_menu.cpp -o obj/coatl_menu.o
	g++ -Iinclude/ -c src/coatl_screen.cpp -o obj/coatl_screen.o
	g++ -Iinclude/ -c src/coatl_status.cpp -o obj/coatl_status.o
	g++ -Iinclude/ -c src/coatl_collision.cpp -o obj/coatl_collision.o
	g++ -Iinclude/ -c src/coatl_gfx.cpp -o obj/coatl_gfx.o
	g++ -Iinclude/ -c src/coatl_obstacle.cpp -o obj/coatl_obstacle.o
	g++ -Iinclude/ -c src/coatl_sfx.cpp -o obj/coatl_sfx.o
	g++ -Iinclude/ -c src/coatl_tile.cpp -o obj/coatl_tile.o
	g++ -Iinclude/ -c src/coatl_config.cpp -o obj/coatl_config.o
	g++ -Iinclude/ -c src/coatl_main.cpp -o obj/coatl_main.o
	g++ -Iinclude/ -c src/coatl_snake.cpp -o obj/coatl_snake.o
	g++ -Iinclude/ -c src/coatl_timer.cpp -o obj/coatl_timer.o
	g++ -Iinclude/ -c src/coatl_game.cpp -o obj/coatl_game.o
	g++ -Iinclude/ -c src/coatl_map.cpp -o obj/coatl_map.o
	g++ -Iinclude/ -c src/coatl_random.cpp -o obj/coatl_random.o
	g++ -Iinclude/ -c src/coatl_sprite.cpp -o obj/coatl_sprite.o
	g++ -Iinclude/ -c src/coatl_view.cpp -o obj/coatl_view.o
	g++ obj/coatl_apple.o obj/coatl_gen.o obj/coatl_menu.o obj/coatl_screen.o obj/coatl_status.o obj/coatl_collision.o obj/coatl_gfx.o obj/coatl_obstacle.o obj/coatl_sfx.o obj/coatl_tile.o obj/coatl_config.o obj/coatl_main.o obj/coatl_snake.o obj/coatl_timer.o obj/coatl_game.o obj/coatl_map.o obj/coatl_random.o obj/coatl_sprite.o obj/coatl_view.o -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -ljsoncpp -lnoise -o bin/coatl

