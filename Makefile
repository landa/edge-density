all:
	g++ ed.cpp -o ed `pkg-config --libs opencv`
