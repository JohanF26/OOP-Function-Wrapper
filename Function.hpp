#ifndef FUNCTION_HPP
#define FUNCTION_HPP
#include <stdexcept>

namespace cs540{

template <typename>
class Function;

//redefine BadFunctionCall
class BadFunctionCall : public std::exception{
    //error message
    virtual const char* what() const throw() {
        return "Error: Bad Function Call, a Callable function is not stored.";
    }
};

template <typename ResultType, typename ... ArgumentTypes>
class Function<ResultType(ArgumentTypes...)>{
private:
    class Callable{
    public:
        //purely virtual functions
        virtual ResultType operator()(ArgumentTypes... args) = 0;
        virtual Callable *clone() = 0;
        virtual ~Callable(){};

    };

    template <typename FunctionType>
    class CallableDerived: public Callable{
    private:
        FunctionType fType;
    public:
        //Constructor using function type
        CallableDerived(const FunctionType &func): fType(func){}
        //creates a new object of the given type
        CallableDerived *clone(){
            return new CallableDerived(fType);
        }
        //calling the function passing args as arguments
        ResultType operator()(ArgumentTypes... args){
            return fType(args...);
        }

    };

    Callable* funcCall;

public:
    //Constructs a function wrapper without target funct
    Function(): funcCall(nullptr){}

    //Constructs a funct wrapper with a function of FunctionType as target
    template <typename FunctionType>
    Function(FunctionType func): funcCall(new CallableDerived<FunctionType>(func)){}

    //Copy constructor
    Function(const Function &func){
        //if func.funccall is not nullptr then clone it
        funcCall = (func.funcCall) ? func.funcCall->clone() : nullptr;
    }

    //Copy assignment. must handle self-assignment
    //Must be able to assign between objects that contain
        //free function
        //lambda
        //functor
    //Given they have the same signature
    Function &operator=(const Function &func){
        //self assignment
        if(this == &func) { return *this; }
        //clearing existing function
        if(funcCall != nullptr) { delete funcCall; }
        funcCall = (func.funcCall) ? func.funcCall->clone() : nullptr;
        return *this;

    }

    //Properly destroy the object and clean any memory necessary
    ~Function(){
        //if not nullptr then delete it, otherwise its clean
        if(funcCall != nullptr) { delete funcCall; }
    }

    //Call the wrapped funct target with the given arguments
    //return its result and throw cs540::BadFunctionCall if not stored
    ResultType operator()(ArgumentTypes... args){
        if(funcCall == nullptr) { throw BadFunctionCall(); }
        return ((*funcCall)(args...));
    }

    //Return true if cs540::Function references a callable function
    explicit operator bool() const{
        bool retVal = true;
        if(funcCall != nullptr){
            //ellipsis will catch any exception
            try{
                (*funcCall)();
            } catch (...) {
                retVal = false;
            }
        } else{
            retVal = false;
        }
        return retVal;
    }
};

//returns !f
template <typename ResultType, typename... ArgumentTypes>
bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){
    return !f;
}
template <typename ResultType, typename... ArgumentTypes>
bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f){
    return !f;
}

//returns bool(f)
template <typename ResultType, typename... ArgumentTypes>
bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){
    return bool(f);
}
template <typename ResultType, typename... ArgumentTypes>
bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f){
    return bool(f);
}

}



#endif
