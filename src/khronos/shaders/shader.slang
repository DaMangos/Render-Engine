struct input
{
    float2 pos;
    float3 colour;
};

struct output
{
    float4 pos : SV_Position;
    float3 colour;
};

[shader("vertex")]
output vert_main(input in)
{
  output output;
  output.pos    = float4(in.pos, 0.0, 1.0);
  output.colour = in.colour;
  return output;
}

[shader("fragment")]
float4 frag_main(output in)
: SV_TARGET
{
  return float4(in.colour, 1.0);
}

