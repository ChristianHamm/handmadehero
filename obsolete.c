//
// Created by Christian Hamm on 26.04.15.
//

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

void Hero_PutPixel(void *pixel_buffer, int pitch,
                   const Uint32 x, const Uint32 y, const Uint32 color) {
    Uint8 *pixel = (Uint8 *) pixel_buffer;
    pixel += (y * pitch) + (x * sizeof(Uint32));
    *((Uint32 *) pixel) = color;
}

Uint32 Hero_RGBColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return (Uint32) ((a & 0xff) << 24)
           | ((b & 0xff) << 16)
           | ((g & 0xff) << 8)
           | ((r & 0xff));
}

void Hero_DebugDrawWeirdGradient() {
    const Uint32 width = g_pixel_buffer_width;
    const Uint32 height = g_pixel_buffer_height;

    static Uint32 blue_offset = 0;
    static Uint32 green_offset = 0;

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




