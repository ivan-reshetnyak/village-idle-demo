#include "pch.h"

#ifndef GLEW_STATIC
#  define GLEW_STATIC
#endif
#include "glew/glew.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glew32s")

extern "C" {
#include "LibJPEG/jpeglib.h"
#include <setjmp.h>
}

#include "libpng/png.h"

#pragma comment(lib, "libpng")
#pragma comment(lib, "zlib")

#include <cstdio>

#include "tex_img.h"

namespace tex {
bool img::Load(const std::string& fileName)
{
    std::unique_ptr<uint8_t[]> buffer = nullptr;

    auto ext = fileName.substr(fileName.find_last_of('.') + 1);
    bool res = false;
    if (ext == "jpg") {
        res = LoadJPEG(fileName, buffer);
    } else if (ext == "png") {
        res = LoadPNG(fileName, buffer);
    }

    if (!res) {
        return false;
    }

    Enable();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    int format = channels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, format, GL_UNSIGNED_BYTE, &buffer[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureParameteri(glId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(glId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(glId, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTextureParameteri(glId, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    return true;
}


bool img::LoadJPEG(const std::string& fileName, std::unique_ptr<uint8_t[]>& dataBuf)
{
    struct errorMgr {
        struct jpeg_error_mgr pub;
        jmp_buf setjmp_buffer;
    } jerr;
    struct jpeg_decompress_struct cinfo;

    errno_t err;
    FILE* file;
    if ((err = fopen_s(&file, fileName.c_str(), "rb")) != 0) {
        return false;
    }

    cinfo.err = jpeg_std_error(&jerr.pub);
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(file);
        return false;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);

    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    width = cinfo.output_width;
    height = cinfo.output_height;
    channels = cinfo.out_color_components;

    size_t bufSize = sizeof(uint8_t) * width * height * channels;
    dataBuf = std::unique_ptr<uint8_t[]>(new uint8_t[bufSize]);
    size_t dataBufPos = 0;

    int rowStride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, rowStride, 1);

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy_s(&dataBuf[dataBufPos], rowStride, buffer[0], rowStride);
        dataBufPos += rowStride;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(file);
    return true;
}

bool img::LoadPNG(const std::string& fileName, std::unique_ptr<uint8_t[]>& buffer)
{
    png_image image{0};
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_file(&image, fileName.c_str()) != 0) {
        image.format = PNG_FORMAT_RGBA;
        buffer = std::unique_ptr<uint8_t[]>(new png_byte[PNG_IMAGE_SIZE(image)]);
        if (buffer && png_image_finish_read(&image, nullptr, &buffer[0], 0, nullptr) != 0) {
            width = image.width;
            height = image.height;
            channels = 4;
            png_image_free(&image);
            return true;
        } else {
            png_image_free(&image);
            return false;
        }
    }

    return false;
}
}
