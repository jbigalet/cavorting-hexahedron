RWBuffer<float> precomp : register(u0);
static const uint precomp_dim = 131;

cbuffer ubo {
    float time;
}



float metaball(float3 pos, float3 center) {
    float3 d = pos-center;
    return 1.f/dot(d, d);
}


static const float dim = 128.f;

float val(float3 pos) {
    float3 p = pos/dim - float3(0.5, 0.5, 0.5);
    /* return dot(p,p) <= radius2 ? 1.f : 0.f; */

    float m = metaball(p, float3(0,0,0));

    float s = fmod(time/5, 1.4f)-0.7;
    m += metaball(p, float3(s,s,s));

    m += metaball(p, float3(fmod(time/2, 1.4f)-0.7, -(fmod(time/3, 1.4f)-0.7), fmod(time/4.f, 1.4f)-0.7));

    m += metaball(p, float3(cos(time/4)*0.7, sin(time/3)*0.7, cos(-time/6)*0.7));

    return m - 50.f;
}

[numthreads(131, 1, 1)]
void main(uint3 id: SV_DispatchThreadID) {
	float3 pos = id;
	pos -= float3(1, 1, 1);
	precomp[id.x + precomp_dim*id.y + precomp_dim*precomp_dim*id.z] = val(pos);
}
