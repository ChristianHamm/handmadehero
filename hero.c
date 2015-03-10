/* ****************************************************************************
 *
 * Adaption of Casey's Handmade Hero in C with SDL
 *
 * Author: Christian Hamm
 * Date: 25.02.2015
 *
 *************************************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/mman.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_audio.h>

#define log_debug(...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

const Uint32 WIDTH = 640;
const Uint32 HEIGHT = 480;
const Uint32 DEPTH = 32;

const Uint32 AUDIO_FREQ = 48000;
const Uint32 AUDIO_FRAMERATE = 60;
const Uint32 AUDIO_CHANNELS = 2;

static SDL_Texture* texture;
static void* pixelBuffer;
static Uint32 pixelBufferHeight;
static Uint32 pixelBufferWidth;
static const Uint32 bytesPerPixel = 4;

static SDL_AudioSpec desiredAudioSpec, audioSpec;
static SDL_AudioDeviceID audioDevice;
static char* audioDeviceName;

void Hero_PutPixel(void *pixelBuffer, int pitch,
    const Uint32 x, const Uint32 y, const Uint32 color) {

    Uint8* pixel = (Uint8 *) pixelBuffer;
    pixel += (y * pitch) + (x * sizeof(Uint32));
    *((Uint32*) pixel) = color;
}

void Hero_ResizeTexture(SDL_Renderer* renderer, Uint32 width, Uint32 height) {
    // Clear _old_ pixel width and height
    if(pixelBuffer)
        munmap(pixelBuffer, pixelBufferWidth * pixelBufferHeight * bytesPerPixel);

    if(texture)
        SDL_DestroyTexture(texture);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         width,
                                         height);

    // Set new pixel width and height
    pixelBufferHeight = height;
    pixelBufferWidth = width;

    pixelBuffer = mmap(0, pixelBufferWidth * pixelBufferHeight * bytesPerPixel,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANON,
        -1, 0);

    memset(pixelBuffer, 0xff, pixelBufferWidth * pixelBufferHeight * bytesPerPixel);

}

void Hero_DrawGradient(int frame_step) {
    for (int y = 0; y < pixelBufferHeight ; y++) {
        for (int x = 0; x < pixelBufferWidth ; x++) {
            Uint32 number = y + frame_step;
            //Uint32 number = rand();
            Uint8 R = (x) % 256;
            Uint8 G = (x) % 256;
            Uint8 B = (number) % 256;
            Uint32 color = (0xff << 24)
                            | ((B & (0xff)) << 16)
                            | ((G & (0xff)) << 8)
                            | ((R & 0xff));
            Hero_PutPixel(pixelBuffer, pixelBufferWidth * 4,
                (x - frame_step) % pixelBufferWidth, y, color);
        }
    }
}

void Hero_UpdateGraphics(SDL_Renderer *renderer) {
    if(pixelBuffer)
        if (SDL_UpdateTexture(texture, 0, pixelBuffer, pixelBufferWidth * 4)) {
            log_debug("Could not update texture!");
            exit(-1);
        }

    SDL_RenderCopy(renderer, texture, 0, 0);
    SDL_RenderPresent(renderer);
};

int Hero_HandleEvents() {
    int running = 1;
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }

        if (event.type == SDL_WINDOWEVENT) {
            SDL_Window *window = SDL_GetWindowFromID(event.window.windowID);
            SDL_Renderer *renderer = SDL_GetRenderer(window);
            switch (event.window.event) {
                /*
            case SDL_WINDOWEVENT_SHOWN:
                SDL_Log("Window %d shown", event.window.windowID);
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                SDL_Log("Window %d hidden", event.window.windowID);
                break;
                */
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_Log("Window %d exposed", event.window.windowID);
                Hero_UpdateGraphics(renderer);
                break;
                /*
            case SDL_WINDOWEVENT_MOVED:
                SDL_Log("Window %d moved to %d,%d",
                        event.window.windowID, event.window.data1,
                        event.window.data2);
                break;
                */
            case SDL_WINDOWEVENT_RESIZED:
                SDL_Log("Window %d resized to %dx%d",
                        event.window.windowID, event.window.data1,
                        event.window.data2);
                Hero_ResizeTexture(renderer, event.window.data1, event.window.data2);
                break;
                /*
            case SDL_WINDOWEVENT_MINIMIZED:
                SDL_Log("Window %d minimized", event.window.windowID);
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                SDL_Log("Window %d maximized", event.window.windowID);
                break;
            case SDL_WINDOWEVENT_RESTORED:
                SDL_Log("Window %d restored", event.window.windowID);
                break;
            case SDL_WINDOWEVENT_ENTER:
                SDL_Log("Mouse entered window %d",
                        event.window.windowID);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                SDL_Log("Mouse left window %d", event.window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                SDL_Log("Window %d gained keyboard focus",
                        event.window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                SDL_Log("Window %d lost keyboard focus",
                        event.window.windowID);
                break;
            case SDL_WINDOWEVENT_CLOSE:
                SDL_Log("Window %d closed", event.window.windowID);
                break;
            default:
                SDL_Log("Window %d got unknown event %d",
                        event.window.windowID, event.window.event);
                break;
                */
            }
        }
    }

    return running;
}

void Hero_PrintSDLVersion() {
    SDL_version compiled;
    SDL_version linked;

    Uint32 subsystem_video_mask = SDL_INIT_VIDEO;
    Uint32 subsystem_audio_mask = SDL_INIT_AUDIO;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    log_debug("Init");
    log_debug("SDL Revision %s", SDL_GetRevision());

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    log_debug("Compiled against SDL version %d.%d.%d\n",
       compiled.major, compiled.minor, compiled.patch);
    log_debug("Linking against SDL version %d.%d.%d\n",
       linked.major, linked.minor, linked.patch);

    /* Check for two subsystems */
    if (SDL_WasInit(subsystem_video_mask) == subsystem_video_mask) {
        log_debug("Video initialized.");
    } else {
        log_debug("Video not initialized.");
    }

    if (SDL_WasInit(subsystem_audio_mask) == subsystem_audio_mask) {
        log_debug("Audio initialized.");
    } else {
        log_debug("Audio not initialized.");
    }
}

/*
void Hero_AudioCallback(void*  userdata, Uint8* stream, int len) {
    log_debug("Playing %d bytes", len);
    //stream = malloc(len * sizeof(Uint8));

    SDL_memset(stream, 0, len);

    for (int x = 0; x < len; x += 2) {
        Uint8* sbyte = (Uint8 *) stream;
        sbyte += x * sizeof(Uint8);

        if (x % 2) {
            *sbyte = 0;
            sbyte++;
            *sbyte = 1;
        }
        else {
            *sbyte = 0;
            sbyte++;
            *sbyte = -1;
        }
    }


    //log_debug("The audio callback is done!\n");
    //
    // amplitude * sin(2.0 * pi * f * f / framerate)

    //free(stream);
}
*/

int main(int argc, char ** argv) {
    // Init stuff
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Hero_PrintSDLVersion();

    // Audio stuff
    Uint32 frame_step = 0;
    Uint32 audio_step = 0;
    Uint32 tone_hz = 440;
    Sint16 tone_volume = 3000;
    Uint32 square_wave_period = AUDIO_FREQ / tone_hz;
    Uint32 half_square_wave_period = square_wave_period / 2;
    Uint32 bytes_per_sample = sizeof(Sint16) * 2;
    int sound_is_playing = 0;

    SDL_zero(desiredAudioSpec);
    desiredAudioSpec.freq = AUDIO_FREQ;
    desiredAudioSpec.format = AUDIO_S16LSB;
    desiredAudioSpec.channels = AUDIO_CHANNELS;
    //desiredAudioSpec.samples = 4096;
    desiredAudioSpec.samples = AUDIO_FREQ * bytes_per_sample / AUDIO_FRAMERATE;
    //desiredAudioSpec.callback = Hero_AudioCallback;
    //desiredAudioSpec.userdata = ;

    audioDevice = SDL_OpenAudioDevice(audioDeviceName, 0,
        &desiredAudioSpec, &audioSpec, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (audioDevice == 0) {
        log_debug("Failed to open audio: %s\n", SDL_GetError());
    } else {
        log_debug("Opened audio device %s", audioDeviceName);

        if (audioSpec.format != desiredAudioSpec.format)
            log_debug("Could not get desired audio format.\n");

        //SDL_PauseAudioDevice(audioDevice, 0);
        //SDL_Delay(5000);
        //SDL_CloseAudio();
    }

    // Create the window and renderer
    SDL_Window* window = SDL_CreateWindow(
        "Handmade Hero",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT,
        SDL_WINDOW_RESIZABLE);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        //SDL_RENDERER_PRESENTVSYNC);
         SDL_RENDERER_SOFTWARE);

    // Initially set the pixel buffer dimensions
    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    pixelBufferWidth = window_w;
    pixelBufferHeight = window_h;
    Hero_ResizeTexture(renderer, window_w, window_h);

    // Loop things
    int running = 1;

    while(running) {
        // Performance
        Uint64 perf_freq = SDL_GetPerformanceFrequency();
        Uint64 perf_counter_start = SDL_GetPerformanceCounter();

        // Actual game stuff
        running = Hero_HandleEvents();
        Hero_DrawGradient(frame_step);
        Hero_UpdateGraphics(renderer);

        // Playing test sound
        Uint32 target_queue_bytes = AUDIO_FREQ * bytes_per_sample;
        Uint32 bytes_to_write = target_queue_bytes - SDL_GetQueuedAudioSize(audioDevice);

        if(bytes_to_write) {
            void* sound_buffer = malloc(bytes_to_write);
            Sint16* sample_out = (Sint16*)sound_buffer;
            Uint32 sample_count = bytes_to_write / bytes_per_sample;

            for(Uint32 i = 0; i < sample_count; ++i) {
                Sint16 sample_value = ((audio_step++ / half_square_wave_period) % 2)
                    ? tone_volume : -tone_volume;
                *sample_out++ = sample_value;
                *sample_out++ = sample_value;
            }
            SDL_QueueAudio(audioDevice, sound_buffer, bytes_to_write);
            free(sound_buffer);
        }

        if(!sound_is_playing) {
            SDL_PauseAudioDevice(audioDevice, 0);
            sound_is_playing = 1;
        }

        // Performance
        Uint64 perf_counter_end = SDL_GetPerformanceCounter();
        Uint64 perf_counter_elapsed = perf_counter_end - perf_counter_start;
        double perf_per_frame = (((1000.0f * (double)perf_counter_elapsed) / (double)perf_freq));

        if((frame_step % 100) == 0)
            log_debug("Frame time %d: %f", frame_step, perf_per_frame);

        frame_step++;
    }

    if(pixelBuffer)
        munmap(pixelBuffer, pixelBufferWidth * pixelBufferHeight * bytesPerPixel);

    if(texture)
        SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

