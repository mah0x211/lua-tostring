local assert = require('assert')
local tostring = require('tostring')

-- test that tostring converts a value to a string
assert.equal(tostring(nil), 'nil')
for expected, v in pairs({
    ['true'] = true,
    ['false'] = false,
    ['hello'] = 'hello',
    ['12345'] = 12345,
    ['123%.45'] = 123.45,
    ['^hello world$'] = setmetatable({}, {
        __tostring = function()
            return 'hello world'
        end,
    }),
    ['^myname: '] = setmetatable({}, {
        __name = 'myname',
    }),
    ['^table: '] = {},
    ['^function: '] = function()
    end,
    ['^thread: '] = coroutine.create(function()
    end),
}) do
    assert.match(tostring(v), expected, false)
end

-- test that tostring accept a multiple arguments
local list = {
    tostring(nil, true, false, 'hello', 12345, 123.45, {}, function()
    end, coroutine.create(function()
    end), setmetatable({}, {
        __name = 'myname',
    })),
}
for expected, actual in pairs({
    ['nil'] = list[1],
    ['true'] = list[2],
    ['false'] = list[3],
    ['hello'] = list[4],
    ['12345'] = list[5],
    ['123%.45'] = list[6],
    ['^table: '] = list[7],
    ['^function: '] = list[8],
    ['^thread: '] = list[9],
    ['^myname: '] = list[10],
}) do
    assert.match(actual, expected, false)
end

-- test that ignore the return value of __tostring metamethod if it is not a string
local res = tostring(setmetatable({}, {
    __tostring = function()
        return 9324
    end,
}))
assert.match(res, '^table: ', false)

-- test that ignore the __name field if it is not a string
res = tostring(setmetatable({}, {
    __name = 123,
}))
assert.match(res, '^table: ', false)
