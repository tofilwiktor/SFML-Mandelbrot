#include <iostream>
#include <complex>
#include <tuple>
#include <SFML/Graphics.hpp>

#define WIDTH 1280
#define HEIGHT 720

const int N3 = (int) pow(256, 3);

std::tuple<int, int, int> n_to_rgb(int n, int max_iter) {
	long double n_normal = (long double)n / (long double)max_iter;
        unsigned short N = 256;
        
        n = (int)(n_normal * (long double) N3);

        int b = n / (N*N);

        int n_normal2 = n - b * N * N;

        int r = n_normal2 / N;
        int g = n_normal2  - r * N;
        

        return std::tuple<int, int, int>(r, g, b);
}


void mandelbrot(sf::VertexArray& va, int shift_x, int shift_y, int max_iter, float zoom) {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			long double x = ((long double)(j - shift_x)) / zoom;
			long double y = ((long double)(i - shift_y)) / zoom;

			std::complex<long double> c(x, y);
			int n = 0;

			std::complex<long double> z(0.0, 0.0);

			for (int k = 0; k < max_iter; k++) {
				std::complex<long double> z2(0.0, 0.0);
				z2.real(real(z) * real(z) - imag(z) * imag(z));
				z2.imag (2 * real(z) * imag(z));

				z.real(real(z2) + real(c));
				z.imag(imag(z2) + imag(c));

				n++;

				if ((real(z) * real(z) + imag(z) * imag(z)) > 4) {
					break;
				}
			}
                        std::tuple<int, int, int> temp = n_to_rgb(n, max_iter);

                        va[i*width + j].position = sf::Vector2f(j, i);
                        sf::Color color(std::get<0>(temp), std::get<1>(temp), std::get<2>(temp));

                        val[i * width + j].color = color;
		}
	}
}

int main () {
	
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot");
        
        window.setFramerateLimit(30);
        sf::VertexArray pixels(sf::Points, WIDTH * HEIGHT);

        float zoom = 200.f;
        int max_iter = 500;
        int shift_x = WIDTH / 2;
        int shift_y = HEIGHT / 2;

        mandelbrot(pixels, shift_x, shift_y, max_iter, zoom);



	return EXIT_SUCCESS;
}
