#include <iostream>
#include <vector>

using namespace std;

class Includer {
public:
  void init(std::string s){ 
    cout << "Initializing " << s << endl;
  };

  void test(std::string s, int n) {
  	cout << "Testing " << s << ": " << n << endl;
  }

  bool install() {
  	return true;
  }
  
  virtual ~Includer(){};
 
  static Includer& instance(){
    static Includer instance;
    return instance;
  }
  
private:
  Includer(){};
  Includer(Includer&) = delete;
  Includer(Includer&&) = delete;
  Includer& operator=(Includer&) = delete;
  Includer operator=(Includer&&) = delete; 
};

class Command {
public:
	virtual std::string name() const = 0;
};

template <typename T> class IncluderCommand;
template <typename R, typename... A>
class IncluderCommand<R(A...)> : public Command {
	std::string name_;
	using IncluderFunc = R(Includer::*)(A...);
	IncluderFunc func_;
	std::string help_;

public:
	IncluderCommand(std::string name, IncluderFunc func, std::string help) {
		name_ = name;
		func_ = func;
		help_ = help;
	}
	
	R exec(A... args) {
		(Includer::instance().*(IncluderFunc)func_)(args...);
	}

	inline std::string name() const override { return name_; }
	inline std::string help() const { return help_; }
};

//template <typename T> using Command = IncluderCommand<T>;

int main(int argc, char** argv){
  	IncluderCommand<void(std::string)> init("init", &Includer::init, "Initialize something...");
	init.exec("Hello");

	IncluderCommand<void(std::string, int)> test("test", &Includer::test, "Test stuff.");
	test.exec("Andreas", 1337);

	IncluderCommand<bool()> install("install", &Includer::install, "Install exactly nothing.");
	cout << install.name() << ": " << install.exec() << endl;

	// Impossible to store IncluderCommand in the same data structure (?)
	vector<Command*> vec;
	vec.push_back(&init);
	cout << vec[0]->name() << endl;
}