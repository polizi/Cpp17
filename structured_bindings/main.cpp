#include <iostream>
#include <utility>


class Customer{
    //注意这里，成员变量是私有的，直接结构化绑定是编译不过的。
    //我们通过提供tuple 的API来间接绑定Customer。
private:
    std::string first_;
    std::string last_;
    long value_;
public:
    Customer(std::string first, std::string last, long value)
        : first_(std::move(first)), last_(std::move(last)),
          value_(value){
    }

    const std::string& FirstName() const{
        return first_;
    }
    std::string& FirstName(){
        return first_;
    }

    const std::string& LastName() const{
        return last_;
    }
    std::string& LastName(){
        return last_;
    }

    long Value() const {
        return value_;
    }

    long& Value(){
        return value_;
    }

};

template <>
struct std::tuple_size<Customer>{
    static constexpr int value = 3;//有3个成员
};

template <>
struct std::tuple_element<2, Customer>{
    using type = long;//最后一个是long
};

template <std::size_t Idx>
struct std::tuple_element<Idx, Customer>{
    using type = std::string;//其余都是string
};

template <std::size_t I>
decltype(auto) get(Customer& c){
    std::cout << "Customer&" << std::endl;
    static_assert(I < 3);
    if constexpr (I == 0){
        return c.FirstName();
    }
    else if constexpr (I == 1){
        return c.LastName();
    }
    else{
        return c.Value();
    }
}

template <std::size_t I>
decltype(auto) get(const Customer& c){
    std::cout << "const Customer&" << std::endl;
    static_assert(I < 3);
    if constexpr (I == 0){
        return c.FirstName();
    }
    else if constexpr (I == 1){
        return c.LastName();
    }
    else{
        return c.Value();
    }
}

template <std::size_t I>
decltype(auto) get(Customer&& c){
    std::cout << "Customer&&" << std::endl;
    static_assert(I < 3);
    if constexpr (I == 0){
        return std::move(c.FirstName());
    }
    else if constexpr (I == 1){
        return std::move(c.LastName());
    }
    else{
        return std::move(c.Value());
    }
}

int main()
{
    Customer c("Tim", "Starr", 42);
    std::cout << "Customer size : " << std::tuple_size<Customer>::value << std::endl;
    std::tuple_element<1, Customer>::type;

    //右值，只读。
    auto [f, l, v] = c;
    f = "xiaoming";
    l = "Waters";
    v += 10;

    std::cout << "f : " << f << " l : " << l << " v : " << v << std::endl;
    std::cout << " c.First : " << c.FirstName()
              << " c.Last : " << c.LastName()
              << " c.value : " << c.Value() << std::endl;

    //通过引用修改值
    auto& [f2, l2, v2] = c;
    f2 = "xiaoming";
    l2 = "Waters";
    v2 += 10;
    std::cout << "f2 : " << f2 << " l2 : " << l2 << " v2 : " << v2 << std::endl;
    std::cout << " c.First : " << c.FirstName()
              << " c.Last : " << c.LastName()
              << " c.value : " << c.Value() << std::endl;
}