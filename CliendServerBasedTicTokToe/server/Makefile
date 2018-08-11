all: run

run: ttt_server
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib ./ttt_server

ttt_server: ttt_server.c
	gcc -o ttt_server ttt_server.c -lulfius -ljansson

call_server:
	curl -XGET localhost:8080/helloworld

copy_needed_libs:
	mkdir -p libs
	cp /usr/local/lib/libjansson.so.4 ./libs/
	cp /usr/local/lib/libulfius.so.2.3 ./libs/
	cp /usr/lib/libyder.so.1.3.3 ./libs/
	cp /usr/lib/liborcania.so.1.2.3 ./libs/

docker_server_make: ttt_server copy_needed_libs
	# Copy systems libs into a local dir so that we can include them
	# in the Docker container
	docker build -t ttt-server .

# Running this way will disconnect docker from your terminal.
# You cannot use getchar() if you run this way
docker_server_run:
	docker run -p 8080:8080 ttt-server:latest

# Running this way will connect your terminal to the docker container,
# allowing getchar() to work as a method for shutting down the server
docker_server_run_getchar:
	docker run -i -t -p 1234:1234 ttt-server:latest

clean:
	rm -rf ./ttt_server ./libs
	sudo docker images -a | grep "ttt_server" | awk '{print $3}' | xargs docker rmi
