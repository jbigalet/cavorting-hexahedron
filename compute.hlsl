#define CHUNK_SIZE 32

static const float dim = (float)(CHUNK_SIZE);
static const uint precomp_dim = (CHUNK_SIZE)+3;


struct Vertex {
    float3 position;
    float3 color;
};

struct Triangle {
    Vertex e[3];
};

AppendStructuredBuffer<Triangle> uav : register(u0);
RWBuffer<float> precomp : register(u1);

cbuffer ubo {
    float time;
    float3 corner;
}



/* void append_vertex(float3 p, float3 c) { */
/*     Vertex v; */
/*     v.position = p; */
/*     v.color = c; */
/*     uav.Append(v); */
/* } */

void append_triangle(float3 a, float3 b, float3 c, float3 color) {
    // flat shading

    /* append_vertex(a, color); */
    /* append_vertex(b, color); */
    /* append_vertex(c, color); */
    Triangle t;
    t.e[0].position = b + corner;
    t.e[1].position = a + corner;
    t.e[2].position = c + corner;

    float3 norm = normalize(cross(b-a, c-a));
    color = norm;
    t.e[0].color = color;
    t.e[1].color = color;
    t.e[2].color = color;

    uav.Append(t);
}

void append_triangle_with_norms(float3 a, float3 b, float3 c, float3 na, float3 nb, float3 nc) {
    /* append_vertex(a, color); */
    /* append_vertex(b, color); */
    /* append_vertex(c, color); */
    Triangle t;
    t.e[0].position = b + corner;
    t.e[1].position = a + corner;
    t.e[2].position = c + corner;

    t.e[0].color = nb;
    t.e[1].color = na;
    t.e[2].color = nc;

    uav.Append(t);
}

void append_aabb(float3 corner, float size=1.f/dim, float3 color=float3(1, 0, 0)) {
    append_triangle(corner + float3(0.f, size, size), corner + float3(0.f, 0.f, size), corner + float3(size, size, size), color); // Z+
    append_triangle(corner + float3(size, size, size), corner + float3(0.f, 0.f, size), corner + float3(size, 0.f, size), color);

    append_triangle(corner + float3(0.f, 0.f, 0.f), corner + float3(0.f, size, 0.f), corner + float3(size, size, 0.f), color); // Z-
    append_triangle(corner + float3(0.f, 0.f, 0.f), corner + float3(size, size, 0.f), corner + float3(size, 0.f, 0.f), color);

    append_triangle(corner + float3(size, 0.f, size), corner + float3(size, 0.f, 0.f), corner + float3(size, size, size), color); // X+
    append_triangle(corner + float3(size, size, size), corner + float3(size, 0.f, 0.f), corner + float3(size, size, 0.f), color);

    append_triangle(corner + float3(0.f, 0.f, 0.f), corner + float3(0.f, 0.f, size), corner + float3(0.f, size, size), color); // X-
    append_triangle(corner + float3(0.f, 0.f, 0.f), corner + float3(0.f, size, size), corner + float3(0.f, size, 0.f), color);

    append_triangle(corner + float3(0.f, size, 0.f), corner + float3(0.f, size, size), corner + float3(size, size, size), color); // Y+
    append_triangle(corner + float3(0.f, size, 0.f), corner + float3(size, size, size), corner + float3(size, size, 0.f), color);

    append_triangle(corner + float3(0.f, 0.f, size), corner + float3(0.f, 0.f, 0.f), corner + float3(size, 0.f, size), color); // Y-
    append_triangle(corner + float3(size, 0.f, size), corner + float3(0.f, 0.f, 0.f), corner + float3(size, 0.f, 0.f), color);
}


static const uint mc_lut[256][3] = {
    { 0u, 0u, 0u },
    { 528417u, 0u, 0u },
    { 420129u, 0u, 0u },
    { 2768920642u, 100u, 0u },
    { 825921u, 0u, 0u },
    { 1778813058u, 200u, 0u },
    { 1762027810u, 200u, 0u },
    { 2804314731u, 43528804u, 0u },
    { 441189u, 0u, 0u },
    { 1705512994u, 236u, 0u },
    { 3968788770u, 104u, 0u },
    { 3993411651u, 62546024u, 0u },
    { 3977173314u, 200u, 0u },
    { 2209495911u, 34388768u, 0u },
    { 3398707491u, 53406016u, 0u },
    { 3993954690u, 200u, 0u },
    { 734337u, 0u, 0u },
    { 3498905634u, 176u, 0u },
    { 3523635490u, 176u, 0u },
    { 3516602791u, 54794672u, 0u },
    { 3524041282u, 176u, 0u },
    { 1791798483u, 27405512u, 0u },
    { 2760586371u, 52858980u, 0u },
    { 2980473195u, 3402406565u, 11472u },
    { 3524188522u, 176u, 0u },
    { 3498905635u, 62282416u, 0u },
    { 2779400299u, 46997740u, 0u },
    { 3517780075u, 4006427629u, 10672u },
    { 3977173315u, 46997704u, 0u },
    { 3985779815u, 4006427569u, 13512u },
    { 3398707491u, 2200889665u, 11472u },
    { 3399297459u, 53406160u, 0u },
    { 1010853u, 0u, 0u },
    { 2996310050u, 244u, 0u },
    { 2979727462u, 244u, 0u },
    { 1138245043u, 17202560u, 0u },
    { 2996607554u, 244u, 0u },
    { 3364657699u, 64694656u, 0u },
    { 4103040035u, 52858980u, 0u },
    { 3383131499u, 3420140277u, 11392u },
    { 2996754794u, 244u, 0u },
    { 1705512995u, 64694764u, 0u },
    { 2981059311u, 46275316u, 0u },
    { 4110859075u, 2211975405u, 15792u },
    { 3977173315u, 64694728u, 0u },
    { 2209495911u, 2785852193u, 15792u },
    { 2970290243u, 3420140233u, 15348u },
    { 2210085879u, 34388912u, 0u },
    { 3533512870u, 244u, 0u },
    { 4111093795u, 64819364u, 0u },
    { 1732392067u, 27227168u, 0u },
    { 3516731494u, 244u, 0u },
    { 3533512871u, 52859124u, 0u },
    { 4147082443u, 1788828265u, 2212u },
    { 1732392067u, 1100968993u, 12904u },
    { 1732687051u, 27227240u, 0u },
    { 3533512871u, 62282484u, 0u },
    { 4111093795u, 1809649829u, 15204u },
    { 3523353219u, 3517903977u, 15348u },
    { 4111388779u, 64819436u, 0u },
    { 3533512871u, 1136021749u, 13036u },
    { 3968784675u, 592218314u, 2815573456u },
    { 3498909731u, 599738614u, 1126314732u },
    { 4147082442u, 236u, 0u },
    { 868041u, 0u, 0u },
    { 4213706786u, 208u, 0u },
    { 4213598498u, 208u, 0u },
    { 2768920643u, 55554660u, 0u },
    { 4178549314u, 208u, 0u },
    { 4169992323u, 65439952u, 0u },
    { 4178549315u, 26888400u, 0u },
    { 2806669523u, 4187651833u, 6756u },
    { 4213619558u, 208u, 0u },
    { 1705512995u, 55554796u, 0u },
    { 3968788771u, 55554664u, 0u },
    { 3993411651u, 3417989225u, 13560u },
    { 1733227771u, 26490092u, 0u },
    { 4169675811u, 1743145169u, 16108u },
    { 2760127523u, 2807015889u, 16108u },
    { 3994001619u, 62546168u, 0u },
    { 4195332738u, 176u, 0u },
    { 602160843u, 9125440u, 0u },
    { 4195332739u, 26888368u, 0u },
    { 4213447115u, 4187651685u, 10672u },
    { 2980584003u, 47159936u, 0u },
    { 4170160674u, 176u, 0u },
    { 2980584003u, 584030849u, 6564u },
    { 2980731495u, 47159972u, 0u },
    { 4195332739u, 62282416u, 0u },
    { 602160843u, 1753955905u, 15204u },
    { 4195332739u, 1787959985u, 15268u },
    { 4178555459u, 1117458610u, 1806973860u },
    { 1711689859u, 3015077041u, 16108u },
    { 602308335u, 9125476u, 0u },
    { 2768914499u, 1139724526u, 2193894648u },
    { 4204772262u, 176u, 0u },
    { 4214189222u, 208u, 0u },
    { 2996310051u, 55554804u, 0u },
    { 4103040035u, 55554660u, 0u },
    { 1138245043u, 3372645761u, 13560u },
    { 3517791851u, 64694592u, 0u },
    { 4169992323u, 2816903377u, 15792u },
    { 3517791851u, 601557057u, 6644u },
    { 4128205955u, 2175164518u, 3555212904u },
    { 2996754795u, 55554804u, 0u },
    { 1705512995u, 3420137965u, 13560u },
    { 2981059311u, 3401718517u, 13560u },
    { 2981035371u, 1120148214u, 3555503232u },
    { 1733227771u, 2777953517u, 15792u },
    { 4169675811u, 1743145170u, 4140465900u },
    { 3552394991u, 3968088898u, 4140756000u },
    { 4128205955u, 2213470445u, 13560u },
    { 3399461367u, 52963832u, 0u },
    { 602706171u, 4103044673u, 2212u },
    { 1743158779u, 3364254409u, 8224u },
    { 1138540027u, 17202632u, 0u },
    { 1762142531u, 1788828325u, 15864u },
    { 4170139815u, 65439988u, 0u },
    { 1778815107u, 2212471034u, 563616244u },
    { 4187986534u, 244u, 0u },
    { 3399461367u, 1797794297u, 15204u },
    { 1138733815u, 2768928202u, 3986067488u },
    { 1809234935u, 4127341858u, 4214190720u },
    { 4178555459u, 1136611573u, 6892u },
    { 2817162747u, 4178067074u, 3986659648u },
    { 3968784675u, 601852153u, 10740u },
    { 4222885922u, 244u, 0u },
    { 1015533u, 0u, 0u },
    { 1031661u, 0u, 0u },
    { 4156035106u, 248u, 0u },
    { 4155926818u, 248u, 0u },
    { 2768920643u, 66026340u, 0u },
    { 4156332610u, 248u, 0u },
    { 3364657699u, 66026368u, 0u },
    { 1762027811u, 66026440u, 0u },
    { 2804314731u, 4002951781u, 16116u },
    { 4187995494u, 104u, 0u },
    { 4121909611u, 33818872u, 0u },
    { 602570487u, 8830628u, 0u },
    { 2785286787u, 2779752809u, 16116u },
    { 4111438027u, 64377080u, 0u },
    { 3420241655u, 1712101733u, 2176u },
    { 2804923851u, 1117008629u, 10528u },
    { 2804904699u, 43528948u, 0u },
    { 4156241026u, 248u, 0u },
    { 3498905635u, 66026416u, 0u },
    { 3523635491u, 66026416u, 0u },
    { 3516602791u, 4014217649u, 16116u },
    { 3524041283u, 66026416u, 0u },
    { 1791798483u, 3986828489u, 16116u },
    { 1762027811u, 4006420681u, 16116u },
    { 3516558439u, 1799788978u, 4225692872u },
    { 4121909611u, 46997752u, 0u },
    { 3498905635u, 1809210033u, 16116u },
    { 602570487u, 2156314277u, 11472u },
    { 4204772775u, 2768677226u, 3008001088u },
    { 4111438027u, 2211860729u, 11472u },
    { 3005953227u, 3381801506u, 4225547620u },
    { 4111253059u, 600969466u, 3007856036u },
    { 3005953227u, 3420091045u, 16116u },
    { 4204768422u, 236u, 0u },
    { 4204768423u, 33818860u, 0u },
    { 4170106983u, 65734892u, 0u },
    { 2970296387u, 1743460709u, 15352u },
    { 4204768423u, 52859116u, 0u },
    { 3364657699u, 4006247297u, 16048u },
    { 4170106983u, 1139476717u, 12904u },
    { 3382483559u, 1724389762u, 4024860336u },
    { 1791388839u, 28290148u, 0u },
    { 1791388839u, 565161061u, 8256u },
    { 4170165282u, 104u, 0u },
    { 1791241347u, 28290112u, 0u },
    { 1695333955u, 1743460857u, 11428u },
    { 2979735911u, 1731172858u, 570843336u },
    { 4169959491u, 65734856u, 0u },
    { 4195331202u, 200u, 0u },
    { 2210216955u, 34241488u, 0u },
    { 3506579779u, 3533632677u, 15352u },
    { 3993115011u, 3552387173u, 15352u },
    { 3516897775u, 54794744u, 0u },
    { 2210216955u, 1107983313u, 12904u },
    { 4014931667u, 3498734758u, 3383564832u },
    { 602743035u, 3968794242u, 3382970468u },
    { 3382483559u, 1743591889u, 15352u },
    { 4195333251u, 2779752869u, 6504u },
    { 1116997799u, 2817337810u, 1705663224u },
    { 602422995u, 8830592u, 0u },
    { 4178555970u, 104u, 0u },
    { 1139028731u, 4204166758u, 3524239524u },
    { 3542767906u, 248u, 0u },
    { 3498909731u, 590317817u, 4296u },
    { 1029321u, 0u, 0u },
    { 4147084234u, 208u, 0u },
    { 3552394735u, 33818824u, 0u },
    { 3552394735u, 26888392u, 0u },
    { 2174226535u, 3420385957u, 13556u },
    { 1138146799u, 17644904u, 0u },
    { 1753756707u, 1800362705u, 15852u },
    { 1138146799u, 554515817u, 6564u },
    { 4121918315u, 1779243730u, 1721149824u },
    { 3516701035u, 55531976u, 0u },
    { 3516701035u, 592402889u, 8256u },
    { 1799795411u, 4102471157u, 10528u },
    { 2175275115u, 1809472166u, 3410408692u },
    { 3516743014u, 64u, 0u },
    { 3516406051u, 55531904u, 0u },
    { 1137851815u, 17644832u, 0u },
    { 3533520294u, 128u, 0u },
    { 3993985203u, 62693620u, 0u },
    { 3364655139u, 3015486641u, 15852u },
    { 3993985203u, 599564533u, 6564u },
    { 1724279219u, 3005492290u, 4156242888u },
    { 2980323435u, 4006533761u, 11508u },
    { 1791945975u, 27405548u, 0u },
    { 1791982515u, 2175282414u, 2760268352u },
    { 4121918315u, 1788549809u, 6564u },
    { 4120293991u, 4146609609u, 8368u },
    { 1730566859u, 3402593014u, 1082951856u },
    { 2211982583u, 4121738698u, 2760853608u },
    { 4137071170u, 176u, 0u },
    { 4111143043u, 64377008u, 0u },
    { 2979741030u, 32u, 0u },
    { 2768914499u, 1120882933u, 8368u },
    { 736677u, 0u, 0u },
    { 2804626131u, 43971248u, 0u },
    { 2804626131u, 580842161u, 8256u },
    { 2961603363u, 3015486701u, 13008u },
    { 3402413235u, 2979982574u, 2164985956u },
    { 3977173571u, 3533632721u, 11428u },
    { 591536339u, 3551966314u, 2996644332u },
    { 598874607u, 4014685106u, 1762578640u },
    { 2986769258u, 208u, 0u },
    { 3516314215u, 2806667721u, 13488u },
    { 1732979155u, 3382490278u, 2164398440u },
    { 2980911819u, 46275280u, 0u },
    { 2175275115u, 1800084145u, 13008u },
    { 1732932787u, 26490020u, 0u },
    { 2979735911u, 1712134609u, 2176u },
    { 3498912802u, 64u, 0u },
    { 863361u, 0u, 0u },
    { 3993956994u, 164u, 0u },
    { 2804036163u, 43971104u, 0u },
    { 3993395235u, 62693476u, 0u },
    { 3977179714u, 100u, 0u },
    { 2209626987u, 34241344u, 0u },
    { 3968784930u, 164u, 0u },
    { 1778815107u, 2174673133u, 2148u },
    { 973413u, 0u, 0u },
    { 3398871399u, 52963688u, 0u },
    { 1730566859u, 3364516517u, 4128u },
    { 1778823810u, 32u, 0u },
    { 438849u, 0u, 0u },
    { 2768918850u, 128u, 0u },
    { 678177u, 0u, 0u },
    { 270369u, 0u, 0u },
    { 0u, 0u, 0u },
};


static const float size = 10.f;
static const float voxel_size = size/dim;

static const float radius = 0.4;
static const float radius2 = radius*radius;



float metaball(float3 pos, float3 center) {
    float3 d = pos-center;
    return 1.f/dot(d, d);
}


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


[numthreads(8, 8, 8)]
void main(uint3 id: SV_DispatchThreadID) {

    uint precomp_idx = (id.x+1) + precomp_dim*(id.y+1) + precomp_dim*precomp_dim*(id.z+1);

    float vals[8];
    uint bitfield = 0u;
    for(uint x=0 ; x<=1 ; x++)
        for(uint y=0 ; y<=1 ; y++)
            for(uint z=0 ; z<=1 ; z++) {
                uint idx = x + 2*y + 4*z;
                float v = precomp[precomp_idx + x + precomp_dim*y + precomp_dim*precomp_dim*z];
                vals[idx] = v;
                bitfield |= ((v > 0 ? 1 : 0) << idx);
            }

    /* if(bitfield != 0 && bitfield != 255) { */
    /*     append_aabb(id/dim); */
    /*     return; */
    /* } */

    float3 tri[3];
    float3 norms[3];
    uint tri_idx = 0;
    uint bytes_idx = 0;
    uint idx_in_byte = 0;
    uint byte = mc_lut[bitfield][0];
    int limit = byte % 4;
    byte >>= 2;

    [allow_uav_condition]  // =(
    while(limit > 0) {

        uint Ai = byte % 8;
        uint Bi = (byte >> 3) % 8;
        float3 A = id + float3(Ai%2, (Ai>>1)%2, (Ai>>2)%2);
        float3 B = id + float3(Bi%2, (Bi>>1)%2, (Bi>>2)%2);
        float t = vals[Bi]/(vals[Bi]-vals[Ai]);
        tri[tri_idx] = t*A + (1-t)*B;

        uint idx_A = precomp_idx + (Ai%2) + precomp_dim*((Ai>>1)%2) + precomp_dim*precomp_dim*((Ai>>2)%2);
        float3 nA = -normalize(float3(
            precomp[idx_A + 1] - precomp[idx_A - 1],
            precomp[idx_A + precomp_dim] - precomp[idx_A - precomp_dim],
            precomp[idx_A + precomp_dim*precomp_dim] - precomp[idx_A - precomp_dim*precomp_dim]
        ));

        uint idx_B = precomp_idx + (Bi%2) + precomp_dim*((Bi>>1)%2) + precomp_dim*precomp_dim*((Bi>>2)%2);
        float3 nB = -normalize(float3(
            precomp[idx_B + 1] - precomp[idx_B - 1],
            precomp[idx_B + precomp_dim] - precomp[idx_B - precomp_dim],
            precomp[idx_B + precomp_dim*precomp_dim] - precomp[idx_B - precomp_dim*precomp_dim]
        ));

        norms[tri_idx] = normalize(t*nA + (1-t)*nB);


        tri_idx++;
        idx_in_byte++;
        if(tri_idx == 3) {
            append_triangle_with_norms(tri[0]/dim, tri[1]/dim, tri[2]/dim, norms[0], norms[1], norms[2]);
            tri_idx = 0;
            limit--;
            if(limit == 0)
                break;
        }

        if(idx_in_byte < 5) {
            byte >>= 6;
        } else {
            idx_in_byte = 0;
            bytes_idx++;
            byte = mc_lut[bitfield][bytes_idx];
            limit += byte % 4;
            byte >>= 2;
        }
    }
}


