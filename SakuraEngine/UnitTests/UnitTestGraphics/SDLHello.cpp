/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:43:56
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-26 00:36:29
 */
#include <iostream>
extern "C"
{
	#ifdef _WIN32
	#include "SDL.h"
	#include "SDL_surface.h"
	#else
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_surface.h"
	#endif
	#undef main
}

using namespace std;
int main(int argc, char** argv)
{
	SDL_Window *win = SDL_CreateWindow("hello world!", 100, 100, 640, 480, SDL_WINDOW_RESIZABLE);
#ifdef _WIN32
	SDL_Surface *picture = SDL_LoadBMP("E:/material/lessons.bmp");
#else
	SDL_Surface *picture = SDL_LoadBMP("/home/saeruhikari/Downloads/test.bmp");
#endif
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, picture);
 
	SDL_FreeSurface(picture);
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);
	SDL_Delay(4000);
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
 
	cout << "Success!" << endl;
	return 0;
}