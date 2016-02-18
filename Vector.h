/*
 * c++��ϰ-3
 * ģ�� vector
 * ��ϰc++��ģ����
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

//Ĭ�Ϲ�����Ϊallocator
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
    typedef ptrdiff_t difference_type;//���ڼ�����ָ��or�������Ĳ�
    typedef reverse_iterator reverse_iterator;
    typedef const_reverse_iterator const_reverse_iterator;
    typedef T* pointer;
    typedef const T* const_pointer;

private:
    //��������� non-const
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
    
    //��������� const
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
    //����
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

    //��ֵ
    template <typename It>  //���������ҷ�Χ�ڸ�ֵ
    void assign(It *beg, It *end){create_from_copy(beg, end);};
    void assign(size_type n, const_reference tval){create_from_fill(n, tval);};
    
    Vector& operator=(std::initializer_list<value_type> il);
    Vector& operator=(const Vector& rhs);
    Vector& operator=(const Vector&& rhs);

    //ȡ��һ��Ԫ�� const
    const_reference front() const {return *cbegin();};
    //ȡ��һ��Ԫ�� non-const
    reference front() {return *begin();};
    //ȡ���һ��Ԫ�� const
    const_reference back() const {return *crbegin();};
    //ȡ���һ��Ԫ�� non-const
    reference back() {return *rbegin();};
    
    //����Ԫ�� const
    const_reference operator[](size_type pos) const;
    const_reference at(size_type pos) const
    {return this->operator[](pos);};
    //����Ԫ�� non-const
    reference operator[](size_type pos);
    reference at(size_type pos){return this->operator[](pos);};

    //��ĩβ���Ԫ��
    void push_back(const_reference tval);
    void push_back(const value_type&& tval);
    //����Ԫ��
    iterator insert(iterator it_pos, const_reference tval);
    iterator insert(iterator it_pos, size_type n, const_reference tval);
    template <typename It>
    iterator insert(iterator it_pos, It *beg, It *end);
    
    //�������
    void clear(){delete_elem();};
    //ɾ��ĩβԪ��
    void pop_back();
    //ɾ��ָ����Ԫ��
    iterator erase(iterator it_pos);
    iterator erase(iterator beg, iterator end);

    //��������������Ԫ��
    void swap(Vector& rhs);

    //����ռ�
    void reserve(size_type n);
    //������С
    void resize(size_type n, const_reference tval = value_type());

    //��ǰ�����������Ԫ����
    const size_type capacity() const {return cap - elem;};
    //�����������������Ԫ����
    const size_type max_size() const
    {return std::numeric_limits<size_type>::max() / sizeof(size_type);};
    //��ǰԪ����
    const size_type size() const {return first_free - elem;};
    //�����Ƿ�Ϊ��
    bool empty() const {return first_free == elem;};

    //����������һ��Ԫ�� non-const
    iterator begin() {return elem;};
    //����������һ��Ԫ�� const
    const_iterator cbegin() const {return elem;};
    //�����������һ��Ԫ�غ����λ�� non-const
    iterator end() {return first_free;}; 
    //�����������һ��Ԫ�غ����λ�� const
    const_iterator cend() const {return first_free;};
        
    //�������������һ��Ԫ�� non-const
    reverse_iterator rbegin() { return reverse_iterator(first_free); };
    //�������������һ��Ԫ�� const
    const_reverse_iterator crbegin() const { return const_reverse_iterator(first_free); };
    //���������������һ��Ԫ�غ����λ�� non-const
    reverse_iterator rend() { return reverse_iterator(elem); };
    //���������������һ��Ԫ�غ����λ�� const
    const_reverse_iterator crend() const { return const_reverse_iterator(elem); };
    
    //�����ڴ������
    allocator_type get_allocator() const {return alloc;};

    //��������
    ~Vector() { free();};

private:
    iterator elem;    //��һ��Ԫ��
    iterator first_free;    //���һ��Ԫ��֮���λ��
    iterator cap;    //�����ڴ�֮���λ��

    allocator_type alloc;   //�ڴ������

    //��������ԴԪ�س�ʼ�� 
    template <typename It>
    void init_from_copy(It beg, It end);
    void init_from_fill(size_type n, const_reference tval);
    
    //��������ԴcopyԪ��
    template <typename It>
    void create_from_copy(It beg, It end);
    void create_from_fill(size_type n, const_reference tval);

    //ɾ������Ԫ�أ������ͷſռ�
    void delete_elem();
    //ɾ������Ԫ�أ����ͷſռ�
    void free();
    
    //��ǰ�Ƿ��п����ڴ�
    bool is_full() const {return first_free!=elem&&first_free == cap;};
    //����ռ䲢����Ԫ��
    void extend(size_type n = 0);
    //�����ʷ�Χ
    bool check(size_type pos) const {return pos < this->size();};

friend bool operator< <T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator<=<T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator> <T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator>=<T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator==<T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
friend bool operator!=<T, Allocator>(const Vector<T, Allocator> &lhs,const Vector<T, Allocator> &rhs);
};

//����
template <typename T, typename Allocator>
template <typename It>
void Vector<T, Allocator>::init_from_copy(It beg, It end)
{
    elem = alloc.allocate(end - beg);//����δ��ʼ���Ŀռ�
    first_free = std::uninitialized_copy(beg, end, elem);
    cap = first_free;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::init_from_fill(size_type n, const_reference tval)
{
    elem = alloc.allocate(n);//����δ��ʼ���Ŀռ�
    first_free = std::uninitialized_fill_n(elem, n, tval);
    cap = first_free;
}

//��ֵ
template <typename T, typename Allocator>
template <typename It>
void Vector<T, Allocator>::create_from_copy(It beg, It end)
{
    if (end - beg <= this->capacity())
    {
        //������пռ��������n��Ԫ��
        //�򲻱����·����ڴ�ռ�
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
        //������пռ��������n��Ԫ��
        //�򲻱����·����ڴ�ռ�
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

//ɾ������Ԫ�أ������ͷſռ�
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

//ɾ������Ԫ�أ����ͷſռ�
template <typename T, typename Allocator>
void Vector<T, Allocator>::free()
{
    delete_elem();
    alloc.deallocate(elem, this->capacity());
    elem = nullptr;
    first_free = nullptr;
    cap = nullptr;
}

//����Ԫ�� const
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

//����Ԫ�� non-const
template <typename T, typename Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::operator[](size_type pos)
{
    return const_cast<reference>(
        static_cast<const Vector<T, Allocator>&>(*this)
        [pos]
    );
}

//��ĩβ���Ԫ��
template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const_reference tval)
{
    if (this->is_full())
    {
        extend();
    }
    alloc.construct(first_free++, tval);
}

//��ĩβ���Ԫ�� �ƶ�
template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const value_type&& tval)
{
    if (this->is_full())
    {
        extend();
    }
    alloc.construct(first_free++, std::move(tval));
}

//���뵥��Ԫ��
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

//����n��ָ��Ԫ��
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert
       (iterator it_pos, size_type n, const_reference tval)
{
    if (n == 0)
    {
        return it_pos;
    }

    //����λ��ǰ��Ԫ����
    difference_type pre_pos = it_pos - elem;
    //����λ�ú��Ԫ����
    difference_type behind_pos = first_free - it_pos;

    if (n + this->size() > this->capacity())
    {
        //�ռ䲻��������
        extend();
    }

    it_pos = elem + pre_pos;
    if (n<behind_pos)//������Ԫ�ص�λ�ú��Ԫ����>n
    {
        //����n��Ԫ�أ����죩
        std::uninitialized_copy_n(first_free - n, n, first_free);
        //����behind_pos-n��Ԫ��
        //���ƶ�Ԫ�ذ��Ӻ���ǰ��˳�򣬷�ֹ����
        std::copy_backward(it_pos, first_free - n, first_free);
        //����n��tval
        std::fill_n(it_pos, n, tval);
    }
    else if (behind_pos<n)//������Ԫ�ص�λ�ú��Ԫ����<n
    {
        //����behind_pos��Ԫ��
        std::uninitialized_copy(it_pos, first_free, it_pos + n);
        //�������n-behind_pos��tval
        std::uninitialized_fill_n(first_free, n - behind_pos, tval);
        //����ʣ�µ�tval
        std::fill_n(it_pos, behind_pos, tval);
    }
    else //������Ԫ�ص�λ�ú��Ԫ����==n
    {
        //����n��Ԫ�أ����죩
        std::uninitialized_copy_n(it_pos, n, first_free);
        //����n��tval
        std::fill_n(it_pos, n, tval);
    }
    first_free += n;

    return it_pos;
}

//����ָ����Χ��Ԫ��
template <typename T, typename Allocator>
template <typename It>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert
   (iterator it_pos, It *beg, It *end)
{
    //�������Ԫ����
    auto n = end - beg;
    if (n == 0)
    {
        return it_pos;
    }

    //����λ��ǰ��Ԫ����
    difference_type pre_pos = it_pos - elem;
    //����λ�ú��Ԫ����
    difference_type behind_pos = first_free - it_pos;

    if (n + this->size() > this->capacity())
    {
        //�ռ䲻��������
        extend(n + this->size());
    }

    it_pos = elem + pre_pos;
    if (n<behind_pos)
    {
        //����n��Ԫ�أ����죩
        std::uninitialized_copy_n(first_free - n, n, first_free);
        //����behind_pos-n��Ԫ��
        std::copy_backward(it_pos, first_free - n, first_free);
        //������Ԫ��
        std::copy(beg, end, it_pos);
    }
    else if (n>behind_pos)
    {
        //����behind_pos��Ԫ��
        std::uninitialized_copy(it_pos, first_free, it_pos + n);
        //�������n-behind_pos����Ԫ��
        std::uninitialized_copy(beg + behind_pos, end, first_free);
        //����ʣ�µ���Ԫ��
        std::copy(beg, beg + behind_pos, it_pos);
    }
    else
    {
        //����n��Ԫ�أ����죩
        std::uninitialized_copy_n(it_pos, n, first_free);
        //����n��tval
        std::copy(beg, end, it_pos);
    }
    first_free += n;

    return it_pos;
}

//ɾ��ĩβԪ��
template <typename T, typename Allocator>
void Vector<T, Allocator>::pop_back()
{
    if (!this->empty())
    {
        alloc.destroy(--first_free);
    }
}

//ɾ��ָ����Ԫ��
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase(iterator it_pos)
{
    //����������β���������ֱ�ӷ���
    if (it_pos != first_free)
    {
        std::copy(it_pos + 1, first_free, it_pos);
        alloc.destroy(--first_free);
    }
    return it_pos;
}

//ɾ��ָ����Χ�ڵ�Ԫ��
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

//����ռ䲢����Ԫ��
template <typename T, typename Allocator>
void Vector<T, Allocator>::extend(size_type n)
{
    //δָ��nʱĬ�ϰ���ǰ��������һ��
    n = (n == 0) ? (this->capacity()==0? 1:this->capacity()*2):n;
    
    auto new_elem = alloc.allocate(n);//����δ��ʼ���Ŀռ�
    //�ƶ�Ԫ��
    auto new_first_free = std::uninitialized_copy
        (std::make_move_iterator(elem)
            , std::make_move_iterator(first_free)
            , new_elem);
    free();
    elem = new_elem;
    first_free = new_first_free;
    cap = elem + n;
}

//��������Vecotr��Ԫ��
template <typename T, typename Allocator>
void Vector<T, Allocator>::swap(Vector& rhs)
{
    std::swap(elem, rhs.elem);
    std::swap(first_free, rhs.first_free);
    std::swap(cap, rhs.cap);
    std::swap(alloc, rhs.alloc);
}

//���·���ռ�
template <typename T, typename Allocator>
void Vector<T, Allocator>::reserve(size_type n)
{
    if (n > this->capacity())
    {
        extend(n);
    }
}

//������С
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

//�߼�����
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