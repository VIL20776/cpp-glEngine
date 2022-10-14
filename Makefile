debug: main.o glBmp.o glObj.o glRender3D.o glShaders.o glBmpRender.o glFigures.o glLights.o glRaytracer.o liblinalg.o
	g++ -g build/main.o build/glBmp.o build/glObj.o build/glRender3D.o build/glShaders.o build/glBmpRender.o build/glFigures.o build/glLights.o build/glRaytracer.o build/liblinalg.o

main.o:
	g++ -g -c raytracer.cpp -o build/main.o
glBmp.o:
	g++ -g -c src/glBmp.cpp -o build/glBmp.o
glObj.o:
	g++ -g -c src/glObj.cpp -o build/glObj.o
glRender3D.o:
	g++ -g -c src/glRender3D.cpp -o build/glRender3D.o
glShaders.o:
	g++ -g -c src/glShaders.cpp -o build/glShaders.o
glBmpRender.o:
	g++ -g -c src/glBmpRender.cpp -o build/glBmpRender.o
glFigures.o:
	g++ -g -c src/Raytracer/glFigures.cpp -o build/glFigures.o
glLights.o:
	g++ -g -c src/Raytracer/glLights.cpp -o build/glLights.o
glRaytracer.o:
	g++ -g -c src/Raytracer/glRaytracer.cpp -o build/glRaytracer.o
liblinalg.o:
	g++ -g -c src/liblinalg.cpp -o build/liblinalg.o