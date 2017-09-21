/**
 *  tolist.h
 *  Позволяет инициализировать список переменных на лету с помощью итератора
 *  Это может быть чистый итератор или итератор контейнера
 *
 *  @author valmat <ufabiz@gmail.com>
 */

#pragma once

namespace RocksUtils {
     
    /**
     * This is a helper class.
     * It can be used only inside the function ListInitializer tolist(Args&& ...args)
     * ListInitializer list(Args&& ...args)
     * In all other cases, use it not possible
     * size is the number of arguments with which the constructor was called
     */ 
    template<typename DataType, unsigned size>
    class ListInitializer
    {
    public:
         
        /**
          * Assignment operator
          * Ref-qualified forbids such things:
          * ListInitializer c(a,b);
          * c = arr;
          * You can only use this form:
          * ListInitializer(a,b) = arr;
          */
        template<typename ContainerType>
        ListInitializer &operator=(const ContainerType &arr) && noexcept // <-- Ref-qualified
        {
            auto it = arr.begin(), itend = arr.end();
            unsigned i = 0;
            while(it != itend && i < size) {
                *parr[i] = *it;
                ++it;
                ++i;
            }
            return *this;
        }
        template<typename ContainerType>
        ListInitializer &operator=(ContainerType &&arr) && noexcept // <-- Ref-qualified
        {
            auto it = arr.begin(), itend = arr.end();
            unsigned i = 0;
            while(it != itend && i < size) {
                *parr[i] = std::move(*it);
                ++it;
                ++i;
            }
            return *this;
        }
         
        /**
          * Deleted constructors. Forbids such things:
          * tolist q(a,b,c,d,e),w(a,b,c,d,e);
          * w = q;
          * You can only use this form:
          * tolist(a,b,c,d,e) = arr;
          */
        ListInitializer(const ListInitializer &that) = delete;
        ListInitializer() = delete;
         
         
    private:
        /**
          * Constructor with one argument
          */
        explicit ListInitializer(DataType& arg) noexcept //: size(1)
        {
            helper(0, arg);
        }
         
        /**
          * Constructor with a variable (>1) number of arguments
          * You can use any number of arguments of type DataType in the constructor.
          * tolist(a,b,c,d) = arr;
          * tolist(a,b) = arr;
          */
        template <typename ...Args>
        ListInitializer(DataType& arg0, Args&... args) noexcept //: size(sizeof...(Args)+1)
        {
            helper(0, arg0, args...);
        }
         
        /**
          * Move constructor
          */
        ListInitializer(ListInitializer &&that) noexcept
        {
            for(unsigned i = 0; i < size; i++) {
                parr[i] = that.parr[i];
                that.parr[i] = nullptr;
            }
        }

        /**
          * Helper method.
          * Allows to initialize the list of any number of arguments.
          * Alternately, one by one makes pointers to the arguments into the internal array.
          */
        template <typename ...Args>
        void helper(int ind, DataType& arg0, Args&... args) noexcept
        {
            helper(ind, arg0);
            helper(++ind, args...);
        }
         
        /**
          * Helper method.
          */
        void helper(int ind, DataType& arg0) noexcept
        {
            parr[ind] = &arg0;
        }
         
        template <typename T, typename ...Args>
        friend ListInitializer<T, sizeof...(Args)+1> tolist(T& arg0, Args&... args) noexcept;
         
        // Internal array of pointers to pointers to arguments
        DataType* parr[size];

    };
     
     
    template <typename DataType, typename ...Args>
    ListInitializer<DataType, sizeof...(Args)+1> tolist(DataType& arg0, Args&... args) noexcept
    {
        return ListInitializer<DataType, sizeof...(Args)+1>(arg0, args...);
    }
     

} // end of namespace RocksUtils