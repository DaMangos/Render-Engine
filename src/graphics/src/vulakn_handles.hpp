#pragma once

#include "dependencies.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <cstddef>
#include <list>
#include <ostream>

namespace graphics
{
namespace vulkan
{
struct user_data
{
    std::ostream * vk_verbose_out;
    std::ostream * vk_info_out;
    std::ostream * vk_warning_out;
    std::ostream * vk_error_out;
};

using context = dependent<vk::raii::Context>;

using instance = dependent<vk::raii::Instance,              //
                           dependencies<vk::raii::Context,  //
                                        user_data>>;        //

using surface = dependent<vk::raii::SurfaceKHR,             //
                          dependencies<vk::raii::Instance,  //
                                       vk::raii::Context,   //
                                       user_data>>;

using debug_utils_messenger = dependent<vk::raii::DebugUtilsMessengerEXT,  //
                                        dependencies<vk::raii::Instance,   //
                                                     vk::raii::Context,    //
                                                     user_data>>;

using physical_device = dependent<vk::raii::PhysicalDevice,         //
                                  dependencies<vk::raii::Instance,  //
                                               vk::raii::Context,   //
                                               user_data>>;

using device = dependent<vk::raii::Device,                       //
                         dependencies<vk::raii::PhysicalDevice,  //
                                      vk::raii::Instance,        //
                                      vk::raii::Context,         //
                                      user_data>>;

using queue = dependent<vk::raii::Queue,                        //
                        dependencies<vk::raii::Device,          //
                                     vk::raii::PhysicalDevice,  //
                                     vk::raii::Instance,        //
                                     vk::raii::Context,         //
                                     user_data>>;

using pipeline_layout = dependent<vk::raii::PipelineLayout,               //
                                  dependencies<vk::raii::Device,          //
                                               vk::raii::PhysicalDevice,  //
                                               vk::raii::Instance,        //
                                               vk::raii::Context,         //
                                               user_data>>;

using pipeline = dependent<vk::raii::Pipeline,                     //
                           dependencies<vk::raii::PipelineLayout,  //
                                        vk::raii::Device,          //
                                        vk::raii::PhysicalDevice,  //
                                        vk::raii::Instance,        //
                                        vk::raii::Context,         //
                                        user_data>>;

using fence = dependent<vk::raii::Fence,                        //
                        dependencies<vk::raii::Queue,           //
                                     vk::raii::Device,          //
                                     vk::raii::PhysicalDevice,  //
                                     vk::raii::Instance,        //
                                     vk::raii::Context,         //
                                     user_data>>;

using fences = dependent<std::list<vk::raii::Fence>,             //
                         dependencies<vk::raii::Queue,           //
                                      vk::raii::Device,          //
                                      vk::raii::PhysicalDevice,  //
                                      vk::raii::Instance,        //
                                      vk::raii::Context,         //
                                      user_data>>;

using semaphores = dependent<std::list<vk::raii::Semaphore>,         //
                             dependencies<vk::raii::Queue,           //
                                          vk::raii::Device,          //
                                          vk::raii::PhysicalDevice,  //
                                          vk::raii::Instance,        //
                                          vk::raii::Context,         //
                                          user_data>>;

using command_pool = dependent<vk::raii::CommandPool,                  //
                               dependencies<vk::raii::Queue,           //
                                            vk::raii::Device,          //
                                            vk::raii::PhysicalDevice,  //
                                            vk::raii::Instance,        //
                                            vk::raii::Context,         //
                                            user_data>>;

using command_buffer = dependent<vk::raii::CommandBuffer,                //
                                 dependencies<vk::raii::CommandPool,     //
                                              vk::raii::Fence,           //
                                              vk::raii::Queue,           //
                                              vk::raii::Device,          //
                                              vk::raii::PhysicalDevice,  //
                                              vk::raii::Instance,        //
                                              vk::raii::Context,         //
                                              user_data>>;

using command_buffers = dependent<std::list<vk::raii::CommandBuffer>,       //
                                  dependencies<vk::raii::CommandPool,       //
                                               std::list<vk::raii::Fence>,  //
                                               vk::raii::Queue,             //
                                               vk::raii::Device,            //
                                               vk::raii::PhysicalDevice,    //
                                               vk::raii::Instance,          //
                                               vk::raii::Context,           //
                                               user_data>>;

using swapchain_create_info = dependent<vk::SwapchainCreateInfoKHR,         //
                                        dependencies<vk::raii::SurfaceKHR,  //
                                                     vk::raii::Instance,    //
                                                     vk::raii::Context,     //
                                                     user_data>>;

using swapchain = dependent<vk::raii::SwapchainKHR,                   //
                            dependencies<vk::SwapchainCreateInfoKHR,  //
                                         vk::raii::SurfaceKHR,        //
                                         vk::raii::Queue,             //
                                         vk::raii::Device,            //
                                         vk::raii::PhysicalDevice,    //
                                         vk::raii::Instance,          //
                                         vk::raii::Context,           //
                                         user_data>>;

using image_views = dependent<std::vector<vk::raii::ImageView>,         //
                              dependencies<vk::raii::SwapchainKHR,      //
                                           vk::SwapchainCreateInfoKHR,  //
                                           vk::raii::SurfaceKHR,        //
                                           vk::raii::Queue,             //
                                           vk::raii::Device,            //
                                           vk::raii::PhysicalDevice,    //
                                           vk::raii::Instance,          //
                                           vk::raii::Context,           //
                                           user_data>>;

using buffer = dependent<vk::raii::Buffer,                       //
                         dependencies<vk::raii::Device,          //
                                      vk::raii::PhysicalDevice,  //
                                      vk::raii::Instance,        //
                                      vk::raii::Context,         //
                                      user_data>>;

using buffers = dependent<std::list<vk::raii::Buffer>,              //
                          dependencies<std::list<vk::raii::Fence>,  //
                                       vk::raii::Queue,             //
                                       vk::raii::Device,            //
                                       vk::raii::PhysicalDevice,    //
                                       vk::raii::Instance,          //
                                       vk::raii::Context,           //
                                       user_data>>;

using device_memory = dependent<vk::raii::DeviceMemory,                 //
                                dependencies<vk::raii::Buffer,          //
                                             vk::raii::Device,          //
                                             vk::raii::PhysicalDevice,  //
                                             vk::raii::Instance,        //
                                             vk::raii::Context,         //
                                             user_data>>;

using device_memories = dependent<std::list<vk::raii::DeviceMemory>,         //
                                  dependencies<std::list<vk::raii::Buffer>,  //
                                               std::list<vk::raii::Fence>,   //
                                               vk::raii::Queue,              //
                                               vk::raii::Device,             //
                                               vk::raii::PhysicalDevice,     //
                                               vk::raii::Instance,           //
                                               vk::raii::Context,            //
                                               user_data>>;

using descriptor_pool = dependent<vk::raii::DescriptorPool,               //
                                  dependencies<vk::raii::Device,          //
                                               vk::raii::PhysicalDevice,  //
                                               vk::raii::Instance,        //
                                               vk::raii::Context,         //
                                               user_data>>;

using descriptor_set_layout = dependent<vk::raii::DescriptorSetLayout,          //
                                        dependencies<vk::raii::DeviceMemory,    //
                                                     vk::raii::Buffer,          //
                                                     vk::raii::Device,          //
                                                     vk::raii::PhysicalDevice,  //
                                                     vk::raii::Instance,        //
                                                     vk::raii::Context,         //
                                                     user_data>>;

using descriptor_set = dependent<vk::raii::DescriptorSet,                //
                                 dependencies<vk::raii::DescriptorPool,  //
                                              vk::raii::Device,          //
                                              vk::raii::PhysicalDevice,  //
                                              vk::raii::Instance,        //
                                              vk::raii::Context,         //
                                              user_data>>;
}
}