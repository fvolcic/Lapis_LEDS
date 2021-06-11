

template<typename ... T>
struct Tuple {}; 

template<typename First, typename ... Rest>
struct Tuple<First, Rest ...>{
    First first; 
    Tuple<Rest ...> rest; 

    Tuple(const First & first, const Rest & ... rest) :
    first(first), rest(rest...){}

}; 

using tuple_car 

int main(){

};