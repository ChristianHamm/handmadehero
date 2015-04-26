/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>

// NOTE: MAP_ANONYMOUS is not defined on Mac OS X and some other UNIX systems.
// On the vast majority of those systems, one can use MAP_ANON instead.
// Huge thanks to Adam Rosenfield for investigating this, and suggesting this
// workaround:
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

#define internal static
#define local_persist static
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

global_variable SDL_Texture *Texture;
global_variable void *BitmapMemory;
global_variable int BitmapWidth;
global_variable int BitmapHeight;
global_variable int BytesPerPixel = 4;

internal void
RenderWeirdGradient(int BlueOffset, int GreenOffset) {
    int Width = BitmapWidth;
    int Height = BitmapHeight;

    int Pitch = Width * BytesPerPixel;
    uint8 *Row = (uint8 *) BitmapMemory;

    for (int Y = 0; Y < BitmapHeight; ++Y) {
        uint32 *Pixel = (uint32 *) Row;
        for (int X = 0;
             X < BitmapWidth;
             ++X) {
            uint8 Blue = (X + BlueOffset);
            uint8 Green = (Y + GreenOffset);

            *Pixel++ = ((Green << 8) | Blue);
        }

        Row += Pitch;
    }
}

internal void
SDLResizeTexture(SDL_Renderer *Renderer, int Width, int Height) {
    if (BitmapMemory) {
        munmap(BitmapMemory,
               BitmapWidth * BitmapHeight * BytesPerPixel);
    }
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
    Texture = SDL_CreateTexture(Renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                Width,
                                Height);
    BitmapWidth = Width;
    BitmapHeight = Height;
    BitmapMemory = mmap(0,
                        Width * Height * BytesPerPixel,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1,
                        0);
}

internal void
SDLUpdateWindow(SDL_Window *Window, SDL_Renderer *Renderer) {
    SDL_UpdateTexture(Texture,
                      0,
                      BitmapMemory,
                      BitmapWidth * BytesPerPixel);

    SDL_RenderCopy(Renderer,
                   Texture,
                   0,
                   0);

    SDL_RenderPresent(Renderer);
}


bool HandleEvent(SDL_Event *Event) {
    bool ShouldQuit = false;

    switch (Event->type) {
        case SDL_QUIT: {
            printf("SDL_QUIT\n");
            ShouldQuit = true;
        }
            break;

        case SDL_WINDOWEVENT: {
            switch (Event->window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED: {
                    SDL_Window *Window = SDL_GetWindowFromID(
                            Event->window.windowID);
                    SDL_Renderer *Renderer = SDL_GetRenderer(Window);
                    printf("SDL_WINDOWEVENT_SIZE_CHANGED (%d, %d)\n",
                           Event->window.data1, Event->window.data2);
                    SDLResizeTexture(Renderer, Event->window.data1,
                                     Event->window.data2);
                }
                    break;

                case SDL_WINDOWEVENT_FOCUS_GAINED: {
                    printf("SDL_WINDOWEVENT_FOCUS_GAINED\n");
                }
                    break;

                case SDL_WINDOWEVENT_EXPOSED: {
                    SDL_Window *Window = SDL_GetWindowFromID(
                            Event->window.windowID);
                    SDL_Renderer *Renderer = SDL_GetRenderer(Window);
                    SDLUpdateWindow(Window, Renderer);
                }
                    break;
            }
        }
            break;
    }

    return (ShouldQuit);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    // Create our window.
    SDL_Window *Window = SDL_CreateWindow("Handmade Hero",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640,
                                          480,
                                          SDL_WINDOW_RESIZABLE);
    if (Window) {
        // Create a "Renderer" for our window.
        SDL_Renderer *Renderer = SDL_CreateRenderer(Window,
                                                    -1,
                                                    0);
        if (Renderer) {
            bool Running = true;
            int Width, Height;
            SDL_GetWindowSize(Window, &Width, &Height);
            SDLResizeTexture(Renderer, Width, Height);
            int XOffset = 0;
            int YOffset = 0;
            while (Running) {
                SDL_Event Event;
                while (SDL_PollEvent(&Event)) {
                    if (HandleEvent(&Event)) {
                        Running = false;
                    }
                }
                RenderWeirdGradient(XOffset, YOffset);
                SDLUpdateWindow(Window, Renderer);

                ++XOffset;
                YOffset += 2;
            }
        }
        else {
            // TODO(casey): Logging
        }
    }
    else {
        // TODO(casey): Logging
    }

    SDL_Quit();
    return (0);
}
