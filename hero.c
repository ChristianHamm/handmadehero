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
#include <sys/mman.h>
#include <SDL2/SDL.h>

#define log_debug(...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

const Uint32 k_window_width = 960;
const Uint32 k_window_height = 540;
//const Uint32 k_display_fps = 30;

const Uint16 k_audio_freq = 48000;
const Uint16 k_audio_rate = 60;
const Uint16 k_audio_channels = 2;

const Sint16 k_audio_bytes_per_sample = sizeof(Sint16) * 2;

static SDL_Texture *g_texture;
static void *g_pixel_buffer;
static Uint32 g_pixel_buffer_height;
static Uint32 g_pixel_buffer_width;
static const Uint32 k_bytes_per_pixel = 4;

static SDL_AudioSpec g_desired_audio_spec, g_audio_spec;
static SDL_AudioDeviceID g_audio_device;
static char *g_audio_device_name;

void Hero_PutPixel(void *pixel_buffer, int pitch,
                   const Uint32 x, const Uint32 y, const Uint32 color) {

    Uint8 *pixel = (Uint8 *) pixel_buffer;
    pixel += (y * pitch) + (x * sizeof(Uint32));
    *((Uint32 *) pixel) = color;
}

void Hero_ResizeTexture(SDL_Renderer *renderer, Uint32 width, Uint32 height) {
    // Clear _old_ pixel width and height
    if (g_pixel_buffer)
        munmap(g_pixel_buffer,
               g_pixel_buffer_width * g_pixel_buffer_height *
               k_bytes_per_pixel);

    if (g_texture)
        SDL_DestroyTexture(g_texture);

    g_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  width,
                                  height);

    // Set new pixel width and height
    g_pixel_buffer_height = height;
    g_pixel_buffer_width = width;

    g_pixel_buffer = mmap(0, g_pixel_buffer_width * g_pixel_buffer_height *
                             k_bytes_per_pixel,
                          PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANON,
                          -1, 0);

    memset(g_pixel_buffer, 0xff,
           g_pixel_buffer_width * g_pixel_buffer_height * k_bytes_per_pixel);

}

void Hero_DrawGradient(int frame_step) {
    for (int y = 0; y < g_pixel_buffer_height; y++) {
        for (int x = 0; x < g_pixel_buffer_width; x++) {
            Uint32 number = (Uint32) (y + frame_step);
            //Uint32 number = (Uint32) rand();
            Uint8 R = (Uint8) ((x) % 256);
            Uint8 G = (Uint8) ((x) % 256);
            Uint8 B = (Uint8) ((number) % 256);
            Uint32 color = (Uint32) ((0xff << 24)
                                     | ((B & (0xff)) << 16)
                                     | ((G & (0xff)) << 8)
                                     | ((R & 0xff)));
            Hero_PutPixel(g_pixel_buffer, g_pixel_buffer_width * 4,
                          (x - frame_step) % g_pixel_buffer_width,
                          (Uint32 const) y,
                          color);
        }
    }
}

void Hero_UpdateGraphics(SDL_Renderer *renderer) {
    if (g_pixel_buffer) if (SDL_UpdateTexture(g_texture, 0, g_pixel_buffer,
                                              g_pixel_buffer_width * 4)) {
        log_debug("Could not update g_texture!");
        exit(-1);
    }

    SDL_RenderCopy(renderer, g_texture, 0, 0);
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
                case SDL_WINDOWEVENT_EXPOSED:
                    SDL_Log("Window %d exposed", event.window.windowID);
                    Hero_UpdateGraphics(renderer);
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    SDL_Log("Window %d resized to %dx%d",
                            event.window.windowID, event.window.data1,
                            event.window.data2);
                    Hero_ResizeTexture(renderer, (Uint32) event.window.data1,
                                       (Uint32) event.window.data2);
                    break;

                default:
                    /*
                    SDL_Log("Window %d got unknown event %d",
                            event.window.windowID, event.window.event);
                    */
                    break;
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

void Hero_PlayTestSound() {
    // Retain the position where we are in the audio test loop
    static Uint32 audio_step = 0;
    Uint32 tone_hz = 440;
    Sint16 tone_volume = 3000;
    Uint32 square_wave_period = k_audio_freq / tone_hz;
    Uint32 half_square_wave_period = square_wave_period / 2;

    Uint32 target_queue_bytes = k_audio_freq * k_audio_bytes_per_sample;
    Uint32 bytes_to_write =
            target_queue_bytes - SDL_GetQueuedAudioSize(g_audio_device);

    if (bytes_to_write) {
        void *sound_buffer = malloc(bytes_to_write);
        Sint16 *sample_out = (Sint16 *) sound_buffer;
        Uint32 sample_count = bytes_to_write / k_audio_bytes_per_sample;

        for (Uint32 i = 0; i < sample_count; ++i) {
            Sint16 sample_value = ((audio_step++ /
                                    half_square_wave_period) % 2)
                                  ? tone_volume : -tone_volume;
            *sample_out++ = sample_value;
            *sample_out++ = sample_value;
        }
        SDL_QueueAudio(g_audio_device, sound_buffer, bytes_to_write);
        free(sound_buffer);
    }
}

void Hero_InitAudio() {
    SDL_zero(g_desired_audio_spec);
    g_desired_audio_spec.freq = k_audio_freq;
    g_desired_audio_spec.format = AUDIO_S16LSB;
    g_desired_audio_spec.channels = k_audio_channels;
    //g_desired_audio_spec.samples = 4096;
    g_desired_audio_spec.samples = (k_audio_freq * k_audio_bytes_per_sample /
                                    k_audio_rate);   g_audio_device = SDL_OpenAudioDevice(g_audio_device_name, 0,
                                         &g_desired_audio_spec, &g_audio_spec,
                                         SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (g_audio_device == 0) {
        log_debug("Failed to open audio: %s\n", SDL_GetError());
    } else {
        log_debug("Opened audio device %s", g_audio_device_name);

        if (g_audio_spec.format != g_desired_audio_spec.format)
            log_debug("Could not get desired audio format.\n");
    }
}

int main(int argc, char **argv) {
    // Init stuff
    srand((unsigned int) time(NULL));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Hero_PrintSDLVersion();

    // Audio stuff
    Hero_InitAudio();

    // Create the window and renderer
    SDL_Window *window = SDL_CreateWindow(
            "Handmade Hero",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            k_window_width, k_window_height,
            SDL_WINDOW_RESIZABLE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
            //                                            SDL_RENDERER_PRESENTVSYNC);
                                                SDL_RENDERER_PRESENTVSYNC);

    // Initially set the pixel buffer dimensions
    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    g_pixel_buffer_width = (Uint32) window_w;
    g_pixel_buffer_height = (Uint32) window_h;
    Hero_ResizeTexture(renderer, (Uint32) window_w, (Uint32) window_h);

    // Loop things
    int sound_is_playing = 0;
    Uint32 frame_step = 0;
    int running = 1;

    while (running) {
        // Performance
        Uint64 perf_freq = SDL_GetPerformanceFrequency();
        Uint64 perf_counter_start = SDL_GetPerformanceCounter();

        // Actual game stuff
        running = Hero_HandleEvents();
        Hero_DrawGradient(frame_step);
        Hero_UpdateGraphics(renderer);

        // Playing test sound
        Hero_PlayTestSound();

        if (!sound_is_playing) {
            SDL_PauseAudioDevice(g_audio_device, 1);
            sound_is_playing = 1;
        }

        // Performance
        Uint64 perf_counter_end = SDL_GetPerformanceCounter();
        Uint64 perf_counter_elapsed = perf_counter_end - perf_counter_start;
        double perf_per_frame = (((1000.0f * (double) perf_counter_elapsed) /
                                  (double) perf_freq));

        if ((frame_step % 100) == 0)
            log_debug("Frame time %d: %f", frame_step, perf_per_frame);

        frame_step++;
    }

    if (g_pixel_buffer)
        munmap(g_pixel_buffer,
               g_pixel_buffer_width * g_pixel_buffer_height *
               k_bytes_per_pixel);

    if (g_texture)
        SDL_DestroyTexture(g_texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

