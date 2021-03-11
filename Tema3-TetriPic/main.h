#define SCALE 10

#define ALB        rgb(255, 255, 255)
#define GALBEN     rgb(255, 255,   0)
#define ALBASTRU   rgb(  0,   0, 255)
#define ROSU       rgb(255,   0,   0)
#define VERDE      rgb(  0, 255,   0)
#define PORTOCALIU rgb(255, 140,   0)
#define ROZ        rgb(255,   0, 255)
#define MOV        rgb(130,   0, 255)
#define NEGRU      rgb(  0,   0,   0)

// Structura unui pixel
struct pixel {
	unsigned char b; // blue
	unsigned char g; // green
	unsigned char r; // red
};
typedef struct pixel Pixel;

struct image {
    int height;
    int width;
    Pixel** pixels;
};
typedef struct image Image;

Image new_image(int height, int width);
Image new_board(int height, int width);
