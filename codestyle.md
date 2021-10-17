# Code Style

1. Learn c++
   1. [This][cpp_help1] may help if you come from other languages

## Methods

* Public: `PascalCase`
* Private: `PascalCase_`
* Parameter/Return Types: Applies to everything!
   if type is non-complex (int, float, pointer): `<Type> a`
   otherwise (e.g. `std::vector`): `const <Type> &a`
* Getters: `Get<FieldName>() const`
   Note: if field is a boolean, `Is<FieldName>() const`
* Setters: `Set<FieldName>(<Type> new<FieldName>)`

## Fields

* Public: `PascalCase`
* Private: `PascalCase_`

## Local Variables

* `camelCase`

## Parentheses

* Functions: `functionName(arg1, arg2, arg3)` (no spaces)
   Reason: Idk, looks better
* Control Statements: `if(condition)` (no spaces)
   Reason: a control statement is basically a function with a closure after it.
   I could implement `if` like this: `void my_if(bool condition, function then, function else);`
   and you won't put a space before the opening `(`.
* Code Blocks: Newline before  `{`
   Reason: Makes it easier to separate code blocks from all of the other noise:
   What is easier to read:

   ```c++
   Self::RootCause<Unnoticed::Do<Self::Actions::All<3>,This::Z<0x1662>>>.PollAllUpgrades<System::RootConsole::UI>(FindAll<Utility::Self<4>>);
   if(very::complex::thing<typename myfunc::conditional<3, (size_t)Value.size>>(Value.argument + 3) == (this->Values() + 16*(Condition::V<17>))) {
      behaviour::Tree<FindValue_t<Self, This::Y<14, Value.reason + Self>>>::Generate<K13 == (this->ValuesT<3>().argument)>();
      Self::TreeLike<Behaviour::FindBoxed_t<Self, This::X<19283, 0x1662>>>.Poll<Console>(Logger::SelectAllMatching<this->Predicate<3>);
   }
   ```

   Do you see *at a glance* where the code block begins and ends? I don't.

    ```c++
   Self::RootCause<Unnoticed::Do<Self::Actions::All<3>,This::Z<0x1662>>>.PollAllUpgrades<System::RootConsole::UI>(FindAll<Utility::Self<4>>);
   if(very::complex::thing<typename myfunc::conditional<3, (size_t)Value.size>>(Value.argument + 3) == (this->Values() + 16*(Condition::V<17>)))
   {
      behaviour::Tree<FindValue_t<Self, This::Y<14, Value.reason + Self>>>::Generate<K13 == (this->ValuesT<3>().argument)>();
      Self::TreeLike<Behaviour::FindBoxed_t<Self, This::X<19283, 0x1662>>>.Poll<Console>(Logger::SelectAllMatching<this->Predicate<3>);
   }
   ```

   Still a mess, but you at least see where the `if` is.

## General Style

clang-format shoud handle it all.
so you can read `.clang-format` or
write some code and see what it changed.

[cpp_help1]: https://stackoverflow.com/a/333964/9110517
