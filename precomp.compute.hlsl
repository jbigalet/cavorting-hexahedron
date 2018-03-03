#define CHUNK_SIZE 32

static const float dim = (float)(CHUNK_SIZE);
static const uint precomp_dim = (CHUNK_SIZE)+3;


RWBuffer<float> precomp : register(u0);

cbuffer ubo {
    float time;
    float3 corner;
}



float metaball(float3 pos, float3 center) {
    float3 d = pos-center;
    return 1.f/dot(d, d);
}


float val(float3 p) {
    /* return dot(p,p) <= radius2 ? 1.f : 0.f; */

    float m = metaball(p, float3(0,0,0));

    /* float s = fmod(time/5, 1.4f)-0.7; */
    /* m += metaball(p, float3(s,s,s)); */

    /* m += metaball(p, float3(fmod(time/2, 1.4f)-0.7, -(fmod(time/3, 1.4f)-0.7), fmod(time/4.f, 1.4f)-0.7)); */

    /* m += metaball(p, float3(cos(time/4)*0.7, sin(time/3)*0.7, cos(-time/6)*0.7)); */

    for(uint i=5 ; i<=60 ; i++) {
        m += metaball(p, float3(i%5/2.f, i%2/1.f, i%7/3.f) - 1 + float3(cos(2*time/i), sin(5*time/i), cos(3*time/i))*0.7);
    }

    return m - 50.f;
}

[numthreads(precomp_dim, 1, 1)]
void main(int3 id: SV_DispatchThreadID) {
    float3 pos = corner + float3(id - 1)/dim;
    precomp[id.x + precomp_dim*id.y + precomp_dim*precomp_dim*id.z] = val(pos);
}
