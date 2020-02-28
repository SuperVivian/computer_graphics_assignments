#include "stdafx.h"
#include "RayTracer.h"


RayTracer::RayTracer (SceneParser *s, int max_bounces, float cutoff_weight, bool shadows)
{
	this->scene = s;
	this->max_bounces = max_bounces;
	this->cutoff_weight = cutoff_weight;
	this->shadows = shadows;
}


RayTracer::~RayTracer ()
{
}

Vec3f RayTracer::traceRay (Ray &ray, float tmin, int bounces, float weight,float indexOfRefraction, Hit &hit)
{
	if (bounces < 0) return Vec3f (0, 0, 0);
	//weight    ���߶�����������ɫ�İٷֱȹ���

	Vec3f color (0, 0, 0);
	Vec3f  ambient = scene->getAmbientLight ();
	hit = Hit (FLT_MAX, NULL, Vec3f (0, 0, 0));

	if (scene->getGroup ()->intersect (ray, hit, tmin)) {//��Ҫ�ͳ�����������ཻ�����ߣ����������������������
		Vec3f point = hit.getIntersectionPoint ();
		Vec3f n = hit.getNormal ();
		//************************************************�ƹ���Ӱ����********************************************************//
		for (int i = 0; i < scene->getNumLights (); i++) {
			Vec3f lightColor, lightDir;	float distanceToLight;
			scene->getLight (i)->getIllumination (point, lightDir, lightColor, distanceToLight);
			lightDir.Normalize ();
			//�Ƿ������Ӱ���
			if (shadows) {
				Vec3f origin = point + n * 0.1f;
				Ray toLight = Ray (origin, lightDir);//�ٴӽ��㴦�����Դ����һ������
				Hit m_hit;
				bool isShaded = InShadow (toLight, m_hit, distanceToLight);
				RayTree::AddShadowSegment (toLight, 0, m_hit.getT ());
				if (isShaded) {					
					continue;//���Դ˹�Դ��Ӱ��
				}
			}
			//�ɼ���Ĺ�Դ��Ӱ��
			color += hit.getMaterial ()->Shade (ray, hit, lightDir, lightColor);
		}
		color += (ambient * hit.getMaterial ()->getDiffuseColor ());

		//************************************************�������********************************************************//
		Vec3f reflectiveColor = hit.getMaterial ()->getReflectiveColor ();
		// ����Ƿ���Ҫ����
		if (bounces && (reflectiveColor != Vec3f (0, 0, 0)))
		{
			Vec3f mirror = mirrorDirection (n, ray.getDirection ());
			Ray r = Ray (point, mirror);			Hit tmp_h;		
			Vec3f v = traceRay (r, EPSILON, bounces - 1, indexOfRefraction,weight, tmp_h);
			RayTree::AddReflectedSegment (r, 0, tmp_h.getT ());
			Vec3f reflection = reflectiveColor * v; //����ϵ��*�õ��ķ�����ɫ		
			color += reflection;
		}

		//************************************************�������********************************************************//
		Vec3f transparentColor = hit.getMaterial ()->getTransparentColor ();
		if (indexOfRefraction && (transparentColor != Vec3f (0, 0, 0)))
		{
			bool inside = ray.getDirection ().Dot3 (n) > 0;
			float new_index = hit.getMaterial ()->getIndexOfRefraction ();//��ò��ʵ�������
			if (inside == 1)
			{//������ڲ�
				new_index = 1;//�Ͱ�indexOfRefraction��Ϊ1
				n = -1 * n;//�Ͱѷ���ȡ��
			}
			Vec3f r_dir;
			if (transmittedDirection (n, ray.getDirection (), indexOfRefraction, new_index, r_dir)) 
			{
				Ray r = Ray (point, r_dir);				Hit tmp_h;
				Vec3f v = traceRay (r, EPSILON, bounces - 1, new_index, weight,tmp_h);
				RayTree::AddTransmittedSegment (r, 0, tmp_h.getT ());
				Vec3f refraction = transparentColor * v;
				color += refraction;
			}
		}

	    return color;
	}
	else
	{//û�н���
		return scene->getBackgroundColor ();
	}
}

Vec3f RayTracer::mirrorDirection (const Vec3f &normal, const Vec3f &incoming) {
	Vec3f v = incoming - 2 *normal.Dot3(incoming) * normal;
	v.Normalize ();
	return v;
}

bool RayTracer::transmittedDirection (const Vec3f &normal, const Vec3f &incoming,
	float index_i, float index_t, Vec3f &transmitted) {
	if (fabs (index_t) < EPSILON) return 0;
	float d = normal.Dot3(incoming);
	float x = index_i / index_t;//������
	float r = 1 - x * x * (1 - d * d);
	if (r < 0) return 0;
	r = sqrt (r);
	transmitted = x * (incoming - d * normal) - r * normal;
	transmitted.Normalize ();
	return 1;
}

bool RayTracer::InShadow (Ray &ray,Hit& hit,float dis) const {
	hit = Hit (FLT_MAX, NULL, Vec3f (0, 0, 0));	
	bool intersect= scene->getGroup ()->intersect (ray, hit, EPSILON);
	//����directional light��ֻҪ�ͱ�������ཻ������Ӱ
	//����point light����������ͱ�������ཻ���������˵�point light�ľ��룬����û����Ӱ��
	if (hit.getT () > dis)intersect = false;
	return intersect;
}
	