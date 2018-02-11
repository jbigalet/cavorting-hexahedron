cbuffer ubo {
    matrix camproj;
}

struct VOut {
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

VOut VS(float3 position : POSITION, float3 color : COLOR) {
/* VOut VS(float3 position : POSITION) { */
    VOut output;
    output.position = mul(camproj, float4(position, 1));
    /* output.position = float4(position, 1); */
    output.color = color;
    return output;
}

float4 PS(float4 position : SV_POSITION, float3 color : COLOR) : SV_TARGET {
/* float4 PS(float4 position : SV_POSITION) : SV_TARGET { */
    /* return float4(1,0,0,1); */
    return float4(color, 1.f);
}
