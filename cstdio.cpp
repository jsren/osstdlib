/* cstdio.cpp - (c) 2018 James Renwick */
#include <cstdio>
#include <string>

namespace std
{
    namespace __detail
    {
        int snprintf(char* buffer, std::size_t maxSize,
                     const char* format, const void** args) noexcept
        {
            auto len = strlen(format);
            int len_out = 0;
            size_t arg_index = 0;
            for (size_t i = 0; i < len; i++)
            {
                if (format[i] == '%' && ++i < len)
                {
                    if (format[i] == '%') {
                        if (maxSize != 0) *buffer++ = '%';
                    }



                    bool flag_left = false;
                    bool flag_zero = false;
                    bool flag_plus = false;
                    bool flag_space = false;
                    bool flag_hash = false;

                    for (; i < len; i++)
                    {
                        // Skip left-align flag and zero flag
                        if (format[i] == '-') {
                            flag_left = true;
                        }
                        else if (format[i] == '0') {
                            flag_zero = true;
                        }
                        else if (format[i] == '+') {
                            flag_plus = true;
                        }
                        else if (format[i] == ' ') {
                            flag_space = true;
                        }
                        else if (format[i] == '#') {
                            flag_hash = true;
                        }
                        else break;
                    }
                    if (i == len) break;

                    

                    if (format[i] == 'i' || format[i] == 'd')
                    {
                        int value = *reinterpret_cast<const int*>(args[arg_index++]);
                        
                    }

                    //else if (format[i] == )

                }
                else len_out++;
            }
        }

        int printf(const char* format, void** args)
        {
            std::string buffer{};
            buffer.reserve(snprintf(nullptr, 0, format, args));
            snprintf(buffer.data(), buffer.capacity(), format, args);

            __platform::size_t count = 0;
            auto res = __platform::__write(__platform::__stdout,
                buffer.data(), buffer.size(), count);

            if (res) return static_cast<int>(count);
            else {
                int out = static_cast<int>(res.rc);
                return out < 0 ? out : -out;
            }
        }
    }
}
