# KSL

KSL is a new programming language designed by myself.

## Basic Grammar

The formal define of KSL (aka. EBNF) is not finished.
So I will use natural language to express it.

### Primitive Elements

KSL, for some aspects, is a lisp-like language.
But I want to have some feature in _Wolfram_.

#### Int

A number without a dot (.) is default an integer.

```ksl
In[1]:= 10
=> Int

Out[1]:= 10
```

#### Real

Floating-point number, or real number, is a number has dot.

```ksl
In[1]:= 10.0
=> Real

Out[1]:= 10.0
```

In KSL, you **can not** emit the zero.
That means `.1` or `1.` is illegal.

#### Complex

KSL will support complex number primitively.

```ksl
In[1]:= Complex[1, 2]
=> Complex

Out[1]:= 1 + 2I
```

`1 + 2I` is the show format,
you can only use `Complex` function (? or Type)
to get a complex number.

All parts of a complex number is `Real`.

> we can use Type number try
> to parse another type to the specific type

#### Arithmetic

`+`, `-`, `*` and `/` can use between Int, Real and Complex.
There will have a type convert if the types of operands are not matched.

```ksl
In[1]:= 1 / 2
=> Quotation

Out[1]:= 1/2

In[2]:= 1.0 / 2
=> Real

Out[1]:= 0.5

In[3]:= 1.3 + Complex[2, 3]
=> Complex

Out[3]:= 3.3 + 3I
```

#### Symbol

Symbols start with `#`, like `#x`.

```ksl
In[1]:= #x
=> Symbol

Out[1]:= x
```

In the show format, `#` will be emitted.

#### String

KSL does not have Character Type, but have String

```ksl
In[1]:= "text"
=> String

Out[1]:= "text"
```

In the show format, `#` will be emitted.

#### Pattern

This is inspired by Wolfram.

A pattern is a way to match something, especially types.

```ksl
In[1]:= a_Int
=> <Pattern>
```

The example shows a pattern of a Integer.

##### Rule

We can use pattern to build a rule.

```ksl
In[1]:= a_Int -> Complex[Real[a], Real[a]]
=> <Rule>
```

##### Apply

We can apply to rule by `Apply` function.

```ksl
In[1]:= Apply[a_Int -> Complex[Real[a], Real[a]], 10]
=> Complex

Out[1]:= 1 + 1I
```

#### List

List is the basic container in KSL.

The relative function is `Cons`, `Fst`, `Snd` and `End`.

```ksl
In[1]:= {1, 2, 3}
=> List[Int]

Out[1]:= {1, 2, 3}

In[2]:= Cons[3, {1, 2, 3}]
=> List[Int]

Out[2]:= {3, 1, 2, 3}

In[3]:= Fst[{1, 2, 3}]
=> Optional[Int]

Out[3]:= Just[1]

In[4]:= Snd[{1, 2, 3}]
=> Optional[Int]

Out[4]:= Just[1]

In[5]:= Snd[{1}]
=> Optional[Int]

Out[5]:= None

In[6]:= End[{1, 2, 3}]
=> Optional[Int]

Out[6]:= Just[3]
```

`Take` and `Drop` will give a List as the result.

```ksl
In[1]:= Take[2, {1, 2, 3}]
=> List[Int]

Out[1]:= {1, 2}

In[2]:= Drop[2, {1, 2, 3}]
=> List[Int]

Out[2]:= {3}

In[3]:= Drop[4, {1, 2, 3}]
=> List[Int]

Out[3]:= {}
```

#### Function

KSL can use `Fun` to get a function.

```ksl
In[1]:= Fun[{_Int, y_Complex}, y]
=> <Function>

In[2]:= Type[Fun[{_Int, y_Complex}, y]]
=> String

Out[2]:= "Int * Complex -> Complex"
```

We can use the function immediately.

```ksl
In[1]:= Fun[{_Int, y_Complex}, y][10, Complex[2, 2]]
=> Complex

Out[1]:= 2 + 2I
```

#### Binding

KSL support binding value inside a block with `Let` function.

```ksl
In[1]:= Let[{
>   a_Int -> 10,
>   b_Int -> 20,
>   f_Function -> Fun[{x_Int}, x + 3]
> }, f[a] + b]
=> Int

Out[1]:= 33
```

If want a variable like bind, means use global environment bind, you can use `Val`

```ksl
Int[1]:= Val[x, 10]
=> <Variable>

In[2]:= x
=> Int

Out[2]:= 10
```

#### Control

`If`, `Cond`, `Select`, `Do`, `Table`, `With`

All same as Wolfram

### Module

Define a module, use `StartModule`.

```ksl
StartModule["KSL/TestModule"]

Val[f_Function, Fun[{x_Int}, x + 1]]

StartModule["KSL/TestModule/Inside"]

Val[y_Complex, Complex[2, 3]]

Export[{y}]

EndModule["KSL/TestModule/Inside"]

Export[{f, Inside}]
EndModule["KSL/TestModule"]
```

Use modules, use `Load`.

```ksl
Load[m, "KSL/TestModule"]

Print[m.f[10]]
```

Here, m is the name represent the module.
A module inside a module
can only called by the outer module.

```ksl
Print[m.Inside.y]
```
