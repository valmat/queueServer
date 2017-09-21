/**
 *  utils.h
 *  utilities
 *
 *  @author valmat <ufabiz@gmail.com>
 */

#pragma once

namespace RocksUtils {

    struct utils
    {
        typedef std::string::size_type size_type;

        /**
         *  Находит позицию символа в строке
         */
        template <typename T>
        static size_type find(const T& str, const char src, size_type start_pos = 0) noexcept
        {
            if(str.empty()) return npos;

            auto size = str.size();
            auto it =  str.data();
            for(size_type pos =  start_pos; pos < size; ++pos) {
                if(it[pos] == src) return pos;
            }
            return npos;
        }

        /**
         *  Находит позицию символа в строке (начиная справа)
         *  start_pos в данном случае позиция справа
         */
        template <typename T>
        static size_type rfind(const T& str, const char src, size_type start_rpos = npos, size_type start_lpos = 0) noexcept
        {
            if(str.empty()) return npos;

            if(start_rpos == npos) start_rpos = str.size() -1;
            auto it =  str.data();
            for(size_type pos = start_rpos; pos > start_lpos; --pos) {
                if(it[pos] == src) return pos;
            }
            if(it[start_lpos] == src) return start_lpos;
            return npos;
        }

        /**
         *  Возвращает неотрецательное число справа от последнего разделителя
         *  @param       key    a string type
         *  @param       src    delimeter
         */
        template <typename T>
        static size_t int_rfind(const T& key, const char src = ':') noexcept
        {
            auto pos = rfind(key, src);
            if(pos >= key.size()-1) return 0;
            return str2int<size_t>(rocksdb::Slice(key.data()+pos+1, key.size()-pos-1));
        }

        /**
         *  Split string by delimeter
         */
        /*
        static std::vector<rocksdb::Slice> split(const rocksdb::Slice &str, const char delim, size_t limit = 0)
        {
            std::vector<rocksdb::Slice> tokens;
            if(limit) tokens.reserve(limit);
            size_type lpos = 0, rpos = 0, size = str.size();
            size_t cntr = 0;

            while(lpos <= size && (!limit || cntr < limit))
            {
                rpos = find(str, delim, lpos);
                if(rpos == npos) {
                    rpos = size;
                }
                    
                tokens.push_back(rocksdb::Slice(str.data()+lpos, rpos-lpos));
                lpos = rpos+1;
                ++cntr;
            }
            return tokens;
        }
        */
        //
        // DODO!!! Нужен вариант с итератоором. Зачем каждый раз созхдавать вектор, если чаще просто нужен итератор!
        //
        // Вариант с rocksdb::Slice опасен использованием уже освобожденной памяти
        static std::vector<std::string> split(const rocksdb::Slice &str, const char delim, size_t limit = 0) noexcept
        {
            std::vector<std::string> tokens;
            if(limit) {
                tokens.reserve(limit);
            } else{
                size_t delimCnt = 0;
                const char * data = str.data();
                for(size_t i = 0; i < str.size(); ++i) {
                    if(delim == data[i]) ++delimCnt;
                }
                tokens.reserve(delimCnt);
            }
            size_type lpos = 0, rpos = 0, size = str.size();
            size_t cntr = 0;

            while(lpos <= size && (!limit || cntr < limit))
            {
                rpos = find(str, delim, lpos);
                if(rpos == npos || (limit && cntr == limit - 1 )) {
                    rpos = size;
                }
                tokens. emplace_back(std::string(str.data()+lpos, rpos-lpos));
                
                lpos = rpos+1;
                ++cntr;
            }
            return tokens;
        }


        /**
         *  Split string by delimeter (starting from the right)
         */
        static std::vector<rocksdb::Slice> rsplit(const rocksdb::Slice &str, const char delim, size_t limit = 0) noexcept
        {
            std::vector<rocksdb::Slice> tokens;
            if(limit) tokens.reserve(limit);
            size_type size = str.size();
            size_type lpos = 0;
            long rpos = size;
            size_t cntr = 0;
            while(rpos >= 0 && (!limit || cntr < limit))
            {
                lpos = rfind(str, delim, rpos-1);
                if(lpos == npos) {
                    //lpos = 0;
                    tokens.push_back(rocksdb::Slice(str.data(), rpos));
                    rpos = -1;
                } else if(lpos == 0) {
                    //lpos = 0;
                    tokens.push_back(rocksdb::Slice(str.data()+1, rpos-1));
                    rpos = -1;
                    if(!limit || cntr+1 < limit) {
                        tokens.push_back(rocksdb::Slice(str.data(), 0));
                    }
                } else {
                    tokens.push_back(rocksdb::Slice(str.data()+lpos+1, rpos-lpos-1));
                    rpos = lpos;
                } 
                ++cntr;
            }
            return tokens;
        }


        /**
         *  Split string by delimeter to int values
         *  @param       string
         *  @param       delimeter
         */
        template <typename NumType>
        static std::vector<NumType> int_split(const rocksdb::Slice &str, const char delim = ':', size_t limit = 0) noexcept
        {
            auto inv = split(str, delim, limit);
            std::vector<NumType> tokens;
            tokens.reserve(inv.size());

            auto it = inv.begin(), endit = inv.end();
            while (it != endit) {
                tokens.push_back(str2int<NumType>(*(it++)));
            }
            return tokens;
        }
        template <typename NumType>
        static std::vector<NumType> int_rsplit(const rocksdb::Slice &str, const char delim = ':', size_t limit = 0) noexcept
        {
            auto inv = rsplit(str, delim, limit);
            std::vector<NumType> tokens;
            tokens.reserve(inv.size());

            auto it = inv.begin(), endit = inv.end();
            while (it != endit) {
                tokens.push_back(str2int<NumType>(*(it++)));
            }
            return tokens;
        }

        
        static void dbg1(const rocksdb::Slice &str, const char delim, size_t limit = 0) noexcept
        {
            std::cout << str.ToString() << "\t split to " << delim << "  limit=" << limit << std::endl;
            auto v = rsplit(str, delim, limit);
            for(auto &it: v) {
                std::cout <<"["<< it.ToString() <<"]"<< std::endl;
            }
            std::cout << std::endl;
        }
        

        /**
         *  Split string by delimeter
         *  @param       string
         *  @param       delimeter
         */
        static std::vector<std::string> split(const std::string &str, const char delim) noexcept
        {
            auto beg = str.begin();
            std::vector<std::string> tokens;
            while(beg != str.end())
            {
                auto temp = std::find(beg, str.end(), delim);
                if(beg != str.end())
                    tokens.push_back(std::string(beg, temp));
                if(temp != str.end())
                    temp++;
                beg = temp;
            }
            return tokens;
        }


        template<typename T>
        static T str2int(const char * str) noexcept
        {
            if(!str) return 0;
            T rez = 0;
            T digit;
            // Перебирает все цифры до символа \0 или первого нецифрового символа
            for(T i = 0; str[i]; i++) {
                rez *= 10;
                //rez += char2int(str[i]);
                switch (str[i]) {
                    case '0': digit = 0; break;
                    case '1': digit = 1; break;
                    case '2': digit = 2; break;
                    case '3': digit = 3; break;
                    case '4': digit = 4; break;
                    case '5': digit = 5; break;
                    case '6': digit = 6; break;
                    case '7': digit = 7; break;
                    case '8': digit = 8; break;
                    case '9': digit = 9; break;
                    default : return rez;
                }
                rez += digit;
            }
            return rez;
        }


        template<typename T, typename R>
        static T str2int(const R &str)  noexcept
        { 
            //return str2int<T>(str.data());
            if(str.empty()) return 0;
            T rez = 0;
            T digit;
            auto size = str.size();
            // Перебирает все цифры до символа \0 или первого нецифрового символа или конца строки по длине
            for(T i = 0; (i < size) && str[i]; i++) {
                rez *= 10;
                switch (str[i]) {
                    case '0': digit = 0; break;
                    case '1': digit = 1; break;
                    case '2': digit = 2; break;
                    case '3': digit = 3; break;
                    case '4': digit = 4; break;
                    case '5': digit = 5; break;
                    case '6': digit = 6; break;
                    case '7': digit = 7; break;
                    case '8': digit = 8; break;
                    case '9': digit = 9; break;
                    default : return rez;
                }
                rez += digit;
            }
            return rez;
        }

        template<typename T>
        static T str2int(const char ch) noexcept
        {
            switch (ch) {
                case '1': return 1;
                case '2': return 2;
                case '3': return 3;
                case '4': return 4;
                case '5': return 5;
                case '6': return 6;
                case '7': return 7;
                case '8': return 8;
                case '9': return 9;
                default : return 0;
            }
        }

        // bool --> string
        static std::string bool2str(bool arg) noexcept
        {
            return arg ? "1" : "0" ;
        }

        // string --> bool
        template<typename T>
        static bool str2bool(const T& arg) noexcept
        {
            return (*arg.data() == '1');
        }

        /**
         *  aggregate strings
         *  Оптимизировано соединяет строки
         */
        //static std::string aggrStr(std::initializer_list<std::string> aggr, const char sep = ':')
        static std::string aggrStr(const std::initializer_list<std::string> &aggr, const char sep = ':') noexcept
        {
            size_t sz = 0;
            for(auto &it: aggr) { sz += it.size()+1;}
            //for(auto &it: aggr) { 
            //    std::cout << it << std::endl;
            //    sz += it.size()+1;
            //}
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

        static void replaceAll(std::string& str, const std::string& from, const std::string& to) noexcept
        {
            if(from.empty())
                return;
            size_t start_pos = 0;
            while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        }

        /**
         *  соеденяет элементы строкового итератора
         */
        template <typename T>
        static std::string join(T it_begin, T it_end, const std::string& sep) noexcept
        {
            if(it_begin == it_end) return "";
            std::string rez { *it_begin };
            ++it_begin;
            for(;it_begin != it_end; ++it_begin) {
                rez += sep + *it_begin;
            }
            return rez;
        }

        static void ltrim(std::string &str) noexcept
        {
            // trim from start
            str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        }
        static void rtrim(std::string &str) noexcept
        {
            // trim from end
            str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
        }
        static void trim(std::string &str) noexcept
        {
            ltrim(str);
            rtrim(str);
        }
        template <typename T>
        static T tolower(T &&str) noexcept
        {
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            return str;
        }
        template <typename T>
        static T toupper(T &&str) noexcept
        {
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);
            return str;
        }

        //static std::string::size_type npos;
        //enum{npos = std::string::npos};
        constexpr static auto npos = std::string::npos;

    };

    //utilities::size_type npos = std::string::npos;
}
