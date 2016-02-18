/*
 * c++练习-3
 * 模板 vector
 * 练习c++的模板编程
 * created by Yang Liu
 * 2016.02.17
 * version: 1.0
 */
 
#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <algorithm>
#include <limits>
#include <iterator>
#include <cstddef>

template <typename T, typename Allocator>
class Vector;

template <typename T, typename Allocator>
bool operator<(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs);
template <typename T, typename Allocator>
bool operator<=(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs);
template <typename T, typename Allocator>
bool operator> (const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs);
template <typename T, typename Allocator>
bool operator>=(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs);
template <typename T, typename Allocator>
bool operator==(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs);
template <typename T, typename Allocator>
bool operator!=(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs);

//默认构造器为allocator
template <typename T, typename Allocator = std::allocator<T>> 
class Vector
{
    class reverse_iterator;
    class const_reverse_iterator;

public:
    typedef size_t size_type;
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef Allocator allocator_type;
    typedef ptrdiff_t difference_type;//用于计算两指针or迭代器的差
    typedef reverse_iterator reverse_iterator;
    typedef const_reverse_iterator const_reverse_iterator;
    typedef T* pointer;
    typedef const T* const_pointer;

private:
    //逆序迭代器 non-const
    class reverse_iterator
    {
    public:
        explicit reverse_iterator(iterator it=nullptr):
            base_iterator(it){};
        
        reverse_iterator& operator++(){--base_iterator;return *this;};
        reverse_iterator operator++(int)
        {
            reverse_iterator re_it(base_iterator);
            --base_iterator;
            return re_it;
        };
        reverse_iterator& operator--(){++base_iterator;return *this;};
        reverse_iterator operator--(int)
        {
            reverse_iterator re_it(base_iterator);
            ++base_iterator;
            return re_it;
        };
        
        reference operator*(){return *(base_iterator-1);};
        pointer operator->(){return base_iterator-1;};
            
    private:
        iterator base_iterator;
        
    friend bool operator==(reverse_iterator lhs, reverse_iterator rhs)
    {return lhs.base_iterator==rhs.base_iterator;};
    friend bool operator!=(reverse_iterator lhs, reverse_iterator rhs)
    {return lhs.base_iterator!=rhs.base_iterator;};
    friend difference_type operator-(reverse_iterator lhs, reverse_iterator rhs)
    {return lhs.base_iterator-rhs.base_iterator;};
    };
    
    //逆序迭代器 const
    class const_reverse_iterator
    {
    public:
        explicit const_reverse_iterator(const_iterator it=nullptr):
            base_iterator(it){};
        
        const_reverse_iterator& operator++(){--base_iterator;return *this;};
        const_reverse_iterator operator++(int)
        {
            const_reverse_iterator re_it(base_iterator);
            --base_iterator;
            return re_it;
        };
        const_reverse_iterator& operator--(){++base_iterator;return *this;};
        const_reverse_iterator operator--(int)
        {
            const_reverse_iterator re_it(base_iterator);
            ++base_iterator;
            return re_it;
        };
            
        const_reference operator*(){return *(base_iterator-1);};
        const_pointer operator->(){return base_iterator-1;};
            
    private:
        const_iterator base_iterator;
        
    friend bool operator==(const_reverse_iterator lhs, const_reverse_iterator rhs)
    {return lhs.base_iterator==rhs.base_iterator;};
    friend bool operator!=(const_reverse_iterator lhs, const_reverse_iterator rhs)
    {return lhs.base_iterator!=rhs.base_iterator;};
    friend difference_type operator-(const_reverse_iterator lhs, const_reverse_iterator rhs)
    {return lhs.base_iterator-rhs.base_iterator;};
    };

public:
    //构造
    Vector() :
         elem(nullptr)
        ,first_free(nullptr)
        ,cap(nullptr)
        ,alloc(allocator_type())
        {};
    explicit Vector(size_type n, const_reference tval = value_type())
    {init_from_fill(n, tval);};
    Vector(std::initializer_list<value_type> il)
    {init_from_copy(il.begin(), il.end());};
    Vector(const Vector& rhs){init_from_copy(rhs.cbegin(), rhs.cend());};
    Vector(const Vector&& rhs):
         elem(rhs.elem)
        ,first_free(rhs.first_free)
        ,cap(rhs.cap)
        ,alloc(rhs.alloc)
        {rhs.elem = rhs.first_free = rhs.cap = nullptr;};
    template <typename It>
    Vector(It *beg, It *end){init_from_copy(beg, end);};

    //赋值
    template <typename It>  //不能在自我范围内赋值
    void assign(It *beg, It *end){create_from_copy(beg, end);};
    void assign(size_type n, const_reference tval){create_from_fill(n, tval);};
    
    Vector& operator=(std::initializer_list<value_type> il);
    Vector& operator=(const Vector& rhs);
    Vector& operator=(const Vector&& rhs);

    //取第一个元素 const
    const_reference front() const {return *cbegin();};
    //取第一个元素 non-const
    reference front() {return *begin();};
    //取最后一个元素 const
    const_reference back() const {return *crbegin();};
    //取最后一个元素 non-const
    reference back() {return *rbegin();};
    
    //访问元素 const
    const_reference operator[](size_type pos) const;
    const_reference at(size_type pos) const
    {return this->operator[](pos);};
    //访问元素 non-const
    reference operator[](size_type pos);
    reference at(size_type pos){return this->operator[](pos);};

    //在末尾添加元素
    void push_back(const_reference tval);
    void push_back(const value_type&& tval);
    //插入元素
    iterator insert(iterator it_pos, const_reference tval);
    iterator insert(iterator it_pos, size_type n, const_reference tval);
    template <typename It>
    iterator insert(iterator it_pos, It *beg, It *end);
    
    //清空容器
    void clear(){delete_elem();};
    //删除末尾元素
    void pop_back();
    //删除指定的元素
    iterator erase(iterator it_pos);
    iterator erase(iterator beg, iterator end);

    //交换两个容器的元素
    void swap(Vector& rhs);

    //分配空间
    void reserve(size_type n);
    //调整大小
    void resize(size_type n, const_reference tval = value_type());

    //当前所能容纳最大元素数
    const size_type capacity() const {return cap - elem;};
    //本容器所能容纳最大元素数
    const size_type max_size() const
    {return std::numeric_limits<size_type>::max() / sizeof(size_type);};
    //当前元素数
    const size_type size() const {return first_free - elem;};
    //容器是否为空
    bool empty() const {return first_free == elem;};

    //迭代器：第一个元素 non-const
    iterator begin() {return elem;};
    //迭代器：第一个元素 const
    const_iterator cbegin() const {return elem;};
    //迭代器：最后一个元素后面的位置 non-const
    iterator end() {return first_free;}; 
    //迭代器：最后一个元素后面的位置 const
    const_iterator cend() const {return first_free;};
        
    //迭代器：逆序第一个元素 non-const
    reverse_iterator rbegin() { return reverse_iterator(first_free); };
    //迭代器：逆序第一个元素 const
    const_reverse_iterator crbegin() const { return const_reverse_iterator(first_free); };
    //迭代器：逆序最后一个元素后面的位置 non-const
    reverse_iterator rend() { return reverse_iterator(elem); };
    //迭代器：逆序最后一个元素后面的位置 const
    const_reverse_iterator crend() const { return const_reverse_iterator(elem); };
    
    //返回内存分配器
    allocator_type get_allocator() const {return alloc;};

    //析构函数
    ~Vector() { free();};

private:
    iterator elem;    //第一个元素
    iterator first_free;    //最后一个元素之后的位置
    iterator cap;    //分配内存之后的位置

    allocator_type alloc;   //内存分配器

    //从其他来源元素初始化 
    template <typename It>
    void init_from_copy(It beg, It end);
    void init_from_fill(size_type n, const_reference tval);
    
    //从其他来源copy元素
    template <typename It>
    void create_from_copy(It beg, It end);
    void create_from_fill(size_type n, const_reference tval);

    //删除所有元素，但不释放空间
    void delete_elem();
    //删除所有元素，并释放空间
    void free();
    
    //当前是否还有空闲内存
    bool is_full() const {return first_free!=elem&&first_free == cap;};
    //扩大空间并拷贝元素
    void extend(size_type n = 0);
    //检查访问范围
    bool check(size_type pos) const {return pos < this->size();};

friend bool operator< <T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator<=<T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator> <T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator>=<T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator==<T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator!=<T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
};

//构造
template <typename T, typename Allocator>
template <typename It>
void Vector<T, Allocator>::init_from_copy(It beg, It end)
{
    elem = alloc.allocate(end - beg);//分配未初始化的空间
    first_free = std::uninitialized_copy(beg, end, elem);
    cap = first_free;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::init_from_fill(size_type n, const_reference tval)
{
    elem = alloc.allocate(n);//分配未初始化的空间
    first_free = std::uninitialized_fill_n(elem, n, tval);
    cap = first_free;
}

//赋值
template <typename T, typename Allocator>
template <typename It>
void Vector<T, Allocator>::create_from_copy(It beg, It end)
{
    if (end - beg <= this->capacity())
    {
        //如果现有空间可以容纳n个元素
        //则不必重新分配内存空间
        delete_elem();
        first_free = std::uninitialized_copy(beg, end, elem);
    }
    else
    {
        free();
        init_from_copy(beg, end);
    }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::create_from_fill(size_type n, const_reference tval)
{
    if (n <= this->capacity())
    {
        //如果现有空间可以容纳n个元素
        //则不必重新分配内存空间
        delete_elem();
        first_free = std::uninitialized_fill_n(elem, n, tval);
    }
    else
    {
        free();
        init_from_fill(n, tval);
    }
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(std::initializer_list<value_type> il)
{
    create_from_copy(il.begin(), il.end());
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& rhs)
{
    if (*this != rhs)
    {
        create_from_copy(rhs.cbegin(), rhs.cend());
    }
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector&& rhs)
{
    if (*this != rhs)
    {
        free();
        elem = rhs.elem;
        first_free = rhs.first_free;
        cap = rhs.cap;
        rhs.elem = nullptr;
        rhs.first_free = nullptr;
        rhs.cap = nullptr;
    }
    return *this;
}

//删除所有元素，但不释放空间
template <typename T, typename Allocator>
void Vector<T, Allocator>::delete_elem()
{
    auto p = first_free;
    while (p != elem)
    {
        alloc.destroy(--p);
    }
    first_free = elem;
}

//删除所有元素，并释放空间
template <typename T, typename Allocator>
void Vector<T, Allocator>::free()
{
    delete_elem();
    alloc.deallocate(elem, this->capacity());
    elem = nullptr;
    first_free = nullptr;
    cap = nullptr;
}

//访问元素 const
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::operator[](size_type pos) const
{
    try 
    {
        if (!check(pos))
        {
            throw std::runtime_error("Vector[]: out of range.");
        }
        return elem[pos];
    }
    catch (std::runtime_error &err)
    {
        std::cout << err.what() << std::endl << "Plz try again." << std::endl;
        exit(1);
    }
}

//访问元素 non-const
template <typename T, typename Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::operator[](size_type pos)
{
    return const_cast<reference>(
        static_cast<const Vector<T, Allocator>&>(*this)
        [pos]
    );
}

//在末尾添加元素
template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const_reference tval)
{
    if (this->is_full())
    {
        extend();
    }
    alloc.construct(first_free++, tval);
}

//在末尾添加元素 移动
template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const value_type&& tval)
{
    if (this->is_full())
    {
        extend();
    }
    alloc.construct(first_free++, std::move(tval));
}

//插入单个元素
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert
    (iterator it_pos, const_reference tval)
{
    if (this->is_full())
    {
        extend();
    }
    return insert(it_pos, 1, tval);
}

//插入n个指定元素
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert
       (iterator it_pos, size_type n, const_reference tval)
{
    if (n == 0)
    {
        return it_pos;
    }

    //插入位置前的元素数
    difference_type pre_pos = it_pos - elem;
    //插入位置后的元素数
    difference_type behind_pos = first_free - it_pos;

    if (n + this->size() > this->capacity())
    {
        //空间不够，扩充
        extend();
    }

    it_pos = elem + pre_pos;
    if (n<behind_pos)//待插入元素的位置后的元素数>n
    {
        //后移n个元素（构造）
        std::uninitialized_copy_n(first_free - n, n, first_free);
        //后移behind_pos-n个元素
        //待移动元素按从后向前的顺序，防止覆盖
        std::copy_backward(it_pos, first_free - n, first_free);
        //插入n个tval
        std::fill_n(it_pos, n, tval);
    }
    else if (behind_pos<n)//待插入元素的位置后的元素数<n
    {
        //后移behind_pos个元素
        std::uninitialized_copy(it_pos, first_free, it_pos + n);
        //构造插入n-behind_pos个tval
        std::uninitialized_fill_n(first_free, n - behind_pos, tval);
        //插入剩下的tval
        std::fill_n(it_pos, behind_pos, tval);
    }
    else //待插入元素的位置后的元素数==n
    {
        //后移n个元素（构造）
        std::uninitialized_copy_n(it_pos, n, first_free);
        //插入n个tval
        std::fill_n(it_pos, n, tval);
    }
    first_free += n;

    return it_pos;
}

//插入指定范围的元素
template <typename T, typename Allocator>
template <typename It>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert
   (iterator it_pos, It *beg, It *end)
{
    //待插入的元素数
    auto n = end - beg;
    if (n == 0)
    {
        return it_pos;
    }

    //插入位置前的元素数
    difference_type pre_pos = it_pos - elem;
    //插入位置后的元素数
    difference_type behind_pos = first_free - it_pos;

    if (n + this->size() > this->capacity())
    {
        //空间不够，扩充
        extend(n + this->size());
    }

    it_pos = elem + pre_pos;
    if (n<behind_pos)
    {
        //后移n个元素（构造）
        std::uninitialized_copy_n(first_free - n, n, first_free);
        //后移behind_pos-n个元素
        std::copy_backward(it_pos, first_free - n, first_free);
        //插入新元素
        std::copy(beg, end, it_pos);
    }
    else if (n>behind_pos)
    {
        //后移behind_pos个元素
        std::uninitialized_copy(it_pos, first_free, it_pos + n);
        //构造插入n-behind_pos个新元素
        std::uninitialized_copy(beg + behind_pos, end, first_free);
        //插入剩下的新元素
        std::copy(beg, beg + behind_pos, it_pos);
    }
    else
    {
        //后移n个元素（构造）
        std::uninitialized_copy_n(it_pos, n, first_free);
        //插入n个tval
        std::copy(beg, end, it_pos);
    }
    first_free += n;

    return it_pos;
}

//删除末尾元素
template <typename T, typename Allocator>
void Vector<T, Allocator>::pop_back()
{
    if (!this->empty())
    {
        alloc.destroy(--first_free);
    }
}

//删除指定的元素
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase(iterator it_pos)
{
    //如果传入的是尾后迭代器，直接返回
    if (it_pos != first_free)
    {
        std::copy(it_pos + 1, first_free, it_pos);
        alloc.destroy(--first_free);
    }
    return it_pos;
}

//删除指定范围内的元素
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase(iterator beg, iterator end)
{
    std::copy(end, first_free, beg);
    auto p = first_free - (end - beg);
    while (first_free != p)
    {
        alloc.destroy(--first_free);
    }

    return beg;
}

//扩大空间并拷贝元素
template <typename T, typename Allocator>
void Vector<T, Allocator>::extend(size_type n)
{
    //未指定n时默认按当前容量扩大一倍
    n = (n == 0) ? (this->capacity()==0? 1:this->capacity()*2):n;
    
    auto new_elem = alloc.allocate(n);//分配未初始化的空间
    //移动元素
    auto new_first_free = std::uninitialized_copy
        (std::make_move_iterator(elem)
            , std::make_move_iterator(first_free)
            , new_elem);
    free();
    elem = new_elem;
    first_free = new_first_free;
    cap = elem + n;
}

//交换两个Vecotr的元素
template <typename T, typename Allocator>
void Vector<T, Allocator>::swap(Vector& rhs)
{
    std::swap(elem, rhs.elem);
    std::swap(first_free, rhs.first_free);
    std::swap(cap, rhs.cap);
    std::swap(alloc, rhs.alloc);
}

//重新分配空间
template <typename T, typename Allocator>
void Vector<T, Allocator>::reserve(size_type n)
{
    if (n > this->capacity())
    {
        extend(n);
    }
}

//调整大小
template <typename T, typename Allocator>
void Vector<T, Allocator>::resize(size_type n, const_reference tval)
{
    int diff = this->size() - n;
    if (diff > 0)
    {
        while (diff--)
        {
            alloc.destroy(--first_free);
        }
    }
    else if (diff < 0)
    {
        diff = -diff;
        if (n > this->capacity())
        {
            extend(n);
        }
        while (diff--)
        {
            alloc.construct(first_free++, tval);
        }
    }
}

//逻辑运算
template <typename T, typename Allocator>
bool operator<(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
{
    auto it_l = lhs.cbegin();
    auto it_r = rhs.cbegin();
    for (;it_l != lhs.cend() && it_r != rhs.cend();++it_l, ++it_r)
    {
        if (*it_l < *it_r)
        {
            return true;
        }
        else if (*it_r < *it_l)
        {
            return false;
        }
    }
    return (it_l == lhs.cend() && it_r != rhs.cend()) ? true : false;
}

template <typename T, typename Allocator>
bool operator<=(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
{
    return lhs<rhs || lhs == rhs;
}

template <typename T, typename Allocator>
bool operator> (const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
{
    return rhs < lhs;
}

template <typename T, typename Allocator>
bool operator>=(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
{
    return rhs < lhs || lhs == rhs;
}

template <typename T, typename Allocator>
bool operator==(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
{
    return (lhs.empty() && rhs.empty())
        || (lhs.size() == rhs.size()
            && std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin())
            );
}

template <typename T, typename Allocator>
bool operator!=(const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
{
    return !(lhs == rhs);
}
#endif