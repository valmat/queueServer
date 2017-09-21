 /**
 *  Splitable.h
 *
 *  @author valmat <ufabiz@gmail.com>
 */
 
#pragma once

#include "SplitIterator.h"


namespace RocksUtils {

    template<typename StrType>
    class Splitable
    {
    public:
        typedef SplitIterator<StrType> iterator;

        Splitable(const std::string &str, const char delim, size_t limit = 0) noexcept
            :
            _str(str),
            _limit(limit),
            _delim(delim)
        {}

        iterator begin() const noexcept
        {
            return SplitIterator<StrType>(_str.data(), _str.size(), _limit, _delim);
        }

        iterator end() const noexcept
        {
            return SplitIterator<StrType>(_str.data(), _str.size(), _limit, _delim, true);
        }

    private:
        std::string _str;
        size_t _limit;
        const char _delim; 
    };

    template<typename StrType = std::string>
    inline Splitable<StrType> toSplit(const std::string &str, const char delim, size_t limit = 0) noexcept
    {
        return Splitable<StrType>(str, delim, limit);
    }
    template<typename StrType = std::string>
    inline Splitable<StrType> toSplit(const rocksdb::Slice &str, const char delim, size_t limit = 0) noexcept
    {
        return Splitable<StrType>(str.ToString(), delim, limit);
    }


} // end of namespace RocksUtils