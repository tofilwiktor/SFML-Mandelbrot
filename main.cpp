#include <iostream>
#include <complex>
#include <tuple>
#include <SFML/Graphics.hpp>

int WIDTH = 1280;
int HEIGHT = 720;


const int N3 = (int) pow(256, 3);

std::tuple<int, int, int> n_to_rgb(int n, int max_iter) {
	long double n_normal = (long double)n / (long double)max_iter;
        int N = 256;
        
        n = (int)(n_normal * (long double) N3);

        int g = n / (N*N);

        int n_normal2 = n - g * N * N;

        int r = n_normal2 / N * N;
        int b = n_normal2 - r * N;
        

        return std::tuple<int, int, int>(r, g, b);
}


void mandelbrot(sf::VertexArray& va, int shift_x, int shift_y, int max_iter, float zoom) {

    #pragma omp parallel for
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

				if (real(z) * real(z) + imag(z) * imag(z) > 4) {
					break;
				}
			}
                        std::tuple<int, int, int> temp = n_to_rgb(n, max_iter);

                        va[i*WIDTH + j].position = sf::Vector2f(j, i);
                        sf::Color color(std::get<0>(temp), std::get<1>(temp), std::get<2>(temp));

                        va[i * WIDTH + j].color = color;
		}
	}

}

int main () {
        auto desktop = sf::VideoMode::getDesktopMode();	
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot");
        
        window.setPosition(sf::Vector2i(desktop.width / 2 - window.getSize().x / 2, desktop.height / 2 - window.getSize().y / 2));

        window.setFramerateLimit(60);
        sf::VertexArray pixels(sf::Points, WIDTH * HEIGHT);



        float zoom = 200.f;
        int max_iter = 100;
        int shift_x = WIDTH / 2;
        int shift_y = HEIGHT / 2;


        //mirrors the mandelbrot set on 1st generation
        HEIGHT /= 2;     

        mandelbrot(pixels, shift_x, shift_y, max_iter, zoom);
        
        HEIGHT *= 2;

        for (int i = HEIGHT / 2; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                pixels[i*WIDTH + j].position = sf::Vector2f(j, HEIGHT - i + HEIGHT / 2);
                pixels[i*WIDTH + j].color = pixels[(i-HEIGHT/2)*WIDTH + j].color;
            }
        }

        while(window.isOpen()) {
            sf::Event event;

            while(window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                shift_x -= pos.x - shift_x;
                shift_y -= pos.y - shift_y;

                zoom *= 2;
                //uncomment for increased precision after every zoom
        //        max_iter += 100;

                for (int i = 0; i < WIDTH * HEIGHT; i++) {
                    pixels[i].color = sf::Color::Black;
                }

                mandelbrot(pixels, shift_x, shift_y, max_iter, zoom);
            }
            window.clear();
            window.draw(pixels);
            window.display();
        }

 
	return EXIT_SUCCESS;
}
