//
// Created by captain on 2021/4/5.
// 图片加载与解析
//

#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include <glad/gl.h>

class Texture2D
{
private:
    Texture2D(){};
    ~Texture2D(){};

public:
    static Texture2D* LoadFromFile(std::string& image_file_path);//加载一个图片文件

public:
    int width_;
    int height_;

    GLenum gl_texture_format_;//纹理ID
};
