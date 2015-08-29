
#include "lzr_ilda.h"

// the ILDA default color table
const ilda_color ilda_palette[] = {
//    R    G    B
    { 255, 0,   0   }, // Red
    { 255, 16,  0   },
    { 255, 32,  0   },
    { 255, 48,  0   },
    { 255, 64,  0   },
    { 255, 80,  0   },
    { 255, 96,  0   },
    { 255, 112, 0   },
    { 255, 128, 0   },
    { 255, 144, 0   },
    { 255, 160, 0   },
    { 255, 176, 0   },
    { 255, 192, 0   },
    { 255, 208, 0   },
    { 255, 224, 0   },
    { 255, 240, 0   },

    { 255, 255, 0   }, // Yellow
    { 224, 255, 0   },
    { 192, 255, 0   },
    { 160, 255, 0   },
    { 128, 255, 0   },
    { 96,  255, 0   },
    { 64,  255, 0   },
    { 32,  255, 0   },
    { 0,   255, 0   }, // Green
    { 0,   255, 36  },
    { 0,   255, 73  },
    { 0,   255, 109 },
    { 0,   255, 146 },
    { 0,   255, 182 },
    { 0,   255, 219 },
    { 0,   255, 255 }, // Cyan

    { 0,   227, 255 }, 
    { 0,   198, 255 },
    { 0,   170, 255 },
    { 0,   142, 255 },
    { 0,   113, 255 },
    { 0,   85,  255 },
    { 0,   56,  255 },
    { 0,   28,  255 },
    { 0,   0,   255 }, // Blue
    { 32,  0,   255 },
    { 64,  0,   255 },
    { 96,  0,   255 },
    { 128, 0,   255 },
    { 160, 0,   255 },
    { 192, 0,   255 },
    { 224, 0,   255 },

    { 255, 0,   255 }, // Magenta
    { 255, 32,  255 },
    { 255, 64,  255 },
    { 255, 96,  255 },
    { 255, 128, 255 },
    { 255, 160, 255 },
    { 255, 192, 255 },
    { 255, 224, 255 },
    { 255, 255, 255 }, // White
    { 255, 224, 224 },
    { 255, 192, 192 },
    { 255, 160, 160 },
    { 255, 128, 128 },
    { 255, 96,  96  },
    { 255, 64,  64  },
    { 255, 32,  32  }
};

const int ilda_color_count = sizeof(ilda_palette) / sizeof(ilda_color);

//helper function to safely free any old color palette
void free_color_table(ilda_parser* ilda)
{
    ilda->nc = 0;
    if(ilda->c != NULL) free(ilda->c);
}

//safe color lookup
//if a palette hasn't been defined, then the default ILDA palette is used
ilda_color lookup_color(ilda_parser* ilda, size_t i)
{
    if((ilda->c == NULL) || (ilda->nc == 0))
    {
        //if no palette was defined, lookup in the default
        if(i < ilda_color_count) return ilda_palette[i];
        else                       return ilda_palette[ILDA_WHITE];
    }
    else
    {
        //use the custom palette
        if(i < ilda->nc) return ilda->c[i];
        else             return ilda->c[ILDA_WHITE];
    }
}
