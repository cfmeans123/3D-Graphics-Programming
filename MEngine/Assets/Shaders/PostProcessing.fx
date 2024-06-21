//this is where post processing effects are handled and sent to the GPU

// Settings - Overscan
#define OVERSCAN_PERCENTAGE     0.02

// Settings - Bloom
#define BLOOM_OFFSET            0.0015
#define BLOOM_STRENGTH          0.8

// Settings - Blur
#define BLUR_MULTIPLIER         1.05
#define BLUR_STRENGTH           0.2
#define BLUR_OFFSET             0.003

// Settings - Grayscale
#define GRAYSCALE_INTENSITY     0
#define GRAYSCALE_GLEAM         0
#define GRAYSCALE_LUMINANCE     1
#define GRAYSCALE_LUMA          0

// Settings - Blacklevel
#define BLACKLEVEL_FLOOR float3(0.05, 0.05, 0.05)

//Settings - Scanlines
#define SCANLINE_FACTOR 0.3

//Settings - Film Grain Effect
#define GRAIN_INTENSITY         0.02
//Lookup Table
#define g0  0.151015505647689
#define g1 -0.5303572634357367
#define g2  1.365020122861334
#define h0  0.132089632343748
#define h1 -0.760732499132376

cbuffer PostProcessBuffer : register(b0)
{
    int mode;
    float params0;
    float params1;
    float params2;
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float2 transformCurve(float2 uv)
{
    uv -= 0.5; // offcenter screen
    float r = (uv.x * uv.x + uv.y * uv.y); // get ratio
    uv *= 4.2 + r; // apply ratio
    uv *= 0.25; // zoom
    uv += 0.5; // move back to center
    return uv;
}

float4 overscan(float4 color, in float2 screenuv, out float2 uv)
{
  // Overscan Region
    uv = screenuv;
    uv -= 0.5;
    uv *= 1 / (1 - OVERSCAN_PERCENTAGE);
    uv += 0.5;

  // The scaler was found through trial and error, but the result seems to work
  // for all the color schemes I tried. I think that Background should be
  // providing the same values as if the texture was sampled, but it doesn't seem
  // to do so. On the otherhand, I can also see a need for the actual Background
  // to be passed into the shader. Maybe this just needs more documentation.
  // SHADERed seems to have a problem with this in the preview window, but the
  // Pixel Inspect shows the right thing.
    if (uv.x < 0.000 | uv.y < 0.000)
    {
        return saturate(float4(color.r, color.g, color.b, 0) * 0.1);
    }
    if (uv.y > 1.000 | uv.y > 1.000)
    {
        return saturate(float4(color.r, color.g, color.b, 0) * 0.1);
    }
  
    return float4(color);
}

float3 bloom(float3 color, float2 uv)
{
    float3 bloom = color - textureMap0.Sample(textureSampler, uv + float2(-BLOOM_OFFSET, 0)).rgb;
    float3 bloom_mask = bloom * BLOOM_STRENGTH;
  //return bloom_mask;
    return saturate(color + bloom_mask);
}

static const float blurWeights[9] = { 0.0, 0.092, 0.081, 0.071, 0.061, 0.051, 0.041, 0.031, 0.021 };

float3 blurH(float3 c, float2 uv)
{
    float3 screen =
    textureMap0.Sample(textureSampler, uv).rgb * 0.102;
    for (int i = 1; i < 9; i++)
        screen +=
    textureMap0.Sample(textureSampler, uv + float2(i * BLUR_OFFSET, 0)).rgb * blurWeights[i];
    for (int i = 1; i < 9; i++)
        screen +=
    textureMap0.Sample(textureSampler, uv + float2(-i * BLUR_OFFSET, 0)).rgb * blurWeights[i];
    return screen * BLUR_MULTIPLIER;
}

float3 blurV(float3 c, float2 uv)
{
    float3 screen =
    textureMap0.Sample(textureSampler, uv).rgb * 0.102;
    for (int i = 1; i < 9; i++)
    {
        screen += textureMap0.Sample(textureSampler, uv + float2(0, i * BLUR_OFFSET)).rgb * blurWeights[i];
    }
    for (int j = 1; j < 9; j++)
    {
        screen += textureMap0.Sample(textureSampler, uv + float2(0, -j * BLUR_OFFSET)).rgb * blurWeights[j];
    }
    return screen * BLUR_MULTIPLIER;
}

float3 blur(float3 color, float2 uv)
{
    float3 blur = (blurH(color, uv) + blurV(color, uv)) / 2 - color;
    float3 blur_mask = blur * BLUR_STRENGTH;
  //return blur_mask;
    return saturate(color + blur_mask);
}

float3 rgb2intensity(float3 c)
{
    return (c.r + c.g + c.b) / 3.0;
}

float3 rgb2luminance(float3 c)
{
    return 0.2989 * c.r + 0.5866 * c.g + 0.1145 * c.b;
}

float3 grayscale(float3 color)
{
    color.rgb = saturate(rgb2luminance(color.rgb));
    return color;
}

float3 blacklevel(float3 color)
{
    color.rgb -= BLACKLEVEL_FLOOR;
    color.rgb = saturate(color.rgb);
    color.rgb += BLACKLEVEL_FLOOR;
    return saturate(color);
}

float3 refreshLines(float3 color, float2 uv)
{
    float timeOver = fmod(params0 / 5, 1.5) - 0.5;
    float refreshLineColorTint = timeOver - uv.y;
    if (uv.y > timeOver && uv.y - 0.03 < timeOver)
        color.rgb += (refreshLineColorTint * 2.0);
    return saturate(color);
}

float squareWave(float y)
{
    //Divide y by scale factor if included
    return 1 - (floor(y) % 2) * SCANLINE_FACTOR;
}

float3 scanlines(float3 color, float2 pos)
{
    float wave = squareWave(pos.y);

  // TODO:GH#3929 make this configurable.
  // Remove the && false to draw scanlines everywhere.
    if (length(color.rgb) < 0.2 && false)
    {
        return saturate(color + wave * 0.1);
    }
    else
    {
        return saturate(color * wave);
    }
}

float3 tint(float3 color)
{
    //float3 for ~amber color
    color.rgb *= float3(1.0, 0.7, 0.0);
    return saturate(color);
}

float permute(float x)
{
    x *= (34 * x + 1);
    return 289 * frac(x * 1 / 289.0);
}

float rand(inout float state)
{
    state = permute(state);
    return frac(state / 41.0);
}

float3 grain(float3 color, float2 uv)
{
    float3 m = float3(uv, params0 % 5 / 5) + 1.0;
    float state = permute(permute(m.x) + m.y) + m.z;
  
    float p = 0.95 * rand(state) + 0.025;
    float q = p - 0.5;
    float r2 = q * q;
  
    float grain = q * (g2 + (g1 * r2 + g0) / (r2 * r2 + h1 * r2 + h0));
    color.rgb += GRAIN_INTENSITY * grain;

    return saturate(color);
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 1.0f;
    //None
    if (mode == 0)
    {
        finalColor = textureMap0.Sample(textureSampler, input.texCoord);
    }
    //monochrome
    else if (mode == 1)
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = (color.r + color.g + color.b) / 3.0f;
    }
    //Invert
    else if (mode == 2)
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = 1.0f - color;
    }
    //Mirror
    else if (mode == 3)
    {
        float2 texCoord = input.texCoord;
        texCoord.x *= params0;
        texCoord.y *= params1;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    //Blur
    else if (mode == 4)
    {
        float u = input.texCoord.x;
        float v = input.texCoord.y;
        finalColor =
        textureMap0.Sample(textureSampler, float2(u, v))
        + textureMap0.Sample(textureSampler, float2(u + params0, v))
        + textureMap0.Sample(textureSampler, float2(u - params0, v))
        + textureMap0.Sample(textureSampler, float2(u, v + params1))
        + textureMap0.Sample(textureSampler, float2(u, v - params1))
        + textureMap0.Sample(textureSampler, float2(u + params0, v + params1))
        + textureMap0.Sample(textureSampler, float2(u + params0, v - params1))
        + textureMap0.Sample(textureSampler, float2(u - params0, v + params1))
        + textureMap0.Sample(textureSampler, float2(u - params0, v - params1));

        finalColor *= 0.11111111f;
    }
    //CombineTwo
    else if (mode == 5)
    {
        float4 color0 = textureMap0.Sample(textureSampler, input.texCoord);
        float4 color1 = textureMap1.Sample(textureSampler, input.texCoord);
        finalColor = (color0 + color1) * 0.5f;
        //Alpha Blend
        //finalColor - (1.0f - color1.a) * color0) + (color1.a * color1);
    }
    //Wave
    else if (mode == 6)
    {
        float waveValue = input.texCoord.x * (3.14159f * 20.0f);
        float2 texCoord = input.texCoord;
        texCoord.y += sin(waveValue) * params0;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    //Chromatic Aberration
    else if (mode == 7)
    {
        float2 distortion = float2(params0, -params1);
        float4 redChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
        float4 blueChannel = textureMap0.Sample(textureSampler, input.texCoord);
        float4 greenChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord);
        finalColor = float4(redChannel.r, greenChannel.g, blueChannel.b, 1.0f);
    }
    else if (mode == 8)
    {
        float4 pos = input.position;
        float2 uv = input.texCoord;
        
        uv = transformCurve(uv);

        // TODO: add monitor visuals and make colors static consts
        // Outer Box
        if (uv.x < -0.025 || uv.y < -0.025)
            return float4(0.00, 0.00, 0.00, 1.0);
        if (uv.x > 1.025 || uv.y > 1.025)
            return float4(0.00, 0.00, 0.00, 1.0);
        // Bezel
        if (uv.x < -0.015 || uv.y < -0.015)
            return float4(0.03, 0.03, 0.03, 1.0);
        if (uv.x > 1.015 || uv.y > 1.015)
            return float4(0.03, 0.03, 0.03, 1.0);
        // Screen Border
        if (uv.x < -0.001 || uv.y < -0.001)
            return float4(0.00, 0.00, 0.00, 1.0);
        if (uv.x > 1.001 || uv.y > 1.001)
            return float4(0.00, 0.00, 0.00, 1.0);
        
        float4 color = float4(1, 0, 1, -1);

        // We need to track two different uv's. The screen uv is effectively
        // the CRT glass. We also want to track uv for when we sample from the
        // texture.
        float2 screenuv = uv;
        // Modifies uv while screenuv remains the same.
        //color = overscan(color, screenuv, uv);

        // This must come after we've adjusted the uv for OVERSCAN.
        if (color.a < 0)
        {
            color = textureMap0.Sample(textureSampler, uv);
        }  
        color.rgb = bloom(color.rgb, uv); 
        color.rgb = blur(color.rgb, uv);
        color.rgb = grayscale(color.rgb);
        color.rgb = blacklevel(color.rgb);
        color.rgb = refreshLines(color.rgb, screenuv);       
        color.rgb = scanlines(color.rgb, pos);
        //color.rgb = tint(color.rgb);
        color.rgb = grain(color.rgb, screenuv);
        
        return color;
    }
        return finalColor;
}