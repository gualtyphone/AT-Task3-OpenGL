#version 330

in highp vec4 color0;
in highp vec3 direction0;

uniform vec3 lightDirection;

out highp vec4 output;

void main()
{
	output = color0 * (1- dot(lightDirection, direction0));
}


//Other Shader with view

//#version 150
//	uniform vec3 hairColour;
//uniform vec3 specColour;

//out vec4 fragColour;

//in vec4 tangent;
//in vec4 lightDir;
//in vec4 viewDir;

//void main()
//{
//	vec3 T = normalize(vec3(tangent));
//	vec3 L = normalize(vec3(lightDir));
//	vec3 V = normalize(vec3(viewDir));

//	float TdotL = dot(T, L);
//	float TdotV = dot(T, V);

//	// The diffuse component

//	float kajiyaDiff=sin(acos(TdotL));
//	if (kajiyaDiff < 0)
//	{
//		kajiyaDiff=0;
//	}
//	kajiyaDiff=pow(kajiyaDiff, 10);

//	//The specular component

//	float kajiyaSpec=cos(abs(acos(TdotL)−acos(−TdotV)));
//	if (kajiyaSpec < 0)
//	{
//		kajiyaSpec=0;
//	}
//	kajiyaSpec=pow(kajiyaSpec, 100);

//	//The fragment color

//	fragColour= vec4(hairColour ∗ 0.6 + hairColour ∗ 0.5∗kajiyaDiff +
//					  specColour ∗ kajiyaSpec, 0.725);
//}
