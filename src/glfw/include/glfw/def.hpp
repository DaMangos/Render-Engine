#pragma once

#include <array>
#include <cstddef>
#include <span>

namespace glfw
{

enum struct action
{
  release = 0,
  press   = 1,
  repeat  = 2,
};

enum struct hat
{
  centred    = 0,
  up         = 1,
  right      = 2,
  down       = 4,
  left       = 8,
  right_up   = (right | up),
  right_down = (right | down),
  left_up    = (left | up),
  left_down  = (left | down),
};

enum struct key
{
  unknown       = -1,
  space         = 32,
  apostrophe    = 39,
  comma         = 44,
  minus         = 45,
  period        = 46,
  slash         = 47,
  _0            = 48,
  _1            = 49,
  _2            = 50,
  _3            = 51,
  _4            = 52,
  _5            = 53,
  _6            = 54,
  _7            = 55,
  _8            = 56,
  _9            = 57,
  semicolon     = 59,
  equal         = 61,
  a             = 65,
  b             = 66,
  c             = 67,
  d             = 68,
  e             = 69,
  f             = 70,
  g             = 71,
  h             = 72,
  i             = 73,
  j             = 74,
  k             = 75,
  l             = 76,
  m             = 77,
  n             = 78,
  o             = 79,
  p             = 80,
  q             = 81,
  r             = 82,
  s             = 83,
  t             = 84,
  u             = 85,
  v             = 86,
  w             = 87,
  x             = 88,
  y             = 89,
  z             = 90,
  left_bracket  = 91,
  backslash     = 92,
  right_bracket = 93,
  grave_accent  = 96,
  world_1       = 161,
  world_2       = 162,
  escape        = 256,
  enter         = 257,
  tab           = 258,
  backspace     = 259,
  insert        = 260,
  del           = 261,
  right         = 262,
  left          = 263,
  down          = 264,
  up            = 265,
  page_up       = 266,
  page_down     = 267,
  home          = 268,
  end           = 269,
  caps_lock     = 280,
  scroll_lock   = 281,
  num_lock      = 282,
  print_screen  = 283,
  pause         = 284,
  f1            = 290,
  f2            = 291,
  f3            = 292,
  f4            = 293,
  f5            = 294,
  f6            = 295,
  f7            = 296,
  f8            = 297,
  f9            = 298,
  f10           = 299,
  f11           = 300,
  f12           = 301,
  f13           = 302,
  f14           = 303,
  f15           = 304,
  f16           = 305,
  f17           = 306,
  f18           = 307,
  f19           = 308,
  f20           = 309,
  f21           = 310,
  f22           = 311,
  f23           = 312,
  f24           = 313,
  f25           = 314,
  kp_0          = 320,
  kp_1          = 321,
  kp_2          = 322,
  kp_3          = 323,
  kp_4          = 324,
  kp_5          = 325,
  kp_6          = 326,
  kp_7          = 327,
  kp_8          = 328,
  kp_9          = 329,
  kp_decimal    = 330,
  kp_divide     = 331,
  kp_multiply   = 332,
  kp_subtract   = 333,
  kp_add        = 334,
  kp_enter      = 335,
  kp_equal      = 336,
  left_shift    = 340,
  left_control  = 341,
  left_alt      = 342,
  left_super    = 343,
  right_shift   = 344,
  right_control = 345,
  right_alt     = 346,
  right_super   = 347,
  menu          = 348,
};

enum struct modifier
{
  shift     = 0x0001,
  control   = 0x0002,
  alt       = 0x0004,
  super     = 0x0008,
  caps_lock = 0x0010,
  num_lock  = 0x0020,
};

enum struct mouse_button
{
  _1     = 0,
  _2     = 1,
  _3     = 2,
  _4     = 3,
  _5     = 4,
  _6     = 5,
  _7     = 6,
  _8     = 7,
  last   = _8,
  left   = _1,
  right  = _2,
  middle = _3,
};

enum struct gamepad_button : unsigned char
{
  a            = 0,
  b            = 1,
  x            = 2,
  y            = 3,
  left_bumper  = 4,
  right_bumper = 5,
  back         = 6,
  start        = 7,
  guide        = 8,
  left_thumb   = 9,
  right_thumb  = 10,
  dpad_up      = 11,
  dpad_right   = 12,
  dpad_down    = 13,
  dpad_left    = 14,
  last         = dpad_left,
  cross        = a,
  circle       = b,
  square       = x,
  triangle     = y,
};

enum struct gamepad_axis
{
  left_x        = 0,
  left_y        = 1,
  right_x       = 2,
  right_y       = 3,
  left_trigger  = 4,
  right_trigger = 5,
};

enum struct input_mode
{
  cursor               = 0x00033001,
  sticky_keys          = 0x00033002,
  sticky_mouse_buttons = 0x00033003,
  lock_mods            = 0x00033004,
  raw_mouse_motion     = 0x00033005,
};

template <class Type>
struct vec2
{
    union
    {
        Type x;
        Type width;
    };

    union
    {
        Type y;
        Type height;
    };
};

template <class Type>
struct vec3
{
    union
    {
        Type x;
        Type red;
    };

    union
    {
        Type y;
        Type green;
    };

    union
    {
        Type z;
        Type blue;
    };
};

template <class Type>
struct vec4
{
    union
    {
        Type x;
        Type red;
        Type left;
    };

    union
    {
        Type y;
        Type green;
        Type top;
    };

    union
    {
        Type z;
        Type blue;
        Type right;
    };

    union
    {
        Type w;
        Type alpha;
        Type bottom;
    };
};

using int2 = vec2<int>;
using int3 = vec3<int>;
using int4 = vec4<int>;

using float2 = vec2<float>;
using float3 = vec3<float>;
using float4 = vec4<float>;

using double2 = vec2<double>;
using double3 = vec3<double>;
using double4 = vec4<double>;

struct workarea
{
    int2 pos;
    int2 size;
};

struct vidmode
{
    int2 size;
    int3 bits;
    int  refresh_rate;
};

struct image
{
    int2                 size;
    std::span<std::byte> pixels;
};

struct gamepad_state
{
    std::array<gamepad_button, 15> buttons;
    std::array<float, 6>           axis;
};
}
