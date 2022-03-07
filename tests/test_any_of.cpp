// Copyright 2022, Emilien KIA
// https://github.com/EmilienKia/any_of
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "catch.hpp"

#include <cassert>

#include "../any_of.hpp"

using namespace ANY_OF_NS;

class base
{
private:
    int _i = -1;

public:
    constexpr base() noexcept : _i(0) {}
    constexpr base(const base&) noexcept = default;
    constexpr base(base&&) noexcept = default;
    base& operator=(const base&) noexcept = default;
    base& operator=(base&&) noexcept = default;

    constexpr base(int i) : _i(i) {}

    int value() const { return _i; }
};

class dep1 : public base
{
public:
    constexpr dep1() noexcept : base(1) {}
    constexpr dep1(const dep1&) noexcept = default;
    constexpr dep1(dep1&&) noexcept = default;
    dep1& operator=(const dep1&) noexcept = default;
    dep1& operator=(dep1&&) noexcept = default;

    constexpr dep1(int i) : base(i) {}
};

class dep11 : public dep1
{
public:
    constexpr dep11() noexcept : dep1(11) {}
    constexpr dep11(const dep11&) noexcept = default;
    constexpr dep11(dep11&&) noexcept = default;
    dep11& operator=(const dep11&) noexcept = default;
    dep11& operator=(dep11&&) noexcept = default;

    constexpr dep11(int i) : dep1(i) {}
};

class dep2 : public base
{
public:
    constexpr dep2() noexcept : base(2) {}
    constexpr dep2(const dep2&) noexcept = default;
    constexpr dep2(dep2&&) noexcept = default;
    dep2& operator=(const dep2&) noexcept = default;
    dep2& operator=(dep2&&) noexcept = default;

    constexpr dep2(int i) : base(i) {}
};

class dep3 : public base
{
public:
    constexpr dep3() noexcept : base(3) {}
    constexpr dep3(const dep3&) noexcept = default;
    constexpr dep3(dep3&&) noexcept = default;
    dep3& operator=(const dep3&) noexcept = default;
    dep3& operator=(dep3&&) noexcept = default;

    constexpr dep3(int i) : base(i) {}
};

typedef any_of<base, base, dep1, dep11, dep2> any_dep;
typedef any_of<base, base, dep1, dep2, dep3> any_other_dep;


TEST_CASE("AnyOf default construct", "[tools]") {
    any_dep d1;
    REQUIRE( d1->value() == 0 );
    REQUIRE( std::holds_alternative<base>(d1) == true );
    REQUIRE( d1.is<base>() );
}

TEST_CASE("AnyOf copy construct", "[tools]") {
    any_dep d1;
    REQUIRE(d1->value() == 0);

    any_dep d2(d1);
    REQUIRE(d2->value() == 0);
    REQUIRE(std::holds_alternative<base>(d2));
    REQUIRE(d2.is<base>());

    any_dep d3(std::move(d2));
    REQUIRE(d3->value() == 0);
    REQUIRE(std::holds_alternative<base>(d3));
    REQUIRE(d3.is<base>());
}

TEST_CASE("AnyOf another copy construct", "[tools]") {
    dep1 d;
    any_dep d1(d);
    REQUIRE(d1->value() == 1);
    REQUIRE(std::holds_alternative<dep1>(d1));
    REQUIRE(d1.is<dep1>());
    any_dep d2(d1);
    REQUIRE(d2->value() == 1);
    REQUIRE(std::holds_alternative<dep1>(d2));
    REQUIRE(d2.is<dep1>());
    any_dep d3(std::move(d2));
    REQUIRE(d3->value() == 1);
    REQUIRE(std::holds_alternative<dep1>(d3));
    REQUIRE(d3.is<dep1>());
}

TEST_CASE("AnyOf init construct", "[tools]") {
    any_dep d1(dep2{});
    REQUIRE(d1->value() == 2);
    REQUIRE(std::holds_alternative<dep2>(d1));
    REQUIRE(d1.is<dep2>());
    any_dep d2(d1);
    REQUIRE(d2->value() == 2);
    REQUIRE(std::holds_alternative<dep2>(d2));
    REQUIRE(d2.is<dep2>());
    any_dep d3(std::move(d2));
    REQUIRE(d3->value() == 2);
    REQUIRE(std::holds_alternative<dep2>(d3));
    REQUIRE(d3.is<dep2>());
}


TEST_CASE("AnyOf copy assignment", "[tools]")
{
    any_dep d1(dep2{});
    any_dep d2;
    d2 = d1;
    REQUIRE(std::holds_alternative<dep2>(d2));
    REQUIRE(d2->value() == 2);
    REQUIRE(d2.is<dep2>());
}

TEST_CASE("AnyOf move assignment", "[tools]")
{
    any_dep d1(dep2{});
    any_dep d2;
    d2 = std::move(d1);
    REQUIRE(std::holds_alternative<dep2>(d2));
    REQUIRE(d2->value() == 2);
    REQUIRE(d2.is<dep2>());
}

TEST_CASE("AnyOf init assignment", "[tools]")
{
    dep2 d;
    any_dep d1;
    d1 = d;
    REQUIRE(std::holds_alternative<dep2>(d1));
    REQUIRE(d1->value() == 2);
    REQUIRE(d1.is<dep2>());
}

TEST_CASE("AnyOf init move construct", "[tools]")
{
    dep2 d;
    any_dep d1;
    d1 = std::move(d);
    REQUIRE(std::holds_alternative<dep2>(d1));
    REQUIRE(d1.is<dep2>());
    REQUIRE(d1->value() == 2);
}

TEST_CASE("AnyOf emplace", "[tools]")
{
    any_dep d1;
    d1.emplace<dep2>(4);
    REQUIRE(std::holds_alternative<dep2>(d1));
    REQUIRE(d1.is<dep2>());
    REQUIRE(d1->value() == 4);
}

TEST_CASE("AnyOf swap", "[tools]")
{
    any_dep d1{ dep1{} };
    any_dep d2{ dep2{} };
    d1.swap(d2);
    REQUIRE(d1.is<dep2>());
    REQUIRE(d1->value() == 2);
    REQUIRE(d2.is<dep1>());
    REQUIRE(d2->value() == 1);
}

TEST_CASE("AnyOf get", "[tools]")
{
    any_dep d{ dep2{} };
    dep2& d2 = d.get<dep2>();
    REQUIRE(d2.value() == 2);
    REQUIRE_THROWS_AS([&]() { 
        dep1& d1 = d.get<dep1>();
    }(), std::bad_variant_access);
}

TEST_CASE("AnyOf get if", "[tools]")
{
    any_dep d{ dep2{} };
    dep2* d2 = d.get_if<dep2>();
    REQUIRE(d2 != nullptr);
    dep1* d1 = d.get_if<dep1>();
    REQUIRE(d1 == nullptr);
}

TEST_CASE("AnyOf other compatible assignation", "[tools]")
{
    any_dep d2{ dep2{} };
    any_other_dep other;
    other = d2;
    REQUIRE(other.is<dep2>());
    REQUIRE(other->value() == 2);
}

TEST_CASE("AnyOf other incompatible assignation", "[tools]")
{
    any_dep d11{ dep11{} };
    any_other_dep other;
    REQUIRE_THROWS_AS([&]() {
        other = d11;
    }(), std::bad_variant_access);
}

TEST_CASE("AnyOf other incompatible construct", "[tools]")
{
    any_dep d11{ dep11{} };
    REQUIRE_THROWS_AS([&]() {
        any_other_dep other{ d11 };
    }(), std::bad_variant_access);
}

TEST_CASE("AnyOf construct from AnyOfOpt", "[tools]")
{
    any_dep::any_of_opt_t d1{ dep1{} };
    any_dep d2{ d1 };
    REQUIRE(d2.is<dep1>());
    REQUIRE(d2->value() == 1);
}

TEST_CASE("AnyOf construct from empty AnyOfOpt", "[tools]")
{
    any_dep::any_of_opt_t d1{ };
    REQUIRE_THROWS_AS([&]() {
        any_dep d2{ d1 };
        }(), std::bad_variant_access);
}

TEST_CASE("AnyOf other incompatible construct from AnyOfOpt", "[tools]")
{
    any_dep::any_of_opt_t d11{ dep11{} };
    REQUIRE_THROWS_AS([&]() {
        any_other_dep other{ d11 };
        }(), std::bad_variant_access);
}