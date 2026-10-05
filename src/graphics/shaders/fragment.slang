struct vertex_data
{
    float3 pos;
    float3 colour : Colour;
};

struct vertex_output
{
    float4 pos    : SV_Position;
    float3 colour : Colour;
};

struct camera_data
{
    float3 focal_point;
    float3 lens_normal;
    float3 lens_origin;
    float3 lens_x_basis;
    float3 lens_y_basis;
    float2 screen_size;
    float  render_distance;
    float  focal_length;
};

ConstantBuffer<camera_data> camera;

[shader("mesh")]
[numthreads(32, 1, 1)]
[outputtopology("triangle")]
void main(out indices uint3 triangles[1], out vertices vertex_output vertices[3], uint3 dispatch_thread_id: SV_DispatchThreadID)

{
  // // find the plane-line intersection

  // uint vertex_count   = 4;
  // uint triangle_count = 2;

  // SetMeshOutputCounts(vertex_count, triangle_count);

  // const float3 ray = camera.focal_point - vertex.pos;

  // const float lens_normal_dot_ray = dot(camera.lens_normal, ray);

  // if(lens_normal_dot_ray == 0)
  //   return {};

  // const float t = dot(camera.lens_origin - vertex.pos, camera.lens_normal) / lens_normal_dot_ray;

  // const float3 intersection = vertex.pos + (t * ray);

  // // change of bases

  // const float2 normalized_screen_coord = {
  //   dot(intersection - camera.lens_origin, camera.lens_x_basis) / camera.screen_size.x,
  //   dot(intersection - camera.lens_origin, camera.lens_y_basis) / camera.screen_size.y,
  // };

  // const float normalized_vertex_distance_from_focal_point = (length(ray) - camera.focal_length) / camera.render_distance;

  // return {
  //   { normalized_screen_coord, normalized_vertex_distance_from_focal_point, 1.0 },
  //   vertex.colour
  // };
}

