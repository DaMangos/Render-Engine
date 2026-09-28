#include <../src/dependencies.hpp>

#include <gtest/gtest.h>

#include <concepts>
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

static_assert(std::tuple_size_v<graphics::dependencies<A>> == 1);
static_assert(std::tuple_size_v<graphics::dependencies<A, B>> == 2);
static_assert(std::tuple_size_v<graphics::dependencies<A, B, C>> == 3);
static_assert(std::tuple_size_v<graphics::dependencies<A, B, C, D>> == 4);
static_assert(std::same_as<std::tuple_element_t<0, graphics::dependencies<A, B, C, D>>, A const &>);
static_assert(std::same_as<std::tuple_element_t<1, graphics::dependencies<A, B, C, D>>, B const &>);
static_assert(std::same_as<std::tuple_element_t<2, graphics::dependencies<A, B, C, D>>, C const &>);
static_assert(std::same_as<std::tuple_element_t<3, graphics::dependencies<A, B, C, D>>, D const &>);
static_assert(std::same_as<graphics::dependency_union_type<graphics::dependencies<A, B>, graphics::dependencies<B, C>>,
                           graphics::dependencies<A, B, C>>);

TEST(DependentTest, StructuredBindings)
{
  graphics::dependent<A> a;
  graphics::dependent<B> b;

  auto deps = graphics::dependency_union(a.as_dependencies(), b.as_dependencies());

  static_assert(std::is_same_v<decltype(deps), graphics::dependencies<A, B>>);

  EXPECT_EQ(&deps.get<A>(), &a.get());
  EXPECT_EQ(&deps.get<B>(), &b.get());

  EXPECT_EQ(&deps.get<0>(), &a.get());
  EXPECT_EQ(&deps.get<1>(), &b.get());

  EXPECT_EQ(&graphics::get<A>(deps), &a.get());
  EXPECT_EQ(&graphics::get<B>(deps), &b.get());

  EXPECT_EQ(&graphics::get<0>(deps), &a.get());
  EXPECT_EQ(&graphics::get<1>(deps), &b.get());

  auto const & [a_dep, b_dep] = deps;

  EXPECT_EQ(&a_dep, &a.get());
  EXPECT_EQ(&b_dep, &b.get());
}

TEST(DependentTest, CleanUpInvoked)
{
  bool destructor_called = false;

  {
    graphics::dependent<bool *>{[](bool *& destructor_called) { *destructor_called = true; }, &destructor_called};
  }

  EXPECT_TRUE(destructor_called);
}

TEST(DependentTest, CleanUpInvokedWithDependencies)
{
  bool cleanup_invoked = false;

  graphics::dependent<A> a;

  {
    graphics::dependent<bool *, graphics::dependencies<A>>([](bool *& cleanup_invoked, A const &) { *cleanup_invoked = true; },
                                                           a.as_dependencies(),
                                                           &cleanup_invoked);
  }

  EXPECT_TRUE(cleanup_invoked);
}

TEST(DependentTest, CleanUpInvokedWithDependenciesNoFirstArg)
{
  bool cleanup_invoked = false;

  graphics::dependent<A> a;

  {
    graphics::dependent<B, graphics::dependencies<A>>([&](A const &) { cleanup_invoked = true; }, a.as_dependencies(), );
  }

  EXPECT_TRUE(cleanup_invoked);
}

TEST(DependentTest, CleanUpInvokedWithoutDependencies)
{
  bool cleanup_invoked = false;

  {
    graphics::dependent<bool *>([](bool *& cleanup_invoked) { *cleanup_invoked = true; }, &cleanup_invoked);
  }

  EXPECT_TRUE(cleanup_invoked);
}

TEST(DependentTest, CleanUpInvokedWithoutDependenciesNoArgs)
{
  bool cleanup_invoked = false;

  {
    graphics::dependent<A>([&]() { cleanup_invoked = true; });
  }

  EXPECT_TRUE(cleanup_invoked);
}

TEST(DependentTest, DependencyIsRetained)
{
  graphics::dependent<A>                            a;
  graphics::dependent<B, graphics::dependencies<A>> b(a.as_dependencies());

  EXPECT_EQ(&b.get_dependency<A>(), &a.get());
}

TEST(DependentTest, AsDependenciesContainsDependentAndDependencies)
{
  graphics::dependent<A>                            a;
  graphics::dependent<B, graphics::dependencies<A>> b(a.as_dependencies());

  auto deps = b.as_dependencies();

  static_assert(std::is_same_v<decltype(deps), graphics::dependencies<B, A>>);

  EXPECT_EQ(&deps.get<B>(), &b.get());
  EXPECT_EQ(&deps.get<A>(), &a.get());
}

TEST(DependentTest, UniqueUnionOfDependencies)
{
  graphics::dependent<A>                            a;
  graphics::dependent<B, graphics::dependencies<A>> b(a.as_dependencies());
  graphics::dependent<C>                            c;
  graphics::dependent<D, graphics::dependencies<C>> d(c.as_dependencies());

  auto result = graphics::dependency_union(graphics::dependency_union(a.as_dependencies(), b.as_dependencies()),
                                           graphics::dependency_union(c.as_dependencies(), d.as_dependencies()));

  static_assert(std::is_same_v<decltype(result), graphics::dependencies<A, B, C, D>>);

  EXPECT_EQ(&result.get<A>(), &a.get());
  EXPECT_EQ(&result.get<B>(), &b.get());
  EXPECT_EQ(&result.get<C>(), &c.get());
  EXPECT_EQ(&result.get<D>(), &d.get());
}

TEST(DependentTest, UnionOfDependencies)
{
  graphics::dependent<A>                            a;
  graphics::dependent<B, graphics::dependencies<A>> b(a.as_dependencies());
  graphics::dependent<C, graphics::dependencies<A>> c(a.as_dependencies());

  auto result = graphics::dependency_union(b.as_dependencies(), c.as_dependencies());

  static_assert(std::is_same_v<decltype(result), graphics::dependencies<B, A, C>>);

  EXPECT_EQ(&result.get<A>(), &a.get());
  EXPECT_EQ(&result.get<B>(), &b.get());
  EXPECT_EQ(&result.get<C>(), &c.get());
}

TEST(DependentTest, MoveConstruction)
{
  graphics::dependent<A> a;

  graphics::dependent<A> b(std::move(a));

  EXPECT_TRUE(a.valueless_after_move());
  EXPECT_FALSE(b.valueless_after_move());
}

TEST(DependentTest, MoveAssignment)
{
  graphics::dependent<A> a;
  graphics::dependent<A> b;

  b = std::move(a);

  EXPECT_TRUE(a.valueless_after_move());
  EXPECT_FALSE(b.valueless_after_move());
}
}