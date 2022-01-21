// Software textured sphere renderer
// It's slow, drawn with pixels, but no OGL is needed and it plays nicely with
// others. Adapted from code by Plucky:
// https://www.allegro.cc/forums/thread/186206/186206#target
//
// TEX_SIZE needs to be <=256 due to coord_transform_table holding shorts.
// In this code, it needs to be a power of 2, but a couple of minor tweaks,
// and this constraint can go away.

#pragma once

#include <allegro5/allegro.h>
#include <string>

class TexturedSphere {
  private:
    int TEX_SIZE;
    int MAP_SIZE;
    double ASPECT_RATIO;
    int *coord_transform_table;
    int *screen2sphere_table;
    ALLEGRO_COLOR *tex_table;
    ALLEGRO_BITMAP *source_bmp;
    bool textureWasSet; // do not destroy passed texture, only a loaded one

    void InitSphereLookupTables(void);
    void
    Spherical2Cartesian(int alpha, int beta, double *x, double *y, double *z);
    void Cartesian2Sphere(double x, double y, double z, int *alpha, int *beta);
    void CreateTextureTable(ALLEGRO_BITMAP *bmp);

  public:
    TexturedSphere(int tex_size);
    ~TexturedSphere(void);
    bool SetTexture(ALLEGRO_BITMAP *texture);
    void Draw(ALLEGRO_BITMAP *dest,
              int phi,
              int theta,
              int psi,
              int radius,
              int center_x,
              int center_y);
};
