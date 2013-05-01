typedef struct{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}color;

color mandelbrotPixelColor(double xC, double yC, double centreX, double centreY, int zoom);
