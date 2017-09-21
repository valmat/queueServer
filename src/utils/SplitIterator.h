 /**
 *  SplitIterator.h
 *
 *  @author valmat <ufabiz@gmail.com>
 */

#pragma once

namespace RocksUtils {

    template<typename StrType>
    class SplitIterator
    {

    public:
        // Invalit iterator
        explicit SplitIterator(const char* data, size_t size, size_t limit, const char delim, bool isInValid) noexcept
        :
            _lpos(size+1),
            _rpos(size),
            _limit(limit),
            _size(size),
            _data(data),
            _delim(delim)
        {}

        explicit SplitIterator(const char* data, size_t size, size_t limit, const char delim) noexcept
        :
            _limit(limit),
            _size(size),
            _data(data),
            _delim(delim)
        {
            _setPos();
        }

        SplitIterator(const SplitIterator& that) = default;
        SplitIterator(SplitIterator&& that)  = default;

        SplitIterator& operator++() noexcept
        {
            _lpos = _rpos+1;
            ++_cntr;
            _setPos();
            return *this;
        }

        SplitIterator operator++(int) noexcept
        {
            SplitIterator tmp(*this);
            operator++();
            return tmp;
        }

        bool operator==(const SplitIterator& rhs) const noexcept
        {
            return (
                _lpos  == rhs._lpos  &&
                _rpos  == rhs._rpos  &&
                _limit == rhs._limit &&
                _size  == rhs._size  &&
                _data  == rhs._data
            );
        }

        bool operator!=(const SplitIterator& rhs) const noexcept
        {
            return !operator==(rhs);
        }

        StrType operator*() const noexcept
        {
            return StrType(_data + _lpos, _rpos-_lpos);
        }

        const char* operator ->() const noexcept
        {
            return _data + _lpos;
        }
    private:

        void _setPos() noexcept
        {
            _rpos = _find(_data, _size, _delim, _lpos);
            if(_rpos == std::string::npos || (_limit && _cntr == _limit - 1 )) {
                _rpos = _size;
            }
        }

        // Находит позицию символа в строке
        template <typename T>
        size_t _find(const T& str, const char src, size_t start_pos = 0) const noexcept
        {
            if(str.empty()) return std::string::npos;

            auto size = str.size();
            auto it =  str.data();
            for(size_t pos =  start_pos; pos < size; ++pos) {
                if(it[pos] == src) return pos;
            }
            return std::string::npos;
        }

        size_t _find(const char* data, size_t size,  const char src, size_t start_pos = 0) const noexcept
        {
            if(!size) return std::string::npos;
            for(size_t pos =  start_pos; pos < size; ++pos) {
                if(data[pos] == src) return pos;
            }
            return std::string::npos;
        }

        size_t _lpos  = 0; 
        size_t _rpos  = 0; 
        size_t _cntr  = 0;
        size_t _limit = 0;
        size_t _size;
        const char* _data;
        const char _delim;

    };

} // end of namespace RocksUtils

