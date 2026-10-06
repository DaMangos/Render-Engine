#include <../src/device_memory_resource.hpp>

#include <gtest/gtest.h>

namespace
{
class mock_buffer
{
  public:
    mock_buffer(std::size_t const allocation_size, std::size_t const)
    : allocation_size(allocation_size),
      bytes(std::make_unique<std::byte[]>(allocation_size))
    {
    }

    [[nodiscard]]
    void * data() noexcept
    {
      return bytes.get();
    }

  private:
    std::size_t                          allocation_size;
    mutable std::unique_ptr<std::byte[]> bytes;
};

TEST(DeviceMemoryResourceTest, BeginAllocatePointerFound)
{
  auto memory_resource = graphics::device_memory_resource<mock_buffer>{0};

  EXPECT_TRUE(memory_resource.empty());

  void * a = memory_resource.allocate(10uz);
  void * b = memory_resource.allocate(20uz);
  void * c = memory_resource.allocate(30uz);
  void * d = memory_resource.allocate(40uz);

  EXPECT_FALSE(memory_resource.empty());

  auto & a_buffer = memory_resource.find_buffer(a);
  auto & b_buffer = memory_resource.find_buffer(b);
  auto & c_buffer = memory_resource.find_buffer(c);
  auto & d_buffer = memory_resource.find_buffer(d);

  EXPECT_EQ(a, a_buffer.get_mapped_memory().data());
  EXPECT_EQ(b, b_buffer.get_mapped_memory().data());
  EXPECT_EQ(c, c_buffer.get_mapped_memory().data());
  EXPECT_EQ(d, d_buffer.get_mapped_memory().data());

  EXPECT_EQ(10uz, a_buffer.get_mapped_memory().size());
  EXPECT_EQ(20uz, b_buffer.get_mapped_memory().size());
  EXPECT_EQ(30uz, c_buffer.get_mapped_memory().size());
  EXPECT_EQ(40uz, d_buffer.get_mapped_memory().size());

  memory_resource.deallocate(a, 10uz);
  memory_resource.deallocate(b, 20uz);
  memory_resource.deallocate(c, 30uz);
  memory_resource.deallocate(d, 40uz);

  EXPECT_THROW(std::ignore = memory_resource.find_buffer(a), std::out_of_range);
  EXPECT_THROW(std::ignore = memory_resource.find_buffer(b), std::out_of_range);
  EXPECT_THROW(std::ignore = memory_resource.find_buffer(c), std::out_of_range);
  EXPECT_THROW(std::ignore = memory_resource.find_buffer(d), std::out_of_range);

  EXPECT_TRUE(memory_resource.empty());
}

TEST(DeviceMemoryResourceTest, InRangeAllocatePointerFound)
{
  auto memory_resource = graphics::device_memory_resource<mock_buffer>{0};

  EXPECT_TRUE(memory_resource.empty());

  auto * a = static_cast<std::byte *>(memory_resource.allocate(10uz));
  auto * b = static_cast<std::byte *>(memory_resource.allocate(20uz));
  auto * c = static_cast<std::byte *>(memory_resource.allocate(30uz));
  auto * d = static_cast<std::byte *>(memory_resource.allocate(40uz));

  EXPECT_FALSE(memory_resource.empty());

  auto & a_buffer = memory_resource.find_buffer(a + 5);
  auto & b_buffer = memory_resource.find_buffer(b + 10);
  auto & c_buffer = memory_resource.find_buffer(c + 15);
  auto & d_buffer = memory_resource.find_buffer(d + 20);

  EXPECT_EQ(a, a_buffer.get_mapped_memory().data());
  EXPECT_EQ(b, b_buffer.get_mapped_memory().data());
  EXPECT_EQ(c, c_buffer.get_mapped_memory().data());
  EXPECT_EQ(d, d_buffer.get_mapped_memory().data());

  EXPECT_EQ(10uz, a_buffer.get_mapped_memory().size());
  EXPECT_EQ(20uz, b_buffer.get_mapped_memory().size());
  EXPECT_EQ(30uz, c_buffer.get_mapped_memory().size());
  EXPECT_EQ(40uz, d_buffer.get_mapped_memory().size());

  memory_resource.deallocate(a, 10uz);
  memory_resource.deallocate(b, 20uz);
  memory_resource.deallocate(c, 30uz);
  memory_resource.deallocate(d, 40uz);

  EXPECT_THROW(std::ignore = memory_resource.find_buffer(a + 5), std::out_of_range);
  EXPECT_THROW(std::ignore = memory_resource.find_buffer(b + 10), std::out_of_range);
  EXPECT_THROW(std::ignore = memory_resource.find_buffer(c + 15), std::out_of_range);
  EXPECT_THROW(std::ignore = memory_resource.find_buffer(d + 20), std::out_of_range);

  EXPECT_TRUE(memory_resource.empty());
}

TEST(DeviceMemoryResourceTest, EndAllocatePointerNotFound)
{
  auto memory_resource = graphics::device_memory_resource<mock_buffer>{0};

  EXPECT_TRUE(memory_resource.empty());

  auto * a = static_cast<std::byte *>(memory_resource.allocate(10uz));
  auto * b = static_cast<std::byte *>(memory_resource.allocate(20uz));
  auto * c = static_cast<std::byte *>(memory_resource.allocate(30uz));
  auto * d = static_cast<std::byte *>(memory_resource.allocate(40uz));

  EXPECT_FALSE(memory_resource.empty());

  EXPECT_THROW(std::ignore = memory_resource.find_buffer(a + 10), std::out_of_range);
  EXPECT_THROW(std::ignore = memory_resource.find_buffer(b + 20), std::out_of_range);
  EXPECT_THROW(std::ignore = memory_resource.find_buffer(c + 30), std::out_of_range);
  EXPECT_THROW(std::ignore = memory_resource.find_buffer(d + 40), std::out_of_range);

  memory_resource.deallocate(a, 10uz);
  memory_resource.deallocate(b, 20uz);
  memory_resource.deallocate(c, 30uz);
  memory_resource.deallocate(d, 40uz);

  EXPECT_TRUE(memory_resource.empty());
}

TEST(DeviceMemoryResourceTest, ValidPointerNotFound)
{
  auto memory_resource = graphics::device_memory_resource<mock_buffer>{0};

  EXPECT_TRUE(memory_resource.empty());

  void * a = memory_resource.allocate(10uz);
  void * b = memory_resource.allocate(20uz);
  void * c = memory_resource.allocate(30uz);
  void * d = memory_resource.allocate(40uz);

  EXPECT_FALSE(memory_resource.empty());

  int i = 0;

  EXPECT_THROW(std::ignore = memory_resource.find_buffer(&i), std::out_of_range);

  memory_resource.deallocate(a, 10uz);
  memory_resource.deallocate(b, 20uz);
  memory_resource.deallocate(c, 30uz);
  memory_resource.deallocate(d, 40uz);

  EXPECT_TRUE(memory_resource.empty());
}

TEST(DeviceMemoryResourceTest, NullPointerNotFound)
{
  auto memory_resource = graphics::device_memory_resource<mock_buffer>{0};

  EXPECT_TRUE(memory_resource.empty());

  void * a = memory_resource.allocate(10uz);
  void * b = memory_resource.allocate(20uz);
  void * c = memory_resource.allocate(30uz);
  void * d = memory_resource.allocate(40uz);

  EXPECT_FALSE(memory_resource.empty());

  EXPECT_THROW(std::ignore = memory_resource.find_buffer(nullptr), std::out_of_range);

  memory_resource.deallocate(a, 10uz);
  memory_resource.deallocate(b, 20uz);
  memory_resource.deallocate(c, 30uz);
  memory_resource.deallocate(d, 40uz);

  EXPECT_TRUE(memory_resource.empty());
}

TEST(DeviceMemoryResourceTest, VectorDataFound)
{
  auto memory_resource = graphics::device_memory_resource<mock_buffer>{0};

  EXPECT_TRUE(memory_resource.empty());

  auto vec = std::pmr::vector<int>(&memory_resource);

  for(int i = 0; i < 20; ++i)
    vec.push_back(i);

  EXPECT_FALSE(memory_resource.empty());

  EXPECT_TRUE(std::ranges::any_of(memory_resource.find_buffer(vec.data()).get_mapped_memory(),
                                  [ptr = static_cast<void const *>(vec.data())](std::byte const & byte)
                                  { return &byte == ptr; }));
}
}