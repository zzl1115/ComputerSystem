all: run

run: Game
	java -cp bin:./libs/* edu.northeastern.cs5007.Game

Game: src/edu/northeastern/cs5007/Game.java
	javac -Xlint:deprecation -d bin -sourcepath src -cp ./libs/*:. src/edu/northeastern/cs5007/Game.java

clean:
	rm -rf bin/*
