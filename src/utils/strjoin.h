/**
 *  strjoin.h
 *
 *  @author valmat <ufabiz@gmail.com>
 */

#pragma once

namespace RocksUtils {
     
    /**
     *  Optimized aggregating strings
     *  used Args pack
     */
    template <unsigned short size>
    struct StrJoinInitializer
    {
        template <typename ...Args>
        StrJoinInitializer(Args&&... args) : aggr{helper(std::forward<Args>(args))...} {}

        /**
         *  Cast to a std::string
         */
        operator std::string () const
        {
            size_t len = 0;
            for(unsigned short i = 0; i < size; ++i) {
                len += aggr[i].size()+1;
            }
            --len;

            std::string rez;
            rez.reserve(len);

            rez += aggr[0];
            for(unsigned short i = 1; i < size; ++i) {
                rez += _sep;
                rez += aggr[i];
            }
            return rez;
        }

        StrJoinInitializer& sep(char s) &
        {
            _sep = s;
            return *this;
        }

        StrJoinInitializer&& sep(char s) &&
        {
            _sep = s;
            return std::move(*this);
        }

        StrJoinInitializer(const StrJoinInitializer& that) = delete;
        StrJoinInitializer(StrJoinInitializer&& that) = default;

    private:
        std::string& helper(std::string& s) { return s;}
        const std::string& helper(const std::string& s) { return s;}
        std::string&& helper(std::string&& s) { return std::move(s);}
        std::string helper(bool x) { return (x ? "1" : "0");}
        std::string helper(const char *x) { return std::string(x);}
        std::string helper(char x) { return std::string(1, x);}
        std::string helper(unsigned char x) { return std::to_string(x);}

        template<typename T>
        typename std::enable_if<std::is_arithmetic<T>::value, std::string>::type
        helper(T&& x) { return std::to_string(x);}

        std::string aggr[size];
        char _sep = ':';
    };

    // output stream operator
    template <unsigned short size>
    std::ostream &operator<<(std::ostream &stream, const StrJoinInitializer<size>& x)
    {
        return stream << static_cast<std::string>(x);
    }

    template <typename ...Args>
    StrJoinInitializer<sizeof...(Args)> strjoin(Args&&... args)
    {
        return StrJoinInitializer<sizeof...(Args)>(std::forward<Args>(args)...);
    }

    /**
     *  Optimized aggregating strings
     *  used initializer_list
     */
    //static std::string strjoin(std::initializer_list<std::string> aggr, const char sep = ':')
    template <typename T>
    static typename std::enable_if< std::is_same<T, std::initializer_list<std::string> >::value, std::string>::type
    strjoin(std::initializer_list<std::string> aggr, const char sep = ':')
    {
        size_t sz = 0;
        for(auto &it: aggr) { sz += it.size()+1;}
        --sz;
        std::string rez;
        rez.reserve(sz);

        auto it = aggr.begin(), itend = aggr.end();
        rez += *(it++);

        while( it != itend) {
            rez += sep;
            rez += *(it++);
        }
        return rez;
    }

}
