#include <khronos/dependencies.hpp>

#include <gtest/gtest.h>

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
  bool destructor_called = false;

  khronos::dependent<A> a;

  {
    khronos::dependent<bool *, khronos::dependencies<A>>([](bool *& destructor_called) { *destructor_called = true; },
                                                         a.as_dependencies(),
                                                         &destructor_called);
  }

  EXPECT_TRUE(destructor_called);

  destructor_called = false;

  {
    khronos::dependent<bool *, khronos::dependencies<A>>([](bool *& destructor_called, A const &)
                                                         { *destructor_called = true; },
                                                         a.as_dependencies(),
                                                         &destructor_called);
  }

  EXPECT_TRUE(destructor_called);
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