#include <SDL2/SDL.h>
#include "ini.h"

enum frakType {
    Mandelbrot,
    Julia
};

struct sComplex {
    double real;
    double imag;
};

struct sRange {
    double upper;
    double lower;
};

struct sParam {
    enum frakType type;
    struct sComplex initial;
    Uint32 iterations;
    double radius;
    struct sRange xlim;
    struct sRange ylim;
    Uint32 width;
    Uint32 height;
};

typedef struct sComplex tComplex;
typedef struct sRange tRange;
typedef struct sParam tParam;

SDL_Color GetColor(double v, const double vmin, const double vmax)
{
    SDL_Color c = {255, 255, 255, 0};
    double dv = vmax - vmin;

    if (v < vmin) {
        v = vmin;

    } else if (v > vmax) {
        c.r = 0;
        c.g = 0;
        c.b = 0;
        return c;
    }

    if (v < (vmin + 0.25 * dv)) {
        c.r = 0;
        c.g = (Uint8) (1020 * (v - vmin) / dv);

    } else if (v < (vmin + 0.5 * dv)) {
        c.r = 0;
        c.b = (Uint8) (255 - 1020 * (v - vmin) / dv);

    } else if (v < (vmin + 0.75 * dv)) {
        c.r = (Uint8) (1020 * (v - vmin) / dv);
        c.b = 0;

    } else {
        c.g = (Uint8) (255 - 1020 * (v - vmin) / dv);
        c.b = 0;
    }

    return c;
}

inline Uint32 GetIterations(tComplex z, const tComplex c, const tParam *param)
{
    double buffer;
    Uint32 i = 0;

    for ( ; i < param->iterations; ++i) {
        buffer = z.real;
        z.real = c.real + z.real * z.real - z.imag * z.imag;
        z.imag = c.imag + 2 * buffer * z.imag;

        if (param->radius < (z.real * z.real + z.imag * z.imag)) {
            break;
        }
    }
    return i;
}

Uint32 GetMandelbrotIterations(tComplex c, const tParam *param)
{
    return GetIterations(param->initial, c, param);
}

Uint32 GetJuliaIterations(tComplex z0, const tParam *param)
{
    return GetIterations(z0, param->initial, param);
}

SDL_Window* CreateView(char title[], const tParam* param)
{
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int) param->width, (int) param->height, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_LockSurface(surface);

    tComplex pt;
    SDL_Color color;
    Uint32 *pixmem32;

    Uint32 (*iterate)(tComplex, const tParam*);

    if (param->type == Mandelbrot) {
        iterate = &GetMandelbrotIterations;
    } else {
        iterate = &GetJuliaIterations;
    }

    double gx = (param->xlim.upper - param->xlim.lower) / param->width;
    double gy = (param->ylim.lower - param->ylim.upper) / param->height;

    for (Uint32 x = 0; x < param->width; ++x) {
        for (Uint32 y = 0; y < param->height; ++y) {

            pt.real = param->xlim.lower + gx * x;
            pt.imag = param->ylim.upper + gy * y;

            color = GetColor(iterate(pt, param), 0, param->iterations - 1);

            pixmem32 = (Uint32*) surface->pixels + (y * (Uint32) surface->pitch / sizeof(Uint32)) + x;
            *pixmem32 = SDL_MapRGB(surface->format, color.r, color.g, color.b);
        }
    }

    SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(window);

    return window;
}

static int ParseParameter(void* user, const char* section, const char* name, const char* value)
{
    tParam* param = (tParam*) user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    if (MATCH("calculation", "julia_set")) {
        param->type = (value[0] == 't' || value[0] == 'T' || value[0] == 'y' || value[0] == 'Y') ? Julia : Mandelbrot;

    } else if (MATCH("calculation", "initial_real")) {
        param->initial.real = atof(value);

    } else if (MATCH("calculation", "initial_imag")) {
        param->initial.imag = atof(value);

    } else if (MATCH("calculation", "iterations")) {
        param->iterations = (Uint32) atoi(value);

    } else if (MATCH("calculation", "radius")) {
        param->radius = atof(value);

    } else if (MATCH("image", "xlim_lower")) {
        param->xlim.lower = atof(value);

    } else if (MATCH("image", "xlim_upper")) {
        param->xlim.upper = atof(value);

    } else if (MATCH("image", "ylim_lower")) {
        param->ylim.lower = atof(value);

    } else if (MATCH("image", "ylim_upper")) {
        param->ylim.upper = atof(value);

    } else if (MATCH("window", "width")) {
        param->width = (Uint32) atoi(value);

    } else if (MATCH("window", "height")) {
        param->height = (Uint32) atoi(value);

    } else {
        return 0;
    }

    return 1;
}

tParam GetDefaultParam()
{
    tParam param;

    param.type = Mandelbrot;
    param.initial.real = 0;
    param.initial.imag = 0;
    param.iterations = 15;
    param.radius = 20;

    param.xlim.lower = -2.25;
    param.xlim.upper = 1;
    param.ylim.lower = -1.3;
    param.ylim.upper = 1.3;

    param.width = 800;
    param.height = 600;

    return param;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

#if SDL_VERSION_ATLEAST(2, 0, 8)
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif

    char title[80];

    SDL_Window* window;
    SDL_Event event;

    tParam param = GetDefaultParam();

    if (argc > 1) {
        if (ini_parse(argv[1], ParseParameter, &param)) {
            printf("Error reading %s\n", argv[1]);
            return 1;
        } else {
            strcpy(title, "FrakView (");
            strcat(title, argv[1]);
            strcat(title, ")");
        }
    } else {
        strcpy(title, "FrakView (default)");
    }

    window = CreateView(title, &param);

    do {
        SDL_WaitEvent(&event);
    } while (event.type != SDL_QUIT);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
