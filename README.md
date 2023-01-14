# DraftCodes

draft codes for my study of programming.

## Bugs or Questions

If some code have bugs or you don't know why I use this way to finish my code,
you can use issue for asking.

## Code Conducts

In my code, I will follow some code conducts,
all the conducts are list at below.

### C programming

#### Naming

I will use so called _camel case_ to naming my variables or types.
For macros (if have), I will use all upper case with some underlines.
And enums, I will use all upper case.

```c
// macro example
#define __MACRO_EX__ 1

// type example
typedef struct TypeEx {
    int contEx;
} TypeEx;

// enum example
enum YesOrNo {
    YES, NO,
};

// variable example
static int valEx;
```

#### Docing

I will do some doc on my codes. The examples are listed here.

The document styles are used only by myself,
maybe someone think it's bad, whatever.

```c
/// @file: exampleDir/example.c

// -- GLOBAL

/// @glob: ExampleGlobalVal
/// @descript: the example global value
extern size_t ExampleGlobalVal = 16;

// -- MACRO

/// @macro: __MACRO_EX__
/// @descript: an example of macro value
#define __MACRO_EX__ 1

/// @macro: __MACRO_F_EX__
/// @descript: an example of macro function
///   * if x is int, call `f`
///   * otherwise, return x itself
#define __MACRO_F_EX__(x) \
  _Generic((x), \
    int: f(x), \
    default: x \
  )

// -- TYPE

/// @type: TypeEx
/// > type for example
/// @content: {contEx} the example content [ int ]
typedef struct TypeEx {
    int contEx;
} TypeEx;

/// @enum: YesOrNo
/// > enum fo yes or no
/// @item: {YES} the yes
/// @item: {NO} the no
enum YesOrNo {
    YES, NO,
};

// -- FUNC

/// @func: exFunc
/// >> example function
/// @param: {paramEx} example parameter [ int ]
/// @return: the example return [ float ]
/// @descript: maybe here
///   * also can
///   * be here
extern float exFunc(int paramEx);
```

That's all document styles I use.

#### Build System

I will use bash script the manage the build process,
and I will use CMake, which is very convenient.

```bash
./configure.sh [arch] [type]
# arch: linux or win, win use mingw
# type: debug or release, use D or R

./build.sh
# or clean the build
./build.sh clean
```

### Rust programming

Rust document style is same as C but with some markdown syntax.

And Rust has _Cargo_, which is really awesome.

> Naming of Rust, I will follow the Rust conduct :(
