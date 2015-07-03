#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>
#if 0  // This code is hardware-dependent
#include <sys/param.h>  // For BYTE_ORDER definition in MinWG

#if ! defined(BYTE_ORDER) || ! defined(BIG_ENDIAN) || ! defined(LITTLE_ENDIAN)
#error "Endian not defiend!"
#endif


struct PixelARGB
{
    PixelARGB() : ARGB(0) {}

    PixelARGB(unsigned long argb) : ARGB(argb) {}

    PixelARGB(unsigned char r, unsigned char g, unsigned char b, unsigned char a=0xFF) :
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

    ~PixelARGB() {}

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
#endif // 0


struct PixelRGB
{
    PixelRGB() : R(0), G(0), B(0) {}

    PixelRGB(unsigned char r, unsigned char g, unsigned char b) :
            R(r), G(g), B(b)
    {}

    ~PixelRGB() {}

    unsigned char R;
    unsigned char G;
    unsigned char B;
};


class Image
{
  public:

    typedef enum 
    {
        ASPR_KEEP,
        ASPR_IGNORE
    } AspectRatio;

    typedef enum
    {
        PIXEL_UNKNOWN=-1,  // Use when we don't care about the representation
        PIXEL_Gray8,       // 8 bits grayscale
        PIXEL_RGB24,       // 24 bits red-green-blue
        PIXEL_RGB32,       // 32 bits XX-red-green-blue
        PIXEL_ARGB32       // 32 bits alpha-red-green-blue
    } PixelRep;

  public:

    static Image* ConstuctImage(std::string image_url, PixelRep pixel_rep=PIXEL_UNKNOWN);  // "Factory" instead of a constructor.

    virtual ~Image() 
    {
    }

//  virtual Image* Copy() const = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    virtual std::string GetName() const = 0;  // Or GetURI()?

    virtual PixelRGB GetPixelRGB( int x, int y ) const = 0;
//  virtual unsigned char GetPixelGray( int x, int y ) const = 0;
//  virtual void* GetPixelPtr( int x, int y ) const = 0;

    virtual void Scale(int width, int height, AspectRatio aspect_ratio=ASPR_IGNORE) = 0;

    virtual void Convert(PixelRep newRep) = 0;

  private:

};


#endif /* __IMAGE_H__ */
