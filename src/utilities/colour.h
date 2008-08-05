#ifndef COLOUR_H_INCLUDED
#define COLOUR_H_INCLUDED

typedef unsigned char ColourComponent;

struct Colour {
		ColourComponent red;
		ColourComponent green;
		ColourComponent blue;
		ColourComponent alpha;

		explicit Colour(const ColourComponent& r,
						const ColourComponent& g,
						const ColourComponent& b,
						const ColourComponent& a):
						red(r),
						green(g),
						blue(b),
						alpha(a) {	}

		explicit Colour(const ColourComponent& r,
						const ColourComponent& g,
						const ColourComponent& b):
						red(r),
						green(g),
						blue(b),
						alpha(255) {}

		Colour(const Colour& c):
		red(0), green(0), blue(0), alpha(0) {
			red = c.red;
			blue = c.blue;
			green = c.green;
			alpha = c.alpha;
		}
};

#endif // COLOUR_H_INCLUDED
