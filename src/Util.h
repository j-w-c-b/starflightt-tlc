/*
        STARFLIGHT - THE LOST COLONY
        util.h - ?
        Author: ?
        Date: ?
*/

#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <allegro5/allegro.h>

#include "Point2D.h"

#define DEGS2RADS ((float)(3.14159 / 180.0))
#define CLOCKS2SECS(c) ((double)(c) / (double)CLOCKS_PER_SEC)

extern "C" int
sfrand(void);

extern "C" void
sfsrand(unsigned int seed);

class Util {
  public:
    /**
     * Initialize utility methods; this should be called before any other
     * methods in this class
     */
    static void Init();

    template <class T>
    static std::string ToString(const T &t, int width = 1, int precision = 2) {
        std::ostringstream oss;
        oss.precision(precision);
        oss.width(width);
        oss.fill('0');
        oss.setf(std::ios_base::fixed);
        oss << t;
        return oss.str();
    }

    static int StringToInt(const std::string &str) {
        int i;
        try {
            i = std::stoi(str);
        } catch (...) {
            return 0;
        }
        return i;
    }

    /**
     * returns a random number in the range [min.max] inclusive
     */
    static int Random(int min, int max) {
        return sfrand() % (max - min + 1) + min;
    }

    template <typename T> static auto random_choice(T t) {
        typename T::iterator start = t.begin();
        typename T::iterator end = t.end();
        auto dis = std::distance(start, end) - 1;
        std::advance(start, Random(0, dis));

        return *start;
    }

    /**
     * given a min-max range, this takes the value and wraps it as
     * necessary to keep it within the range [min, max)
     */
    static double
    WrapValue(double value, double min = 0.0, double max = 360.0) {
        /*
        if (val < min) { val = max + 1 + (val-min); }
        else if (val > max) { val = val - max - 1; }
        */
        if (min >= max) {
            return max;
        }

        if (value < min) {
            value = max + fmod((value - min), (max - min));
        } else if (value >= max) {
            value = fmod((value - min), (max - min));
        }

        return value;
    }

    // this function will continue to return false until millisecond delay has
    // transpired, which is useful when called from inside a loop or timed
    // routine
    static int ReentrantDelay(int ms);

    // converts a string to all upper case
    static std::string ToUpper(const std::string &str);

    static std::string resource_path(const std::string &relative_path) {
        static ALLEGRO_PATH *data_dir =
            al_get_standard_path(ALLEGRO_RESOURCES_PATH);
        if (relative_path[0] == '/') {
            return relative_path;
        }
        ALLEGRO_PATH *resource = al_create_path(relative_path.c_str());
        al_rebase_path(data_dir, resource);
        std::string res = al_path_cstr(resource, ALLEGRO_NATIVE_PATH_SEP);
        al_destroy_path(resource);
        return res;
    }
};

class Rect {
  public:
    int left;
    int top;
    int right;
    int bottom;

  public:
    Rect() {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }

    Rect(Point2D ul, Point2D lr) {
        left = (int)ul.x;
        top = (int)ul.y;
        right = (int)lr.x;
        bottom = (int)lr.y;
    }

    Rect(int l, int t, int r, int b) {
        left = l;
        top = t;
        right = r;
        bottom = b;
    }

    bool contains(Point2D p) { return contains(p.x, p.y); }

    bool contains(int x, int y) {
        return (x >= left && x <= right && y >= top && y <= bottom);
    }

    bool contains(double x, double y) {
        return (x >= left && x <= right && y >= top && y <= bottom);
    }
};

#endif
