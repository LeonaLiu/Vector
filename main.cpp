/*
 * c++练习-3
 * 模板 vector
 * 练习c++的模板编程
 * created by Yang Liu
 * 2016.02.17
 * version: 1.0
 */
#include <iostream>
#include <iterator>
#include <string>
#include "Vector.h"

template <typename T>
void print(Vector<T>& vec, const char* str);

void test_int();
void test_str();

int main()
{
    std::cout << "----- This program test Vector.h -----" << std::endl;
    std::cout << "Int Test: " << std::endl;
    test_int();
    std::cout << std::endl << "String Test: " << std::endl;
    test_str();
  
    return 0;
}


template <typename T>
void print(Vector<T>& vec, const char* str)
{    
    std::cout << str << ": ";
    std::ostream_iterator<T> out_it(std::cout," ");
    std::copy(vec.cbegin(), vec.cend(),out_it);
    std::cout << std::endl;
}

void test_int()
{
    //构造
    std::cout << "1.Initialize:" << std::endl;
    Vector<int> vec1;
    print(vec1, "vec1");
    Vector<int> vec2{ 1,2,3,4,5,6 };
    print(vec2, "vec2");
    Vector<int> vec3(10, 7);
    print(vec3, "vec3");
    Vector<int> vec4(vec2.cbegin() + 1, vec2.cend() - 1);
    print(vec4, "vec4");
    Vector<int> vec5(vec2);
    print(vec5, "vec5");

    //赋值
    std::cout << std::endl << "2.Assign:" << std::endl;
    vec1 = vec2;
    print(vec1, "vec1 = vec2");
    vec1 = { 9,8,7,6,5,4,3,2,1 };
    print(vec1, "vec1 = {9,8,7,6,5,4,3,2,1}");
    vec3.assign(9, 8);
    print(vec3, "vec3.assign(9, 8)");
    vec3.assign(10, 9);
    print(vec3, "vec3.assign(10, 9)");
    vec3.assign(12, 10);
    print(vec3, "vec3.assign(12, 10)");
    vec3.assign(vec2.cbegin(), vec2.cend());
    print(vec3, "vec3.assign(vec2.cbegin(), vec2.cend())");

    //容量
    std::cout << std::endl << "3.Capacity:" << std::endl;
    print(vec3, "vec3");
    std::cout << "vec3.capacity() = " << vec3.capacity() << std::endl;
    std::cout << "vec3.max_size() = " << vec3.max_size() << std::endl;
    std::cout << "vec3.size() = " << vec3.size() << std::endl;
    std::cout << "vec3.empty() = " << (vec3.empty()?"True":"False") << std::endl;

    //添加元素
    std::cout << std::endl << "4.Add Element:" << std::endl;
    vec3.push_back(-1);
    print(vec3, "vec3.push_back(-1)");
    vec3.insert(vec3.begin()+1,-2);
    print(vec3, "vec3.insert(vec3.begin()+1,-2)");
    vec3.insert(vec3.end()-2,2,-3);
    print(vec3, "vec3.insert(vec3.end()-2,2,-3)");
    print(vec1, "vec1");
    vec3.insert(vec3.end()-2,vec1.begin(),vec1.begin()+5);
    print(vec3, "vec3.insert(vec3.end()-2,vec1.begin(),vec1.begin()+5)");
    
    //访问
    std::cout << std::endl << "5.Access:" << std::endl;
    print(vec3, "vec3");
    std::cout << "vec3.front() = " << vec3.front() << std::endl;
    std::cout << "vec3.back() = " << vec3.back() << std::endl;
    std::cout << "vec3[7] = " << vec3[8] << std::endl;
    std::cout << "vec3.at(8) = " << vec3.at(8) << std::endl;

    //删除
    std::cout << std::endl << "6.Delete Element:" << std::endl;
    print(vec2, "vec2");
    vec2.pop_back();
    std::cout << "After pop_back():";
    print(vec2, "vec2");
    vec2.erase(vec2.begin());
    print(vec2, "vec2.erase(vec2.begin())");
    vec2.erase(vec2.begin(), vec2.begin() + 3);
    print(vec2, "vec2.erase(vec2.begin(), vec2.begin() + 3)");
    vec2.clear();
    print(vec2, "vec2.clear()");

    //交换
    std::cout << std::endl << "7.Swap:" << std::endl;
    print(vec1, "vec1");
    print(vec3, "vec3");
    vec1.swap(vec3);
    std::cout << "--- swap vec1 & vec3 ---" << std::endl;
    print(vec1, "vec1");
    print(vec3, "vec3");

    //调整大小
    std::cout << std::endl << "8.Change size:" << std::endl;
    std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
    std::cout << "vec1.size() = " << vec1.size() << std::endl;
    vec1.reserve(30);
    std::cout << "After vec1.reserve(30): " << std::endl;
    print(vec1, "vec1");
    std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
    std::cout << "vec1.size() = " << vec1.size() << std::endl;
    vec1.resize(8);
    std::cout << "After vec1.resize(8): " << std::endl;
    print(vec1, "vec1");
    std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
    std::cout << "vec1.size() = " << vec1.size() << std::endl;
    
    //逆序打印
    std::cout << std::endl << "8.Backward:" << std::endl;
    print(vec3, "vec3");
    std::cout << "Backward sequence: ";
    //std::ostream_iterator<int> out_it(std::cout, " ");
    //std::copy(vec3.crbegin(), vec3.crend(), out_it); 报错，不能实例化copy模板
    for (auto it = vec3.crbegin();it != vec3.crend();++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;


    //逻辑运算
    std::cout << std::endl << "9.Logical Calculus:" << std::endl;
    vec2 = { 1,2,3,4,5,6 };
    print(vec2, "vec2");
    vec3.assign(10, 7);
    print(vec3, "vec3");
    vec5 = vec2;
    print(vec5, "vec5");
    std::cout << "vec2 " << ((vec2 <= vec3) ? "<=" : ">") << " vec3" << std::endl;
    std::cout << "vec2 " << ((vec2 > vec3) ? ">" : "<=") << " vec3" << std::endl;
    std::cout << "vec2 " << ((vec2 == vec3) ? "==" : "!=") << " vec3" << std::endl;
    std::cout << "vec2 " << ((vec2 == vec5) ? "==" : "!=") << " vec5" << std::endl;

    return ;
}

void test_str()
{
    //构造
    std::cout << "1.Initialize:" << std::endl;
    Vector<std::string> vec1;
    print(vec1, "vec1");
    Vector<std::string> vec2{ "a","b","c","d","e","f" };
    print(vec2, "vec2");
    Vector<std::string> vec3(10, "x");
    print(vec3, "vec3");
    Vector<std::string> vec4(vec2.cbegin() + 1, vec2.cend() - 1);
    print(vec4, "vec4");
    Vector<std::string> vec5(vec2);
    print(vec5, "vec5");

    //赋值
    std::cout << std::endl << "2.Assign:" << std::endl;
    vec1 = vec2;
    print(vec1, "vec1 = vec2");
    vec1 = {"f","e","d","c","b","a"};
    print(vec1, "vec1 = {f,e,d,c,b,a}");
    vec3.assign(9, "y");
    print(vec3, "vec3.assign(9, y)");
    vec3.assign(10, "z");
    print(vec3, "vec3.assign(10, z)");
    vec3.assign(12, "x");
    print(vec3, "vec3.assign(12, x)");
    vec3.assign(vec2.cbegin(), vec2.cend());
    print(vec3, "vec3.assign(vec2.begin(), vec2.end())");

    //容量
    std::cout << std::endl << "3.Capacity:" << std::endl;
    print(vec3, "vec3");
    std::cout << "vec3.capacity() = " << vec3.capacity() << std::endl;
    std::cout << "vec3.max_size() = " << vec3.max_size() << std::endl;
    std::cout << "vec3.size() = " << vec3.size() << std::endl;
    std::cout << "vec3.empty() = " << (vec3.empty() ? "True" : "False") << std::endl;

    //添加元素
    std::cout << std::endl << "4.Add Element:" << std::endl;
    vec3.push_back("x");
    print(vec3, "vec3.push_back(x)");
    vec3.insert(vec3.begin() + 1, "y");
    print(vec3, "vec3.insert(vec3.begin()+1,y)");
    vec3.insert(vec3.end() - 2, 2, "z");
    print(vec3, "vec3.insert(vec3.end()-2,2,z)");
    print(vec1, "vec1");
    vec3.insert(vec3.end() - 2, vec1.begin(), vec1.begin() + 5);
    print(vec3, "vec3.insert(vec3.end()-2,vec1.begin(),vec1.begin()+5)");
    
    //访问
    std::cout << std::endl << "5.Access:" << std::endl;
    print(vec3, "vec3");
    std::cout << "vec3.front() = " << vec3.front() << std::endl;
    std::cout << "vec3.back() = " << vec3.back() << std::endl;
    std::cout << "vec3[8] = " << vec3[8] << std::endl;
    std::cout << "vec3.at(8) = " << vec3.at(8) << std::endl;
    //std::cout << "vec3[-1] = " << vec3[-1] << std::endl; //此条将触发out of range异常

    //删除
    std::cout << std::endl << "6.Access:" << std::endl;
    print(vec2, "vec2");
    vec2.pop_back();
    std::cout << "After pop_back():";
    print(vec2, "vec2");
    vec2.erase(vec2.begin());
    print(vec2, "vec2.erase(vec2.begin())");
    vec2.erase(vec2.begin(), vec2.begin() + 3);
    print(vec2, "vec2.erase(vec2.begin(), vec2.begin() + 3)");
    vec2.clear();
    print(vec2, "vec2.clear()");

    //交换
    std::cout << std::endl << "7.Swap:" << std::endl;
    print(vec1, "vec1");
    print(vec3, "vec3");
    vec1.swap(vec3);
    std::cout << "--- swap vec1 & vec3 ---" << std::endl;
    print(vec1, "vec1");
    print(vec3, "vec3");

    //调整大小
    std::cout << std::endl << "8.Change size:" << std::endl;
    std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
    std::cout << "vec1.size() = " << vec1.size() << std::endl;
    vec1.reserve(30);
    std::cout << "After vec1.reserve(30): " << std::endl;
    print(vec1, "vec1");
    std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
    std::cout << "vec1.size() = " << vec1.size() << std::endl;
    vec1.resize(8);
    std::cout << "After vec1.resize(8): " << std::endl;
    print(vec1, "vec1");
    std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
    std::cout << "vec1.size() = " << vec1.size() << std::endl;
    
    //逆序打印
    std::cout << std::endl << "8.Backward:" << std::endl;
    print(vec3, "vec3");
    std::cout << "Backward sequence: ";
    for (auto it = vec3.crbegin();it != vec3.crend();++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    //逻辑运算
    std::cout << std::endl << "9.Logical Calculus:" << std::endl;
    vec2 = { "z","b","c","d","e","f" };
    print(vec2, "vec2");
    vec3.assign(10, "x");
    print(vec3, "vec3");
    vec5 = vec2;
    print(vec5, "vec5");
    std::cout << "vec2 " << ((vec2 <= vec3) ? "<=" : ">") << " vec3" << std::endl;
    std::cout << "vec2 " << ((vec2 > vec3) ? ">" : "<=") << " vec3" << std::endl;
    std::cout << "vec2 " << ((vec2 == vec3) ? "==" : "!=") << " vec3" << std::endl;
    std::cout << "vec2 " << ((vec2 == vec5) ? "==" : "!=") << " vec5" << std::endl;

    return ;
}
