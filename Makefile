debug: main.o glBmp.o glBmpRender.o glObj.o glRender3D.o glShaders.o liblinalg.o
	g++ -g build/main.o build/glBmp.o build/glBmpRender.o build/glObj.o build/glRender3D.o build/glShaders.o build/liblinalg.o -fopenmp

main.o:
	g++ -g -c main.cpp -o build/main.o
glBmp.o:
	g++ -g -c src/glBmp.cpp -o build/glBmp.o
glBmpRender.o:
	g++ -g -c src/glBmpRender.cpp -o build/glBmpRender.o
glObj.o:
	g++ -g -c src/glObj.cpp -o build/glObj.o
glRender3D.o:
	g++ -g -c src/glRender3D.cpp -o build/glRender3D.o -fopenmp
glShaders.o:
	g++ -g -c src/glShaders.cpp -o build/glShaders.o
liblinalg.o:
	g++ -g -c src/liblinalg.cpp -o build/liblinalg.o