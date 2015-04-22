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
const double k_display_msmax = 1000.0 / 60;    // 30 FPS

const Uint16 k_audio_freq = 48000;
const Uint16 k_audio_rate = 60;
const Uint16 k_audio_channels = 2;

const Sint16 k_audio_bytes_per_sample = sizeof(Sint16) * 2;

static SDL_Texture *g_texture;
static void *g_pixel_buffer;
static Uint32 g_pixel_buffer_height;
static Uint32 g_pixel_buffer_width;
static const Uint32 k_bytes_per_pixel = (Uint32) sizeof(Uint32);

static SDL_AudioSpec g_desired_audio_spec, g_audio_spec;
static SDL_AudioDeviceID g_audio_device;
static char *g_audio_device_name;

static SDL_GameController *g_game_controller = NULL;
static int g_num_game_controllers = 0;
static int EVT_RIGHT = 0;
static int EVT_LEFT = 0;
static int EVT_UP = 0;
static int EVT_DOWN = 0;
static int EVT_BTNA = 0;
static int EVT_BTNB = 0;
static int EVT_BTNX = 0;
static int EVT_BTNY = 0;

void Hero_PutPixel(void *pixel_buffer, int pitch,
                   const Uint32 x, const Uint32 y, const Uint32 color) {
    Uint8 *pixel = (Uint8 *) pixel_buffer;
    pixel += (y * pitch) + (x * sizeof(Uint32));
    *((Uint32 *) pixel) = color;
}

void Hero_DebugDrawWeirdGradient() {
    const Uint32 width = g_pixel_buffer_width;
    const Uint32 height = g_pixel_buffer_height;

    static Uint32 blue_offset = 0;
    static Uint32 green_offset = 0;

    if (EVT_RIGHT)
        blue_offset -= 4;
    if (EVT_LEFT)
        green_offset += 4;

    ++blue_offset;
    green_offset += 2;

    Uint32 pitch = width * k_bytes_per_pixel;
    //log_debug("pitch %d", pitch);
    Uint8 *row = (Uint8 *) g_pixel_buffer;
    Uint32 *pixel = (Uint32 *) row;

    for (Uint32 y = 0; y < height; ++y) {

        for (Uint32 x = 0; x < width; ++x) {
            Uint8 blue = (Uint8) (x + blue_offset);
            Uint8 green = (Uint8) (x + green_offset);

            *pixel++ = ((green << 8) | blue);
        }

        //log_debug("row %d", *row);
        row += pitch;
    }
}

void Hero_DebugDrawRunningPixel(SDL_Renderer *renderer) {
    static Uint32 pos = 0;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    static SDL_Rect rectangle;

    rectangle.x += 0 + EVT_RIGHT * 8 - EVT_LEFT * 8;
    rectangle.y += 0 + EVT_DOWN * 8 - EVT_UP * 8;
    rectangle.w = 50;
    rectangle.h = 50;
    SDL_RenderFillRect(renderer, &rectangle);
}

Uint32 Hero_RGBColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return (Uint32) ((a & 0xff) << 24)
           | ((b & 0xff) << 16)
           | ((g & 0xff) << 8)
           | ((r & 0xff));
}

void Hero_DebugDrawGradient(int frame_step) {
    frame_step = 0;
    static int pmove = 0;

    if (EVT_RIGHT)
        pmove += 1;

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
                          (x - pmove * 4) % g_pixel_buffer_width,
                          (Uint32 const) y,
                          color);
        }
    }
}

void Hero_DebugDrawBox() {
    static int pmove = 0;

    if (EVT_RIGHT)
        pmove += 8;

    Hero_PutPixel(g_pixel_buffer, g_pixel_buffer_width, 100, 200,
                  0xffff0000);
    Hero_PutPixel(g_pixel_buffer, g_pixel_buffer_width, 100, 250,
                  0xff00ff00);
    Hero_PutPixel(g_pixel_buffer, g_pixel_buffer_width, 150, 200,
                  0xff0000ff);
    Hero_PutPixel(g_pixel_buffer, g_pixel_buffer_width, 150, 250,
                  0xffffffff);
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

void Hero_UpdateGraphics(SDL_Renderer *renderer) {
    if (g_pixel_buffer) if (SDL_UpdateTexture(g_texture, 0, g_pixel_buffer,
                                              g_pixel_buffer_width *
                                              k_bytes_per_pixel)) {
        log_debug("Could not update g_texture!");
        exit(-1);
    }

    SDL_RenderCopy(renderer, g_texture, 0, 0);
    SDL_RenderPresent(renderer);
};

void Hero_InitControllers() {
    log_debug("Init controllers...");

    if (g_game_controller)
        SDL_GameControllerClose(g_game_controller);

    g_num_game_controllers = SDL_NumJoysticks();
    log_debug("Found %d controller(s)", g_num_game_controllers);
    SDL_GameControllerEventState(SDL_IGNORE);

    for (int i = 0; i < g_num_game_controllers; ++i) {
        if (SDL_IsGameController(i)) {
            g_game_controller = SDL_GameControllerOpen(i);
            if (g_game_controller) {
                log_debug("Controller #%d name %s", i,
                          SDL_GameControllerNameForIndex(i));
                SDL_GameControllerEventState(SDL_ENABLE);
                break;
            } else {
                fprintf(stderr, "Could not open gamecontroller %i: %s\n", i,
                        SDL_GetError());
            }
        }
    }
}

int Hero_HandleEvents() {
    int running = 1;
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }

        // Controller plugged in or out
        if (event.type == SDL_CONTROLLERDEVICEADDED ||
            event.type == SDL_CONTROLLERDEVICEREMOVED)
            Hero_InitControllers();

        // Controller DPad stuff
        if (event.type == SDL_CONTROLLERBUTTONDOWN &&
            event.cbutton.state == SDL_PRESSED) {
            switch (event.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    EVT_RIGHT = 1;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    EVT_LEFT = 1;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    EVT_UP = 1;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    EVT_DOWN = 1;
                    break;
                default:
                    break;
            }

        }

        if (event.type == SDL_CONTROLLERBUTTONUP &&
            event.cbutton.state == SDL_RELEASED) {
            switch (event.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    EVT_RIGHT = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    EVT_LEFT = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    EVT_UP = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    EVT_DOWN = 0;

                default:
                    break;
            }
        }

        // Controller Axis stuff
        if (event.type == SDL_CONTROLLERAXISMOTION) {
            switch (event.caxis.axis) {
                case SDL_CONTROLLER_AXIS_LEFTX:
                    //log_debug("left axis: %d", event.caxis.value);
                    if (event.caxis.value > 25000) {
                        EVT_RIGHT = 1;
                        EVT_LEFT = 0;
                    }
                    else if (event.caxis.value < -25000) {
                        EVT_RIGHT = 0;
                        EVT_LEFT = 1;
                    }
                    else {
                        EVT_RIGHT = 0;
                        EVT_LEFT = 0;
                    }
                    break;
                default:
                    break;
            }
        }

        if (event.type == SDL_KEYDOWN && event.key.state == SDL_PRESSED) {
            log_debug("Key down %d - %s", event.key.keysym.sym,
                      SDL_GetKeyName(event.key.keysym.sym));

            switch (event.key.keysym.sym) {
                case SDLK_RIGHT:
                    EVT_RIGHT = 1;
                    break;
                case SDLK_LEFT:
                    EVT_LEFT = 1;
                    break;
                case SDLK_UP:
                    EVT_UP = 1;
                    break;
                case SDLK_DOWN:
                    EVT_DOWN = 1;
                    break;
                default:
                    break;
            }
        }

        if (event.type == SDL_KEYUP && event.key.state == SDL_RELEASED) {
            switch (event.key.keysym.sym) {
                case SDLK_RIGHT:
                    EVT_RIGHT = 0;
                    break;
                case SDLK_LEFT:
                    EVT_LEFT = 0;
                    break;
                case SDLK_UP:
                    EVT_UP = 0;
                    break;
                case SDLK_DOWN:
                    EVT_DOWN = 0;
                default:
                    break;
            }
        }

        if (event.type == SDL_WINDOWEVENT) {
            SDL_Window *window = SDL_GetWindowFromID(event.window.windowID);
            SDL_Renderer *renderer = SDL_GetRenderer(window);
            switch (event.window.event) {
                case SDL_WINDOWEVENT_EXPOSED:
                    SDL_Log("Window %d exposed", event.window.windowID);
                    //Hero_UpdateGraphics(renderer);
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
                    SDL_RenderClear(renderer);
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    SDL_Log("Window %d resized to %dx%d",
                            event.window.windowID, event.window.data1,
                            event.window.data2);
                    Hero_ResizeTexture(renderer,
                                       (Uint32) event.window.data1,
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

    log_debug("sizeof(Uint32) = %d", sizeof(Uint32));
    log_debug("sizeof(Uint8) = %d", sizeof(Uint8));
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
    g_desired_audio_spec.samples = (k_audio_freq *
                                    k_audio_bytes_per_sample /
                                    k_audio_rate);
    g_audio_device = SDL_OpenAudioDevice(g_audio_device_name, 0,
                                         &g_desired_audio_spec,
                                         &g_audio_spec,
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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
    Hero_PrintSDLVersion();

    // Audio stuff
    Hero_InitAudio();

    // Handled in event loop
    // Hero_InitControllers();

    // Create the window and renderer
    SDL_Window *window = SDL_CreateWindow(
            "Handmade Hero",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            k_window_width, k_window_height,
            SDL_WINDOW_RESIZABLE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Initially set the pixel buffer dimensions
    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    g_pixel_buffer_width = (Uint32) window_w;
    g_pixel_buffer_height = (Uint32) window_h;
    //Hero_ResizeTexture(renderer, (Uint32) window_w, (Uint32) window_h);

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
        //Hero_DebugDrawGradient(frame_step);
        //Hero_DebugDrawWeirdGradient();
        //Hero_DebugDrawBox();
        //Hero_UpdateGraphics(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rectangle;

        rectangle.x = 0;
        rectangle.y = 0;
        rectangle.w = g_pixel_buffer_width;
        rectangle.h = g_pixel_buffer_height;
        SDL_RenderFillRect(renderer, &rectangle);
        Hero_DebugDrawRunningPixel(renderer);

        SDL_RenderPresent(renderer);

        // Playing test sound
        Hero_PlayTestSound();

        if (!sound_is_playing) {
            SDL_PauseAudioDevice(g_audio_device, 1);
            sound_is_playing = 1;
        }

        // Performance
        Uint64 perf_counter_end = SDL_GetPerformanceCounter();
        Uint64 perf_counter_elapsed = perf_counter_end - perf_counter_start;
        double perf_per_frame = ((
                (1000.0f * (double) perf_counter_elapsed) /
                (double) perf_freq));

        // Ensure we are at a constant framerate
        double fps_padding_time = k_display_msmax - perf_per_frame;

        if (fps_padding_time > 0)
            SDL_Delay((Uint32) fps_padding_time);

        if ((frame_step % 100) == 0)
            log_debug("Frame time %d: %f, maxms %f, padding %f", frame_step,
                      perf_per_frame, k_display_msmax, fps_padding_time);

        frame_step++;
    }

    log_debug("Shutting down...");

    if (g_game_controller != NULL)
        SDL_GameControllerClose(g_game_controller);

    /*
    if (g_pixel_buffer)
        munmap(g_pixel_buffer,
               g_pixel_buffer_width * g_pixel_buffer_height *
               k_bytes_per_pixel);
               */

    if (g_texture)
        SDL_DestroyTexture(g_texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

