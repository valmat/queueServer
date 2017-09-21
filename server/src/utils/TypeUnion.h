
#pragma once

template <typename ...Types>
class TypeUnion
{
    static_assert (  sizeof...(Types), "TypeUnion is empty" );
public:
    // Разрешаем создание неинициализированных объектов
    TypeUnion() = default;

    // Запретим копирование и перемещение
    TypeUnion(const TypeUnion &) = delete;
    TypeUnion(TypeUnion &&)      = delete;
    TypeUnion &operator=(const TypeUnion &) = delete;
    TypeUnion &operator=(TypeUnion &&)      = delete;

    ~TypeUnion()
    {
        // Проверяем был ли размещен какой-нибудь объект
        // если да, разрушаем его
        if(deleter) {
            deleter(&storage);
            deleter = nullptr;
        }
    }

    // этот метод размещает в "объединении" объект типа T
    // при этом тип T должен быть перечислен среди типов указанных при создании объединения
    // Список аргументов args будет передан конструктору
    template <typename T, typename ...Args>
    TypeUnion& assign(Args&&... args)
    {
        // Проверяем на этапе компиляции возможность создания объекта в "объединении"
        static_assert ( same_as<T>(), "Type must be present in the types list " );

        // Проверяем не размещен ли уже какой-то объект в памяти
        // Если размещен, освобождаем память от него.
        if(deleter) deleter(&storage);

        // В выделенной памяти создаем объект типа Т
        // Создаем объект, используя точную передачу аргументов
        new(&storage) T(std::forward<Args>(args)...);
        
        // эта функция корректно разрушит инстацированный объект
        deleter = freeMem<T>;

        return *this;
    }

    // Получаем указатель на размещенный в "объединении" объект
    template<typename T>
    T* get()
    {
        // if TypeUnion::assign was not called, then return nullptr
        return (deleter ? static_cast<T*>(static_cast<void*>(&storage)) : nullptr);
    }

    // Получаем указатель на размещенный в "объединении" объект
    template<typename T>
    T const* get() const
    {
        // if TypeUnion::assign was not called, then return nullptr
        return (deleter ? static_cast<T const*>(static_cast<void const*>(&storage)) : nullptr);
    }

private:
    // функция этого типа будет использована для вызова деструктора
    typedef void (deleter_t)(void *);

    // вычисляем максимум на этапе компиляции
    template <typename T>
    static constexpr T max(T r0){return r0;}
    template <typename T, typename ...R>
    static constexpr T max(T r0, R... r){return ( r0 > max(r...) ? r0 : max(r...) );}

    // is_same для нескольких типов
    template <typename T>
    static constexpr bool same_as(){return max( std::is_same<T, Types>::value... );}

    // шаблонная функция используется для разрушения размещенного в памяти объекта
    template<typename T>
    static void freeMem(void *p)
    {
        static_cast<T*>(static_cast<void*>(p))->T::~T();
    }

    // Выделяем память, вмещающую объект наиболшего типа
    struct 
    { 
        // Вычисляем максимальный размер и максимальное выравнивание
        // из содержащихся типов на этапе компиляции
        alignas(max(alignof(Types)...)) unsigned char _[max( sizeof(Types)... )];
    } storage;

    deleter_t *deleter = nullptr;
};