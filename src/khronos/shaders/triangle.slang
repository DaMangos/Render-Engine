static float2 positions[3] = { float2(0.0, -0.5), float2(0.5, 0.5), float2(-0.5, 0.5) };

static float3 colours[3] = { float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0) };

struct vertex_output
{
    float3 colours;
    float4 sv_position : SV_Position;
};

[shader("vertex")]
vertex_output vert_main(uint vertex_id: SV_VertexID)
{
  return {
    colours[vertex_id],
    { positions[vertex_id], 0.0, 1.0 },
  };
}

[shader("fragment")]
float4 frag_main(vertex_output in)
: SV_Target
{
  return { in.colours, 1.0 };
}

