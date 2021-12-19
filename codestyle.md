# Code Style

1. Learn c++
   1. [This][cpp_help1] may help if you come from other languages

## Methods

* Public: `PascalCase`
* Private: `PascalCase_`
* Private that are shared with `friend`s: `PascalCase`
* Parameter/Return Types: Applies to everything!
   if type is non-complex (int, float, pointer): `<Type> a`
   otherwise (e.g. `std::vector`): `const <Type> &a`
* Getters: `Get<FieldName>() const`
   Note: if field is a boolean, `Is<FieldName>() const`
* Setters: `Set<FieldName>(<Type> new<FieldName>)`
* Note: if a getter returns an immedeate value, it can only return `<Type>`
   becasue c++

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
   Self::RootCause<Unnoticed::Do<Self::Actions::All<3>,This::Z<0x1662>>>
      .PollAllUpgrades<System::RootConsole::UI>(FindAll<Utility::Self<4>>).GenerateInternalStateMachine(143);
   if(very::complex::thing<typename myfunc::conditional<3>(Value.argument + 3) == (Condition::V<17>)) {
      behaviour::Tree<FindValue_t<Self, This::Y<14, Value.reason + Self>>>::Generate<K13 == (this->ValuesT<3>().argument)>();
      Self::TreeLike<Behaviour::FindBoxed_t<Self, This::X<19283, 0x1662>>>.Poll<Console>(Logger::SelectAllMatching<this->Predicate<3>);
   }
   More::Stuff("Important String [commons::selector(findroot)])", 127382.28f 0x17283 + 7128372.2f & 3)
   ```

   Do you see *at a glance* where the code block begins and ends? I don't.

    ```c++
   Self::RootCause<Unnoticed::Do<Self::Actions::All<3>,This::Z<0x1662>>>
      .PollAllUpgrades<System::RootConsole::UI>(FindAll<Utility::Self<4>>).GenerateInternalStateMachine(143);
   if(very::complex::thing<typename myfunc::conditional<3>(Value.argument + 3) == (Condition::V<17>))
   {
      behaviour::Tree<FindValue_t<Self, This::Y<14, Value.reason + Self>>>::Generate<K13 == (this->ValuesT<3>().argument)>();
      Self::TreeLike<Behaviour::FindBoxed_t<Self, This::X<19283, 0x1662>>>.Poll<Console>(Logger::SelectAllMatching<this->Predicate<3>);
   }
   More::Stuff("Important String [commons::selector(findroot)])", 127382.28f 0x17283 + 7128372.2f & 3)
   ```

   Still a mess, but you at least see where the `if` is.

## Comments

### Documentation Comments

* If you're using vscode install the [Doxygen Documentation Generator](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen)

### "TODO"/"FIXME" Comments

* A 'bare' TODO comment begins with `TODO:` and explanation what to do later (Note: explanation doesnt have to be long)
* A 'bare' FIXME comment begins with `FIXME:` and explanation of what the bug is (do not misuse!)
* You can direct a TODO/FIXME onto someone (like git blame or sth.) with this syntax:
  `TODO(->NICKNAME): ...` or `FIXME(->NICKNAME): ...` (Note: do not use real names!)
* If you indend to fix the issue yourself and make sure others don't fix it, add *just* your nickname in parens.
  `TODO(NICKNAME): <Very context-dependent thing with a lot of stuff that only you know/understand>`
* If you are not sure whether this is an issue, use a question mark instead
of a colon:
  `TODO? Move this text 3 pixels to the left, but looks fine either way`
* If a TODO/FIXME explanation is redundant (rarely) you can write it without a message

### Other Comments

* In general, use C99-style comments:

  ```c++
  // This is a comment.
  
  // This is a very very very very very very long comment message
  // with a lot of stuff that is very important. TODO: gotta to somethin'!

  "(Note^) nothing goes after a TODO/FIXME in a multi-line comment"

  /** This is a documentation comment */
  int MyCoolFunction();

  "(Note^) Two stars at the beginning"

  /**
   * This is a multi-line documentation comment
   **/
  int MultilineFunction();

  "(Note^) Two stars at the end"

  /**
   * And this
   * */
  int MultilineFunctionV2();

  "(Note^) The space between the two stars at the end"

  /**
   * And also this.
   */
  int MultilineFunctionV3();
  
  "(Note^) A single star at the end"

  /** This is also possible
   * Im not sure if I want to leave it though */
  int MultilineFunctionV4();
  
  "(Note^) No newlines before/after end and start"
  ```

## General Style

clang-format shoud handle it all.
so you can read `.clang-format` or
write some code and see what changed.

## Semantics style

* redundant namespaces:
  Sometimes, c++ can't resolve a symbol, so you are required to
  write out the full name of the namespace. Also sometimes, you
  have similar symbols in different namespaces, you might want
  to specify the partical name of the namespace. For example:
  you have two symbols: `EventTypeMouseMoved` and `EventTypeFocus`
  from  different categories (one from the input system and the
  other from the widget system) If you have both avaliable in
  the same context, write `event::EventTypeMouseMoved` and
  `gui::EventTypeFocus` (but sometimes it's obvious, so don't
  specify the namespace)

[cpp_help1]: https://stackoverflow.com/a/333964/9110517
