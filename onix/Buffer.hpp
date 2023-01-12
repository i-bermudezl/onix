#pragma once

#include <memory>

template <typename T> class Buffer
{
  public:
    explicit Buffer(int width, int height);

    int getWidth() const;
    int getHeight() const;

    T read(int x, int y) const;
    void write(T value, int x, int y);

  private:
    int width;
    int height;
    std::unique_ptr<T[]> buffer;
};

template <typename T>
Buffer<T>::Buffer(int width, int height) : width{width}, height{height}, buffer{new T[width * height]}
{
}

template <typename T> int Buffer<T>::getWidth() const
{
    return width;
}

template <typename T> inline int Buffer<T>::getHeight() const
{
    return height;
}

template <typename T> inline T Buffer<T>::read(int x, int y) const
{
    return buffer[x + width * y];
}

template <typename T> void Buffer<T>::write(T value, int x, int y)
{
    buffer[x + width * y] = value;
}
