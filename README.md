# any_of

any_of is a C++ single-header aiming to provide modern strong-typed union.

It acts as an C/C++ union or C++ std::variant but with data member which must derive from a same common type.
any_of is able to embed one instance of a list of types, all these types shall be or override a common type.
The embedded object can be directly accessed with the base common type without having to write any casting,
type reinterpretation nor visiting helper.

## How to use
Simply include the header and use any_of or any_of_opt classes.

```cpp
#include <any_of.hpp>

anyof::any_of<base, type1, type2, type3> any1{ type1{/*...*/} };
any1->mycommonfunc();
```
