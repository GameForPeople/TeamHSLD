Shader "Custom/SectionShader" {
	Properties{
		_MainTex("Texture", 2D) = "white" {}
		_BumpMap("Bumpmap", 2D) = "bump" {}
		_Color("Color", Color) = (1, 1, 1, 1)
	}
		SubShader{
		Tags{ "RenderType" = "Opaque" }
		Cull Off
		CGPROGRAM
#pragma surface surf Lambert
	struct Input {
		float2 uv_MainTex;
		float2 uv_BumpMap;
		float3 worldPos;
	};

	sampler2D _MainTex;
	sampler2D _BumpMap;

	fixed4 _Color;

	void surf(Input IN, inout SurfaceOutput o) {
		clip(frac((IN.worldPos.y + IN.worldPos.z*0.1) * 5) - 0.5);

		o.Albedo = tex2D(_MainTex, IN.uv_MainTex).rgb * _Color;

		o.Normal = UnpackNormal(tex2D(_BumpMap, IN.uv_BumpMap));
	}
	ENDCG
	}
		Fallback "Diffuse"
}