
// // #include <khronos/library.hpp>
// #include <khronos/present_window.hpp>

// khronos::present_window::present_window(library const & library, glfw::window && window)
// : glfw::window(std::move(window))
// {
//   auto const surface_dependencies = dependency_builder<vk::raii::SurfaceKHR, vk::raii::Instance>{}  //
//                                       .add_dependency(library.instance);

//   surface.reset(surface_dependencies, glfw::window::create_surface(*library.instance));
// }