struct Vertex {
    float3 position;
    float3 color;
};

struct Triangle {
    Vertex e[3];
};

StructuredBuffer<Triangle> data : register(t0);


cbuffer ubo {
    matrix camproj;
}

struct VOut {
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

float linear_depth(float z) {
    const float near = 0.1f;
    const float far = 200.f;

    return 2*far*near/(far+near-z*(far-near));
}

/* VOut VS(float3 position : POSITION, float3 color : COLOR) { */
VOut VS(uint InstanceId : SV_VertexID) {
/* VOut VS(float3 position : POSITION) { */

    Vertex v = data[InstanceId/3].e[InstanceId%3];

    float3 position = v.position;
    float3 color = v.color;

    VOut output;
    output.position = mul(camproj, float4(position, 1));
    output.color = color;
    return output;
}

float4 PS(float4 p : SV_POSITION, float3 color : COLOR) : SV_TARGET {
    float c = 1/p.w;  //p.z;
    c = 1 - pow(1-c, 3.f);
    /* c = clamp(linear_depth(c), 0, 1); */
    /* c = p.z > 0.9999 ? 1 : 0; */
    //return float4(float3(c,c,c), 1.f);
    return float4(color, 1.f);
}
