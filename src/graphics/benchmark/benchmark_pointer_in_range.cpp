#include <benchmark/benchmark.h>

#include <algorithm>
#include <array>
#include <execution>

namespace
{
static std::array<std::byte, 64 * 64 * 64 * 64 * 64> range;
static std::byte *                                   pointer;

static void pointer_in_range_control(benchmark::State & state)
{
  benchmark::DoNotOptimize(pointer);

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(std::any_of(range.begin(),  //
                                         range.end(),    //
                                         [ptr = pointer](std::byte const & byte) { return &byte == ptr; }));
  }
}

BENCHMARK(pointer_in_range_control);

static void pointer_in_range_seq(benchmark::State & state)
{
  benchmark::DoNotOptimize(pointer);

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(std::any_of(std::execution::seq,
                                         range.begin(),
                                         range.end(),
                                         [ptr = pointer](std::byte const & byte) { return &byte == ptr; }));
  }
}

BENCHMARK(pointer_in_range_seq);

static void pointer_in_range_par(benchmark::State & state)
{
  benchmark::DoNotOptimize(pointer);

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(std::any_of(std::execution::par,
                                         range.begin(),
                                         range.end(),
                                         [ptr = pointer](std::byte const & byte) { return &byte == ptr; }));
  }
}

BENCHMARK(pointer_in_range_par);

static void pointer_in_range_par_unseq(benchmark::State & state)
{
  benchmark::DoNotOptimize(pointer);

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(std::any_of(std::execution::par_unseq,
                                         range.begin(),
                                         range.end(),
                                         [ptr = pointer](std::byte const & byte) { return &byte == ptr; }));
  }
}

BENCHMARK(pointer_in_range_par_unseq);

static void pointer_in_range_unseq(benchmark::State & state)
{
  benchmark::DoNotOptimize(pointer);

  for(auto _ : state)
  {
    benchmark::DoNotOptimize(std::any_of(std::execution::unseq,
                                         range.begin(),
                                         range.end(),
                                         [ptr = pointer](std::byte const & byte) { return &byte == ptr; }));
  }
}

BENCHMARK(pointer_in_range_unseq);
}