#include "GeometryElement.h"

Box::Box(qeal halfX, qeal halfY, qeal halfZ, Eigen::Vector3d center):
	_halfX(halfX),
	_halfY(halfY),
	_halfZ(halfZ),
	_center(center)
{
	pointsNum = 8;
	facesNum = 12;
	
	Eigen::Vector3d min = center - Eigen::Vector3d(_halfX, _halfY, _halfZ);
	Eigen::Vector3d max = center + Eigen::Vector3d(_halfX, _halfY, _halfZ);
	setFromBounding(min, max);

	faceIndices.buffer = (int*)malloc(3 * 12 * sizeof(int));
	faceIndices.offset = 0;
	faceIndices.span = 36;

	int indices[36] = {
	0, 1, 2,
	0, 2, 3,
	3, 2, 6,
	3, 6, 7,
	7, 6, 5,
	7, 5, 4,
	4, 5, 1,
	4, 1, 0,
	2, 1, 5,
	2, 5, 6,
	7, 4, 0,
	3, 7, 0 };
	std::copy(indices, indices + 36, faceIndices.buffer);
}

void Box::scale(qeal sx, qeal sy, qeal sz)
{
	_halfX *= sx;
	_halfY *= sy;
	_halfZ *= sz;
	Eigen::Vector3d min = _center - Eigen::Vector3d(_halfX, _halfY, _halfZ);
	Eigen::Vector3d max = _center + Eigen::Vector3d(_halfX, _halfY, _halfZ);
	setFromBounding(min, max);	
}

void Box::setFromBounding(const Eigen::Vector3d min, const Eigen::Vector3d max)
{
	//points.resize(24);
	if (points.buffer != nullptr)
		free(points.buffer);
	points.buffer = (qeal*)malloc(2 * 12 * sizeof(qeal));
	points.offset = 0;
	points.span = 24;

	points.buffer[0] = min[0];
	points.buffer[1] = min[1];
	points.buffer[2] = min[2];

	points.buffer[3] = min[0];
	points.buffer[4] = max[1];
	points.buffer[5] = min[2];

	points.buffer[6] = max[0];
	points.buffer[7] = max[1];
	points.buffer[8] = min[2];

	points.buffer[9] = max[0];
	points.buffer[10] = min[1];
	points.buffer[11] = min[2];

	points.buffer[12] = min[0];
	points.buffer[13] = min[1];
	points.buffer[14] = max[2];

	points.buffer[15] = min[0];
	points.buffer[16] = max[1];
	points.buffer[17] = max[2];

	points.buffer[18] = max[0];
	points.buffer[19] = max[1];
	points.buffer[20] = max[2];

	points.buffer[21] = max[0];
	points.buffer[22] = min[1];
	points.buffer[23] = max[2];

	_halfX = (max[0] - min[0]) / 2;
	_halfY = (max[1] - min[1]) / 2;
	_halfZ = (max[2] - min[2]) / 2;

	_center = (max + min) / 2.0;
}

void Box::setCenter(Eigen::Vector3d center)
{
	Eigen::Vector3d min = center - Eigen::Vector3d(_halfX, _halfY, _halfZ);
	Eigen::Vector3d max = center + Eigen::Vector3d(_halfX, _halfY, _halfZ);
	setFromBounding(min, max);
}

Floor::Floor(int xz)
{
	pointsNum = 8;
	facesNum = 12;
	
	hasTextureCoords = true;
	scaleXZ(xz);
	_y = 0;

	faceIndices.buffer = (int*)malloc(3 * 12 * sizeof(int));
	faceIndices.offset = 0;
	faceIndices.span = 36;

	int indices[36] = {
	0, 1, 2,
	0, 2, 3,
	3, 2, 6,
	3, 6, 7,
	7, 6, 5,
	7, 5, 4,
	4, 5, 1,
	4, 1, 0,
	2, 1, 5,
	2, 5, 6,
	7, 4, 0,
	3, 7, 0 };
	std::copy(indices, indices + 36, faceIndices.buffer);
}

void Floor::scaleXZ(int xz)
{
	_scaleXZ = xz;

	points.buffer = (qeal*)malloc(2 * 12 * sizeof(qeal));
	points.offset = 0;
	points.span = 24;

	Eigen::Vector3d min(-0.5 * _scaleXZ, -1.0, -0.5 * _scaleXZ);
	Eigen::Vector3d max(0.5 * _scaleXZ, 0, 0.5 * _scaleXZ);
	points.buffer[0] = min[0];
	points.buffer[1] = min[1];
	points.buffer[2] = min[2];

	points.buffer[3] = min[0];
	points.buffer[4] = max[1];
	points.buffer[5] = min[2];

	points.buffer[6] = max[0];
	points.buffer[7] = max[1];
	points.buffer[8] = min[2];

	points.buffer[9] = max[0];
	points.buffer[10] = min[1];
	points.buffer[11] = min[2];

	points.buffer[12] = min[0];
	points.buffer[13] = min[1];
	points.buffer[14] = max[2];

	points.buffer[15] = min[0];
	points.buffer[16] = max[1];
	points.buffer[17] = max[2];

	points.buffer[18] = max[0];
	points.buffer[19] = max[1];
	points.buffer[20] = max[2];

	points.buffer[21] = max[0];
	points.buffer[22] = min[1];
	points.buffer[23] = max[2];
	//
}

void Floor::setYPlane(qeal Y)
{
	_y = Y;
	for (int i = 0; i < pointsNum; i++)
		points.buffer[3 * i + 1] = _y;
}

void SphereElement::operator=(const SphereElement& b)
{
	center = b.center;
	radius = b.radius;
}

void SphereElement::show()
{
	glPushMatrix();
	glTranslated(center.data()[0], center.data()[1], center.data()[2]);
	glColor3d(204.0 / 255.0, 204.0 / 255.0, 153.0 / 255.0);
	gluSphere(gluNewQuadric(), radius, 50, 50);
	glPopMatrix();
}

SplintElement::SplintElement(const Vector3 v0, const Vector3 v1, const Vector3 v2, Vector3 n)
{
	vt[0] = v0;
	vt[1] = v1;
	vt[2] = v2;
	if (n == Vector3(0, 0, 0))
		updateNormal();
}

void SplintElement::updateNormal(bool reverse)
{
	const Vector3 v0v1 = vt[1] - vt[0];
	const Vector3 v0v2 = vt[2] - vt[0];
	nt = v0v1.cross(v0v2);
	nt.normalize();
	if (reverse)
	{
		nt = nt * -1;
	}
}

void ConeElement::computeCone()
{
	Vector3 c0c1 = _sp1 - _sp0;
	// one sphere is included in another sphere
	if (c0c1.norm() - abs(_r1 - _r0) < 1e-8)
	{
		apex = _r1 > _r0 ? _sp1 : _sp0;
		axis = Vector3(0, 0, 1);
		base = _r0 > _r1 ? _r0 : _r1;
		top = _r1 < _r0 ? _r1 : _r0;
		height = 0.0;
		rot_axis = Vector3(0, 0, 1);
		rot_angle = 0.;
		return;
	}

	if (c0c1.norm() < 1e-8)
	{
		apex = _sp0;
		axis = Vector3(0, 0, 1);
		base = _r0;
		top = _r0;
		height = 0.;
		rot_axis = Vector3(0, 0, 1);
		rot_angle = 0.;
	}
	else
	{
		qeal dr0r1 = fabs(_r0 - _r1);
		if (dr0r1 < 1e-8)
		{
			apex = _sp0;
			axis = _sp1 - _sp0;
			axis.normalize();
			base = _r0;
			top = _r0;
			height = (_sp1 - _sp0).norm();
		}
		else
		{
			apex = (_r1 * _sp0 - _r0 * _sp1) / (_r1 - _r0);
			axis = (_r0 > _r1) ? (_sp1 - _sp0) : (_sp0 - _sp1);
			axis.normalize();

			qeal cangle;
			Vector3 apexc0 = apex - _sp0;
			qeal vc0len = apexc0.norm();
			Vector3 apexc1 = apex - _sp1;
			qeal vc1len = apexc1.norm();

			cangle = sqrt(1. - _r0 * _r0 / vc0len / vc0len);

			if (_r0 < _r1)
			{
				apex = _sp0 + apexc0 * cangle * cangle;
				base = _r1 * cangle;
				top = _r0 * cangle;
				height = abs(vc1len - vc0len) * cangle * cangle;
			}
			else
			{
				apex = _sp1 + apexc1 * cangle * cangle;
				base = _r0 * cangle;
				top = _r1 * cangle;
				height = abs(vc0len - vc1len) * cangle * cangle;
			}
		}

		Vector3 za(0, 0, 1);
		rot_angle = acos(axis.dot(za));
		if ((fabs(rot_angle) < 1e-12) || (fabs(rot_angle - M_PI) < 1e-12))
			rot_axis = Vector3(0, 0, 1);
		else
			rot_axis = axis.cross(za);
		rot_axis.normalize();
		rot_angle *= (180. / M_PI);
	}
}

void ConeElement::show()
{
}

void TwoSplintElements::show()
{
	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor3d(204.0 / 255.0, 204.0 / 255.0, 153.0 / 255.0);
	glNormal3f(st1.nt[0], st1.nt[1], st1.nt[2]);
	glVertex3f(st1.vt[0][0], st1.vt[0][1], st1.vt[0][2]);
	glVertex3f(st1.vt[1][0], st1.vt[1][1], st1.vt[1][2]);
	glVertex3f(st1.vt[2][0], st1.vt[2][1], st1.vt[2][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3d(204.0 / 255.0, 204.0 / 255.0, 153.0 / 255.0);
	glNormal3f(-st1.nt[0], -st1.nt[1], -st1.nt[2]);
	glVertex3f(st1.vt[0][0], st1.vt[0][1], st1.vt[0][2]);
	glVertex3f(st1.vt[2][0], st1.vt[2][1], st1.vt[2][2]);
	glVertex3f(st1.vt[1][0], st1.vt[1][1], st1.vt[1][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3d(204.0 / 255.0, 204.0 / 255.0, 153.0 / 255.0);
	glNormal3f(st2.nt[0], st2.nt[1], st2.nt[2]);
	glVertex3f(st2.vt[0][0], st2.vt[0][1], st2.vt[0][2]);
	glVertex3f(st2.vt[1][0], st2.vt[1][1], st2.vt[1][2]);
	glVertex3f(st2.vt[2][0], st2.vt[2][1], st2.vt[2][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3d(204.0 / 255.0, 204.0 / 255.0, 153.0 / 255.0);
	glNormal3f(-st2.nt[0], -st2.nt[1], -st2.nt[2]);
	glVertex3f(st2.vt[0][0], st2.vt[0][1], st2.vt[0][2]);
	glVertex3f(st2.vt[2][0], st2.vt[2][1], st2.vt[2][2]);
	glVertex3f(st2.vt[1][0], st2.vt[1][1], st2.vt[1][2]);
	glEnd();

	glPopMatrix();
}
