#ifndef COATL_PCH_HPP
#define COATL_PCH_HPP

// C library
#include <math.h>
#include <time.h>

// STL
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

// JSONCPP
#ifdef WIN32
#include <json/json.h>
#else // WIN32
#include <jsoncpp/json/json.h>
#endif // WIN32

// Noise
#ifdef WIN32
#include <noise/noise.h>
#else // WIN32
#include <libnoise/noise.h>
#endif // WIN32

#endif // COATL_PCH_HPP
