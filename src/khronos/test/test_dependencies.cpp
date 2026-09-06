#include <../src/dependencies.hpp>

#include <gtest/gtest.h>

#include <concepts>
#include <functional>
#include <utility>

namespace
{
struct A
{
};

struct B
{
};

struct C
{
};

struct D
{
};

static_assert(std::tuple_size_v<khronos::dependencies<A>> == 1);
static_assert(std::tuple_size_v<khronos::dependencies<A, B>> == 2);
static_assert(std::tuple_size_v<khronos::dependencies<A, B, C>> == 3);
static_assert(std::tuple_size_v<khronos::dependencies<A, B, C, D>> == 4);
static_assert(std::same_as<std::tuple_element_t<0, khronos::dependencies<A, B, C, D>>, A const &>);
static_assert(std::same_as<std::tuple_element_t<1, khronos::dependencies<A, B, C, D>>, B const &>);
static_assert(std::same_as<std::tuple_element_t<2, khronos::dependencies<A, B, C, D>>, C const &>);
static_assert(std::same_as<std::tuple_element_t<3, khronos::dependencies<A, B, C, D>>, D const &>);
static_assert(std::same_as<khronos::dependency_union_type<khronos::dependencies<A, B>, khronos::dependencies<B, C>>,
                           khronos::dependencies<A, B, C>>);

TEST(DependentTest, StructuredBindings)
{
  khronos::dependent<A> a;
  khronos::dependent<B> b;

  auto deps = khronos::dependency_union(a.as_dependencies(), b.as_dependencies());

  static_assert(std::is_same_v<decltype(deps), khronos::dependencies<A, B>>);

  EXPECT_EQ(&deps.get<A>(), &a.get());
  EXPECT_EQ(&deps.get<B>(), &b.get());

  EXPECT_EQ(&deps.get<0>(), &a.get());
  EXPECT_EQ(&deps.get<1>(), &b.get());

  EXPECT_EQ(&khronos::get<A>(deps), &a.get());
  EXPECT_EQ(&khronos::get<B>(deps), &b.get());

  EXPECT_EQ(&khronos::get<0>(deps), &a.get());
  EXPECT_EQ(&khronos::get<1>(deps), &b.get());

  auto const & [a_dep, b_dep] = deps;

  EXPECT_EQ(&a_dep, &a.get());
  EXPECT_EQ(&b_dep, &b.get());
}

TEST(DependentTest, CleanUpInvoked)
{
  bool destructor_called = false;

  {
    khronos::dependent<bool *>{[](bool *& destructor_called) { *destructor_called = true; }, &destructor_called};
  }

  EXPECT_TRUE(destructor_called);
}

TEST(DependentTest, CleanUpInvokedWithDependencies)
{
  bool cleanup_invoked = false;

  khronos::dependent<A> a;

  {
    khronos::dependent<bool *, khronos::dependencies<A>>([](bool *& cleanup_invoked, A const &) { *cleanup_invoked = true; },
                                                         a.as_dependencies(),
                                                         &cleanup_invoked);
  }

  EXPECT_TRUE(cleanup_invoked);
}

TEST(DependentTest, CleanUpInvokedWithoutDependencies)
{
  bool cleanup_invoked = false;

  {
    khronos::dependent<bool *>([](bool *& cleanup_invoked) { *cleanup_invoked = true; }, &cleanup_invoked);
  }

  EXPECT_TRUE(cleanup_invoked);
}

TEST(DependentTest, DependencyIsRetained)
{
  khronos::dependent<A>                           a;
  khronos::dependent<B, khronos::dependencies<A>> b(a.as_dependencies());

  EXPECT_EQ(&b.get_dependency<A>(), &a.get());
}

TEST(DependentTest, AsDependenciesContainsDependentAndDependencies)
{
  khronos::dependent<A>                           a;
  khronos::dependent<B, khronos::dependencies<A>> b(a.as_dependencies());

  auto deps = b.as_dependencies();

  static_assert(std::is_same_v<decltype(deps), khronos::dependencies<B, A>>);

  EXPECT_EQ(&deps.get<B>(), &b.get());
  EXPECT_EQ(&deps.get<A>(), &a.get());
}

TEST(DependentTest, UniqueUnionOfDependencies)
{
  khronos::dependent<A>                           a;
  khronos::dependent<B, khronos::dependencies<A>> b(a.as_dependencies());
  khronos::dependent<C>                           c;
  khronos::dependent<D, khronos::dependencies<C>> d(c.as_dependencies());

  auto result = khronos::dependency_union(khronos::dependency_union(a.as_dependencies(), b.as_dependencies()),
                                          khronos::dependency_union(c.as_dependencies(), d.as_dependencies()));

  static_assert(std::is_same_v<decltype(result), khronos::dependencies<A, B, C, D>>);

  EXPECT_EQ(&result.get<A>(), &a.get());
  EXPECT_EQ(&result.get<B>(), &b.get());
  EXPECT_EQ(&result.get<C>(), &c.get());
  EXPECT_EQ(&result.get<D>(), &d.get());
}

TEST(DependentTest, UnionOfDependencies)
{
  khronos::dependent<A>                           a;
  khronos::dependent<B, khronos::dependencies<A>> b(a.as_dependencies());
  khronos::dependent<C, khronos::dependencies<A>> c(a.as_dependencies());

  auto result = khronos::dependency_union(b.as_dependencies(), c.as_dependencies());

  static_assert(std::is_same_v<decltype(result), khronos::dependencies<B, A, C>>);

  EXPECT_EQ(&result.get<A>(), &a.get());
  EXPECT_EQ(&result.get<B>(), &b.get());
  EXPECT_EQ(&result.get<C>(), &c.get());
}

TEST(DependentTest, MoveConstruction)
{
  khronos::dependent<A> a;

  khronos::dependent<A> b(std::move(a));

  EXPECT_TRUE(a.valueless_after_move());
  EXPECT_FALSE(b.valueless_after_move());
}

TEST(DependentTest, MoveAssignment)
{
  khronos::dependent<A> a;
  khronos::dependent<A> b;

  b = std::move(a);

  EXPECT_TRUE(a.valueless_after_move());
  EXPECT_FALSE(b.valueless_after_move());
}
}