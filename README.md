# Mandelbrot Fractal

## Compilar:
### Linux:
gcc main.c -o main -pthread

### Mac/BSD
gcc -o mandelbrot  mandelbrot.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL

## Rodar:
./main 1024 768 1000 -1.0 -1.0
./mandelbrot
