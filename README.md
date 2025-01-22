# lua-tostring

This module takes arguments of any type and converts them into a human-readable string representation.


## Installation

```sh
luarocks install tostring
```

---

## ... = tostring( ... )

this function converts its arguments to strings.

It calls that `__tostring` metamethod and uses its return value if the metamethod is defined and returns a string. Otherwise, the argument is converted to a string using the following rules:


- `nil`: `"nil"`.
- `boolean`: `"true"` or `"false"`.
- `string`: the string itself.
- `number`: the string representation of the number that is produced by `lua_pushfstring()` with the format `"%I"` (or `"%d"` if Lua version is less than 5.3) if the number is an integer. If the number is a float, then the format is `"%f"`.
- `table` and `userdata`: if the argument has a metatable with a `__name` field, then use the value of this field as the typename of the argument with the address of the argument in hexadecimal format. Otherwise, use the typename of the argument with the address of the argument in hexadecimal format. (e.g. `"table: 0x600003ac1bc0"`)
- any other type: the typename of the argument with the address of the argument in hexadecimal format. (e.g. `"function: 0x600003ac1bc0"`)


**Parameters:**

- `...`: any values.

**Returns:**

- `...`: a string representation of the input values.


**Example**

```lua
local dump = require('dump')
local tostring = require('tostring')
local list = {
    tostring(nil, true, false, 'hello', 12345, 123.45, {}, function()
    end, coroutine.create(function()
    end), setmetatable({}, {
        __name = 'myname',
    })),
}
print(dump(list))
-- {
--     [1] = "nil",
--     [2] = "true",
--     [3] = "false",
--     [4] = "hello",
--     [5] = "12345",
--     [6] = "123.45",
--     [7] = "table: 0x600003ac1bc0",
--     [8] = "function: 0x600002fc18c0",
--     [9] = "thread: 0x6000014c41a8",
--     [10] = "myname: 0x600003ac1c00"
-- }
```
