#pragma once

#if defined(__GNUC__)

#define __STD_EXPECT(x, v) __builtin_expect((x), (v))

#else

#define __STD_EXPECT(x, v) (x) == (v)

#endif
