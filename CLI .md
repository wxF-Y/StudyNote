>如果是想要快速上手，直接看 安装 和 基础用法 即可

# CLI

​		CLI11是用于 C + + 11及其以外的Command Parser，它提供了具有简单和直观界面的丰富特性集。

### 介绍

​		ClI11是一个强大的Command Parser，拥有非常漂亮，轻便得语法，并且没有超过 C++11得依赖。使用ClI11只需要包含单独的一个头文件，它很容易用于小型项目，但是对于复杂的命令行项目来说足够强大，并且可以为框架定制。它在 Azure 和 GitHub Actions 上进行了测试，最初由 GooFit GPU 配件框架使用。它的灵感来自于 Python 的 plumbum.cli。

模块功能网址

[CLI11: Introduction (cliutils.github.io)](https://cliutils.github.io/CLI11/)

### 为什么要使用ClI

>1、仅仅需要包含一个头文件。
>
>2、语法简单，代码可读性高
>
>3、使用C++11或者更高的C++版本（Should work with GCC 4.8+ (default on CentOS/RHEL 7), Clang 3.4+, AppleClang 7+, NVCC 7.0+, or MSVC 2015+）
>
>4、跨平台性好，支持Windows ，macOS，Linux
>
>5、能够在所有通用平台和编译器通过编译
>
>6、打印信息明确
>
>7、错误处理方式好
>
>8、自然支持标准的 shell 习惯用法，如分组标志、位置分隔符等
>
>9、易于执行，具有帮助，解析错误等提供正确的退出和细节
>
>10、可用的子命令语法，支持多个子命令、嵌套子命令、选项组和可选的备份(稍后解释)。
>
>11、能够添加配置文件(TOML、 INI 或自定义格式) ，并生成它。
>
>12、为 HPC 应用程序生成可以直接在代码中使用的实际值，而不是需要花费计算时间才能查找的东西。
>
>13、使用常见类型、简单的自定义类型和可扩展到特殊类型的类型。
>
>14、开源

​	See [Awesome C++](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Ffffaraz%2Fawesome-cpp%2Fblob%2Fmaster%2FREADME.md%23cli) for a less-biased list of parsers. You can also find other single file libraries at [Single file libs](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fnothings%2Fsingle_file_libs%2Fblob%2Fmaster%2FREADME.md).

### 缺点

>1、语法上的非标准变化，比如-long 选项。这是非标准的，应该避免，因此这个库强制执行。
>
>2、部分选项的完成，例如 Python 的 argparse 提供了不完整参数。最好不要猜。大多数第三方 Python 命令行解析器实际上是重新实现命令行解析，而不是使用 argparse，因为这种设计缺陷(最近的版本确实有禁用它的选项)。
>
>3、自动完成: 这可能最终会被添加到 Plumums 和 CLI11中，但是目前还不支持。
>
>4、宽字符串/unicode: 因为它只使用标准库，所以可能很难正确实现

### 安装

>1、将 CLI11.hpp 从最新版本复制到 include 目录，就可以使用了。这包括整个命令解析器库，但不包括单独的实用功能(如 Timer、 AutoTimer)。这些实用功能可以单独复制。也可以直接包含CLI目录
>
>2、在链接时使用 add _ subdirectory 和 CLI11: : CLI11接口目标。如果不使用子模块，则必须确保复制的文件与当前项目位于同一个树目录中，以防止 CMake 和 add _ subdirectory 出错。
>
>3、全局标头: 使用 CLI/* 。存储在共享文件夹中的 hpp 文件。您可以将 git 存储库签出到系统范围的文件夹，例如/opt/。使用 CMake，您可以通过以下方式添加 include 路径:
>
>```c++
>if(NOT DEFINED CLI11_DIR)
>set (CLI11_DIR "/opt/CLI11" CACHE STRING "CLI11 git repository")
>endif()
>include_directories(${CLI11_DIR}/include)
>```
>
>添加以下头文件在在Source Code
>
>```c++
>#include "CLI/App.hpp"
>#include "CLI/Formatter.hpp"
>#include "CLI/Config.hpp"
>```
>
>- 通过 CMake 3.14 + 中的 FetchContent (或者3.11 + ，需要更多工作) : 你可以通过提取内容来添加它，然后像上面一样使用 CLI11: : CLI11目标，CMake 会在配置阶段下载项目:
>
>```C++
>include(FetchContent)
>FetchContent_Declare(
>  cli11
>  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
>  GIT_TAG        v2.2.0
>)
>
>FetchContent_MakeAvailable(cli11)
>```

以上是复制的官网说明，我们代码引入方法：

**方法一**

- 下载CLI11(https://github.com/CLIUtils/CLI11.git)，包含目录添加CLI11的  include 文件夹

- >#include <CLI/CLI.hpp>

**方法二**

- 下载CLI11的Release版本的ClI.hpp文件，直接引入项目

- >#include "CLI.hpp"

### 基础用法

添加一些命令行参数

Example Code

```c++
#include <CLI.hpp>
#include <iostream>
int main(int argc, char** argv) {
    CLI::App app{"App description"}; // 初始化
    
    std::string filename = "default"; // 默认参数，可以为空
    app.add_option("-f,--file", filename, "A help string") // 添加命令行参数
         ->required()  // 必要
  		->check(CLI::ExistingFile); // 必须是已经存在的文件

    CLI11_PARSE(app, argc, argv);  // 解析
    std::cout << "filename : " << filename << std::endl;
    return 0;
}
```

命令行测试

```shell
>test.exe -f "C:\data\a.txt"
filename : C:\data\a.txt

>test.exe -h
App description
Usage: ConsoleApplication2.exe [OPTIONS]
Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              A help string
```

命令行参数分为Flags 和 Options ， Flags和Options的区别在于，后者需要传入相应类型的参数值，前者不需要。

#### Flags 

**Boolean flags**

最简单的方式就是使用boolean flag

```cpp
bool my_flag{false};
app.add_flag("-f", my_flag, "Optional description");
```

如果我们的命令行存在 -f 参数，my_flag的值为 true 否则 my_flag的值为默认值 false 

如果以上这样定义，-f 参数可以出现多次，如果我们只想 -f 参数只出现一次，可以使用 ->take_last() 

```cpp
bool my_flag{ false };
app.add_flag("-f", my_flag, "Optional description")
		->take_last();
```

这样定义，如果命令行出现 -f -f  或者 -ff 就会抛出 ParseError 并且提供相应描述

**Integer flags**

如果想要允许多个值，可以使用Interger flags

```cpp
int my_flag{0};
app.add_flag("-f", my_flag, "Optional description");
```

```shell
test.exe -f -f -f
filename : 3
```

**flags支持类型**

ClI11支持的类型包括

>bool, int, float, vector, enum, or string-like

**Flags的默认值**

**boolean flags**

```cpp
app.add_flag("--flag,!--no-flag",result,"help for flag");
```

>如果是命令行传入 --flag ，则 result为 1 ，不传入则为 0
>
>如果命令行传入 --no-flag ，则 result 为 -1 ，不传入则为 0

另外一种写法

```cpp
app.add_flag("--flag,!--no-flag{false}",result,"help for flag");
```

{} 中即为默认值

**Integer flags**

```cpp
app.add_flag("-1{1},-2{2},-3{3}",result,"numerical flag")
```

```shell
>test.exe -1
result : 1

>test.exe -2
result : 2

>test.exe -3
result : 3
```

同 boolean flags

**add_flags返回值**

返回值为一个CLI::Option的指针类型。可以捕获并使用它，并且绑定一些校验

```cpp
CLI::Option* my_flag = app.add_flag("-f", "Optional description");
```

解析后可以使用 my_flag->count() 去获得这些数字找到的次数，也可以使用 *my_flag去获得这个boolean值

**回调flags**

C++14 以上可以添加回调函数，在传入参数时进行调用，具体使用方法如下：

```
auto callback = [](int count){std::cout << "This was called " << count << " times";};
app.add_flag_function("-c", callback, "Optional description");
```

#### Option

和Flag特性大多一样，无非就是需要传入参数值

```cpp
int int_option{0};
app.add_option("-i", int_option, "Optional description");
```

**支持的类型**

| Type           | CLI11                                                        |
| -------------- | ------------------------------------------------------------ |
| number like    | Integers, floats, bools, or any type that can be constructed from an integer or floating point number |
| string-like    | std\::string, or anything that can be constructed from or assigned a std\::string |
| char           | For a single char, single string values are accepted, otherwise longer strings are treated as integral values and a conversion is attempted |
| complex-number | std::complex or any type which has a real(), and imag() operations available, will allow 1 or 2 string definitions like "1+2j" or two arguments "1","2" |
| enumeration    | any enum or enum class type is supported through conversion from the underlying type(typically int, though it can be specified otherwise) |
| container-like | a container(like vector) of any available types including other containers |
| wrapper        | any other object with a `value_type` static definition where the type specified by `value_type` is one of type in this list |
| tuple          | a tuple, pair, or array, or other type with a tuple size and tuple_type operations defined and the members being a type contained in this list |
| function       | A function that takes an array of strings and returns a string that describes the conversion failure or empty for success. May be the empty function. (`{}`) |
| streamable     | any other type with a `<<` operator will also work           |

使用vector或者其他Container将会存在两种传参方法

| Separate names    | Combined names |
| ----------------- | -------------- |
| `--vec 1 --vec 2` | `--vec 1 2`    |

也可以使用  ->expected(N) 去指定传入参数的数量。

```cpp
std::vector<int> int_vec;
app.add_option("--vec", int_vec, "My vector option")
    ->expected(10); // int_vec 只能传10给以内的参数值
```

用于 CLI11 的容器定义是一个具有 end ()、 insert (...)、 clear ()和 value _ type 定义的类型。这包括 vector、 set、 deque、 list、 forward _ iist、 map、 unordered _ map 以及来自标准库的其他一些内容，还有来自 boost 库的许多其他容器。

插入值，如果需要分割，可以使用符号 %%

```shell
cmd --vec_of_vec 1 2 3 4 %% 1 2
```

**Option modifiers**

使用 add_options 可以添加的一些语法检查和参数过滤等

| Modifier                                                | Description                                                  |
| ------------------------------------------------------- | ------------------------------------------------------------ |
| `->required()`                                          | The program will quit if this option is not present. This is `mandatory` in Plumbum, but required options seems to be a more standard term. For compatibility, `->mandatory()` also works. |
| `->expected(N)`                                         | Take `N` values instead of as many as possible, mainly for vector args. |
| `->expected(Nmin,Nmax)`                                 | Take between `Nmin` and `Nmax` values.                       |
| `->type_size(N)`                                        | specify that each block of values would consist of N elements |
| `->type_size(Nmin,Nmax)`                                | specify that each block of values would consist of between Nmin and Nmax elements |
| `->needs(opt)`                                          | This option requires another option to also be present, opt is an `Option` pointer. |
| `->excludes(opt)`                                       | This option cannot be given with `opt` present, opt is an `Option` pointer. |
| `->envname(name)`                                       | Gets the value from the environment if present and not passed on the command line. |
| `->group(name)`                                         | The help group to put the option in. No effect for positional options. Defaults to `"Options"`. `"Hidden"` will not show up in the help print. |
| `->description(string)`                                 | Set/change the description                                   |
| `->ignore_case()`                                       | Ignore the case on the command line (also works on subcommands, does not affect arguments). |
| `->ignore_underscore()`                                 | Ignore any underscores on the command line (also works on subcommands, does not affect arguments). |
| `->allow_extra_args()`                                  | Allow extra argument values to be included when an option is passed. Enabled by default for vector options. |
| `->disable_flag_override()`                             | specify that flag options cannot be overridden on the command line use `=<newval>` |
| `->delimiter('<CH>')`                                   | specify a character that can be used to separate elements in a command line argument, default is , common values are ',', and ';' |
| `->multi_option_policy( CLI::MultiOptionPolicy::Throw)` | Sets the policy for handling multiple arguments if the option was received on the command line several times. `Throw`ing an error is the default, but `TakeLast`, `TakeFirst`, `TakeAll`, and `Join` are also available. See the next four lines for shortcuts to set this more easily. |
| `->take_last()`                                         | Only use the last option if passed several times. This is always true by default for bool options, regardless of the app default, but can be set to false explicitly with `->multi_option_policy()`. |
| `->take_first()`                                        | sets `->multi_option_policy(CLI::MultiOptionPolicy::TakeFirst)` |
| `->take_all()`                                          | sets `->multi_option_policy(CLI::MultiOptionPolicy::TakeAll)` |
| `->join()`                                              | sets `->multi_option_policy(CLI::MultiOptionPolicy::Join)`, which uses newlines or the specified delimiter to join all arguments into a single string output. |
| `->join(delim)`                                         | sets `->multi_option_policy(CLI::MultiOptionPolicy::Join)`, which uses `delim` to join all arguments into a single string output. this also sets the delimiter |
| `->check(Validator)`                                    | perform a check on the returned results to verify they meet some criteria. See [Validators](https://cliutils.github.io/CLI11/book/chapters/validators.html) for more info |
| `->transform(Validator)`                                | Run a transforming validator on each value passed. See [Validators](https://cliutils.github.io/CLI11/book/chapters/validators.html) for more info |
| `->each(void(std::string))`                             | Run a function on each parsed value, *in order*.             |
| `->default_str(string)`                                 | set a default string for use in the help and as a default value if no arguments are passed and a value is requested |
| `->default_function(string())`                          | Advanced: Change the function that `capture_default_str()` uses. |
| `->default_val(value)`                                  | Generate the default string from a value and validate that the value is also valid. For options that assign directly to a value type the value in that type is also updated. Value must be convertible to a string(one of known types or have a stream operator). |

**默认值**

- `group`: The group name starts as "Options"
- `required`: If the option must be given. Defaults to `false`. Is ignored for flags.
- `multi_option_policy`: What to do if several copies of an option are passed and one value is expected. Defaults to `CLI::MultiOptionPolicy::Throw`. This is also used for bool flags, but they always are created with the value `CLI::MultiOptionPolicy::TakeLast` regardless of the default, so that multiple bool flags does not cause an error. But you can override that flag by flag.
- `ignore_case`: Allow any mixture of cases for the option or flag name
- `ignore_underscore`: Allow any number of underscores in the option or flag name
- `configurable`: Specify whether an option can be configured through a config file
- `disable_flag_override`: do not allow flag values to be overridden on the command line
- `always_capture_default`: specify that the default values should be automatically captured.
- `delimiter`: A delimiter to use for capturing multiple values in a single command line string (e.g. --flag="flag,-flag2,flag3")

如果是基础使用，以上以及可以完成，后续高级功能学习网址如下

[Introduction · CLI11 Tutorial (cliutils.github.io)](https://cliutils.github.io/CLI11/book/)
