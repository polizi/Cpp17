#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>
#include <map>
#include <experimental/filesystem>

std::mutex some_mutex;
std::vector<int> some_struct;
void IfWithInitialization(){
    //初始化作用域开始
    if(int i = 10; true){
        std::cout << "true " << i << std::endl;
    }
    else{
        std::cout << "false " << i << std::endl;
    }
    //初始化作用域结束

    if(int i = 10; false){
        std::cout << "true " << i << std::endl;
    }
    else{
        std::cout << "false " << i << std::endl;
    }
}

void ClassTemplateArgDeduction(){
    std::mutex mutex;

    {
        //C++17 之前
        std::lock_guard<std::mutex> lock_guard(mutex);
    }

    {
        //c++17
        std::lock_guard lock_guard1(mutex);
    }


    if(std::lock_guard lg(some_mutex); !some_struct.empty()){
        std::cout << some_struct.front() << std::endl;
    }

    //上面这段代码等同于下面这段代码
    {
        std::lock_guard<std::mutex> lock_guard(some_mutex);
        if(!some_struct.empty()){
            std::cout << some_struct.front() << std::endl;
        }
    }


    //注意。在初始化时一定要给一个名字，尽管你不会用到，但是如果不给名字,
    //则其作用域仅仅维持到初始化结束。
    if(std::lock_guard<std::mutex>{some_mutex};//锁的作用域开始-结束
        !some_struct.empty()){                          //没有被锁住
        std::cout << some_struct.front() << std::endl;  //没有被锁住
    }


    //一种有人喜欢有人讨厌的写法，出事化的名字为"_"。
    if(std::lock_guard<std::mutex> _{some_mutex};
        !some_struct.empty()){
        std::cout << some_struct.front() << std::endl;
    }

}

void MapControl(){
    std::map<std::string, int> map;

    {
        //C++17之前
        auto ret = map.insert({"new", 42});
        if(!ret.second){
            const auto& elem = *(ret.first);
            std::cout << "already there : " << elem.first << std::endl;
        }
    }

    {
        //C++17
        if(const auto& [pos, ok] = map.insert({"new", 42}); !ok){
            const auto& [key, value] = *pos;
            std::cout << "already there : " << key << std::endl;
        }
    }
}

void SwitchWithInitialization(){
    using namespace std::experimental::filesystem;
//    using namespace std::filesystem;
    const std::string path_name = "../../if_switch_with_initialization";

    switch (path p(path_name); status(p).type()) {
        case file_type::not_found :
            std::cout << p << " not found" << std::endl;
            break;
        case file_type::directory:
            std::cout << p << " :\n";
            for(auto& e : directory_iterator(p)){
                std::cout << "-" << e.path() << std::endl;
            }
            break;

        default:
            std::cout << p << " exists\n";
            break;
    }

}
int main() {

    //if 初始化
    IfWithInitialization();

    //C++17模板参数类型推断与 if初始化的一些规则。
    ClassTemplateArgDeduction();

    //使用if 初始化来处理map插入的返回值处理。
    MapControl();

    //使用switch 初始化
    SwitchWithInitialization();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}