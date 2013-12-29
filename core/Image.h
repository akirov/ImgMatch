#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>
#include <sys/param.h>  // For BYTE_ORDER definition in MinWG

#if ! defined(BYTE_ORDER) || ! defined(BIG_ENDIAN) || ! defined(LITTLE_ENDIAN)
#error "Endian not defiend!"
#endif


struct Pixel
{
/*  int x;
    int y; */

    Pixel() : ARGB(0) {}
    Pixel(unsigned long argb) : ARGB(argb) {}
    Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a=0xFF) :
#if BYTE_ORDER == BIG_ENDIAN
        A(a), 
        R(r), 
        G(g), 
        B(b)
#elif BYTE_ORDER == LITTLE_ENDIAN
        B(b), 
        G(g), 
        R(r), 
        A(a)
#else
#error "Unknown BYTE_ORDER"
#endif /* BYTE_ORDER */
    {}

    operator unsigned long () const { return ARGB; }

    union {
        struct {
#if BYTE_ORDER == BIG_ENDIAN
            unsigned char A;
            unsigned char R;
            unsigned char G;
            unsigned char B;
#elif BYTE_ORDER == LITTLE_ENDIAN
            unsigned char B;
            unsigned char G;
            unsigned char R;
            unsigned char A;
#else
#error "Unknown BYTE_ORDER"
#endif /* BYTE_ORDER */
        };
        unsigned long ARGB;
    };
};


class Image
{
  public:

    typedef enum 
    {
        ASPR_KEEP,
        ASPR_IGNORE
    } AspectRatio;


  public:

    static Image* ConstuctImage(std::string imageUrl);  // "Factory" instead of constructor.

    virtual ~Image() 
    {
    }

    virtual Image* Copy() const = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    virtual std::string GetName() const = 0;  // Or GetLocation()?

    virtual Pixel GetPixel( int x, int y ) const = 0;

    virtual void Scale(int width, int height, AspectRatio aspect_ratio = ASPR_IGNORE) = 0;

  private:

};


#endif /* __IMAGE_H__ */
