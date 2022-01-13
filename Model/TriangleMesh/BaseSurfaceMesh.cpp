#include "BaseSurfaceMesh.h"
#include "Commom/FileIO.h"
#include "Model/tiny_obj_loader.h"
#include <stdlib.h>

void BaseSurfaceMesh::setNameAndDir(const std::string filename)
{
	getFilenameInfo(filename, dir, name, format);
	if (nickName == std::string(""))
		nickName = name;
}

bool BaseSurfaceMesh::readMeshFromObjFormat(const std::string filename, BaseSurfaceMeshBufferPool* pool)
{
	std::ifstream fin(filename.c_str());
	if (!fin.is_open())
		return false;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> groupMaterials;

	std::string err;
	std::string base_dir = getPathDir(filename);
	if (base_dir.empty())
	{
		base_dir = ".";
	}

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &groupMaterials, &err, filename.c_str(), base_dir.c_str(), true);
	if (!err.empty())
	{
		std::cerr << err << std::endl;
	}
	if (!ret) {
		std::cerr << "Error: Failed to load " << filename << " !" << std::endl;
		fin.close();
		return false;
	}

	hasNormals = false;
	hasTextureCoords = false;
	bool hasMaterials = false;

	if ((int)(attrib.normals.size()) / 3 > 0)
		hasNormals = true;
	if ((int)(attrib.texcoords.size()) / 3 > 0)
		hasTextureCoords = true;
	if ((int)groupMaterials.size() > 0)
		hasMaterials = true;

	int pointsNum = (int)(attrib.vertices.size()) / 3;
	int textureCoordNum = (int)(attrib.texcoords.size()) / 2;

	std::vector<qeal> points(attrib.vertices.size());
	for (int i = 0; i < attrib.vertices.size(); i++)
		points[i] = attrib.vertices[i];

	std::vector<qeal> pointNormals(3 * pointsNum);

	std::vector<qeal> texCoords(attrib.texcoords.size());
	for (int i = 0; i < attrib.texcoords.size(); i++)
		texCoords[i] = attrib.texcoords[i];

	std::vector<std::vector<int>> pointFaceList(pointsNum);

	int renderGroupNum = shapes.size();
	std::vector<int> groupFacesNum(renderGroupNum);
	std::vector<int> faceIndices;

	for (int i = 0; i < renderGroupNum; i++)
	{
		groupFacesNum[i] = shapes[i].mesh.indices.size() / 3;

		for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++)
		{
			tinyobj::index_t idx0 = shapes[i].mesh.indices[3 * f + 0];
			tinyobj::index_t idx1 = shapes[i].mesh.indices[3 * f + 1];
			tinyobj::index_t idx2 = shapes[i].mesh.indices[3 * f + 2];

			int vid0 = idx0.vertex_index;
			int vid1 = idx1.vertex_index;
			int vid2 = idx2.vertex_index;

			faceIndices.push_back(vid0);
			faceIndices.push_back(vid1);
			faceIndices.push_back(vid2);

			int face_id = faceIndices.size() / 3 - 1;
			pointFaceList[vid0].push_back(face_id);
			pointFaceList[vid1].push_back(face_id);
			pointFaceList[vid2].push_back(face_id);
		}
	}

	int facesNum = (int)faceIndices.size() / 3;

	std::vector<qeal> faceNormals(3 * facesNum);
	//alignBuffer

	////////////////////////////////////

	this->pointsNum = pointsNum;
	this->facesNum = facesNum;

	pool->totalPointsNum += this->pointsNum;
	pool->totalFacesNum += this->facesNum;

	registerToOverallBuffer<qeal>(points, pool->pointsBuffer, this->points);

	registerToOverallBuffer<qeal>(pointNormals, pool->pointsNormalBuffer, this->pointNormals);
	registerToOverallBuffer<qeal>(faceNormals, pool->facesNormalBuffer, this->faceNormals);
	registerToOverallBuffer<int>(faceIndices, pool->faceIndicesBuffer, this->faceIndices);
	registerToOverallBuffer<int>(pointFaceList, pool->pointFaceListBuffer, this->pointFaceIndices);
	//////////////////////////////////////
	setNameAndDir(filename);
	pool->meshNum++;

	pointsOffset = this->points.offset / 3;
	faceOffset = this->faceIndices.offset / 3;
	return true;
}

bool BaseSurfaceMesh::writeMeshToObjFormat(const std::string filename)
{
	std::string dir, name, format;
	getFilenameInfo(filename, dir, name, format);
	if (format != std::string("obj"))
		return false;

	std::ofstream fout(filename.c_str());
	if (!fout.is_open())
		return false;
	
	for (size_t i = 0; i < pointsNum; i++)
		fout << "v " << points.buffer[3 * i] << " " << points.buffer[3 * i + 1] << " " << points.buffer[3 * i + 2] << std::endl;

	for (size_t i = 0; i < facesNum; i++)
		fout << "f " << faceIndices.buffer[3 * i] << " " << faceIndices.buffer[3 * i + 1] << " " << faceIndices.buffer[3 * i + 2] << std::endl;

	return true;
}

qeal BaseSurfaceMesh::uniform()
{
	qeal div = bbox[3] - bbox[0];
	if (div < (bbox[4] - bbox[1]))
		div = (bbox[4] - bbox[1]);
	if (div < (bbox[5] - bbox[2]))
		div = (bbox[5] - bbox[2]);
	if (fabs(div) < 1e-6) div = 1.0;

	for (int i = 0; i < pointsNum; i++)
	{
		points.buffer[3 * i] /= div;
		points.buffer[3 * i + 1] /= div;
		points.buffer[3 * i + 2] /= div;
	}


	return div;
}

void BaseSurfaceMesh::computeBBox()
{
	bbox[0] = QEAL_MAX;
	bbox[1] = QEAL_MAX;
	bbox[2] = QEAL_MAX;
	bbox[3] = -QEAL_MAX;
	bbox[4] = -QEAL_MAX;
	bbox[5] = -QEAL_MAX;
	for (int i = 0; i < pointsNum; i++)
	{
		qeal x = points.buffer[3 * i];
		qeal y = points.buffer[3 * i + 1];
		qeal z = points.buffer[3 * i + 2];
		if (x < bbox[0])
			bbox[0] = x;
		else if (x > bbox[3])
			bbox[3] = x;

		if (y < bbox[1])
			bbox[1] = y;
		else if (y > bbox[4])
			bbox[4] = y;

		if (z < bbox[2])
			bbox[2] = z;
		else if (z > bbox[5])
			bbox[5] = z;
	}
}

void BaseSurfaceMesh::getCenter(qeal & cx, qeal & cy, qeal & cz)
{
	computeBBox();
	cx = (bbox[0] + bbox[3]) / 2.0;
	cy = (bbox[1] + bbox[4]) / 2.0;
	cz = (bbox[2] + bbox[5]) / 2.0;
}

void BaseSurfaceMesh::translateMesh(const qeal x, const qeal y, const qeal z)
{
	for (int i = 0; i < pointsNum; i++)
	{
		points.buffer[3 * i] += x;
		points.buffer[3 * i + 1] += y;
		points.buffer[3 * i + 2] += z;
	}
}

void BaseSurfaceMesh::scaleMesh(const qeal s, const qeal cx, const qeal cy, const qeal cz)
{
	for (int i = 0; i < pointsNum; i++)
	{
		points.buffer[3 * i] = (points.buffer[3 * i] - cx) * s + cx;
		points.buffer[3 * i + 1] = (points.buffer[3 * i + 1] - cy) * s + cy;
		points.buffer[3 * i + 2] = (points.buffer[3 * i + 2] - cz) * s + cz;
	}
}

void BaseSurfaceMesh::rotateMesh(const qglviewer::Quaternion oldR, const qglviewer::Quaternion R, const qeal cx, const qeal cy, const qeal cz)
{
	for (int i = 0; i < pointsNum; i++)
	{
		qglviewer::Vec pos(points.buffer[3 * i] - cx, points.buffer[3 * i + 1] - cy, points.buffer[3 * i + 2] - cz);
		pos = oldR.inverseRotate(pos);
		pos = R.rotate(pos);

		points.buffer[3 * i] = pos.x + cx;
		points.buffer[3 * i + 1] = pos.y + cy;
		points.buffer[3 * i + 2] = pos.z + cz;
	}
}

Vector3 BaseSurfaceMeshBuffer::getSurfacePoint(int nid)
{
	return Vector3(points.buffer[3 * nid], points.buffer[3 * nid + 1], points.buffer[3 * nid + 2]);
}

void BaseSurfaceMeshBuffer::setSurfacePoint(int nid, qeal* p)
{
	points.buffer[3 * nid] = p[0];
	points.buffer[3 * nid + 1] = p[1];
	points.buffer[3 * nid + 2] = p[2];
}

Vector3 BaseSurfaceMeshBuffer::getSurfacePointNormal(int nid)
{
	return Vector3(pointNormals.buffer[3 * nid], pointNormals.buffer[3 * nid + 1], pointNormals.buffer[3 * nid + 2]);
}

void BaseSurfaceMeshBuffer::setSurfacePointNormal(int nid, qeal* n)
{
	pointNormals.buffer[3 * nid] = n[0];
	pointNormals.buffer[3 * nid + 1] = n[1];
	pointNormals.buffer[3 * nid + 2] = n[2];
}

Vector3 BaseSurfaceMeshBuffer::getSurfaceFaceNormal(int nid)
{
	return Vector3(faceNormals.buffer[3 * nid], faceNormals.buffer[3 * nid + 1], faceNormals.buffer[3 * nid + 2]);
}

void BaseSurfaceMeshBuffer::setSurfaceFaceNormal(int nid, qeal* n)
{
	faceNormals.buffer[3 * nid] = n[0];
	faceNormals.buffer[3 * nid + 1] = n[1];
	faceNormals.buffer[3 * nid + 2] = n[2];
}

Vector3i BaseSurfaceMeshBuffer::getSurfaceFace(int fid)
{
	return Vector3i(faceIndices.buffer[3 * fid], faceIndices.buffer[3 * fid + 1], faceIndices.buffer[3 * fid + 2]);
}

int BaseSurfaceMeshBuffer::getSurfacePointOverallId(int nid)
{
	return nid + pointsOffset;
}

int BaseSurfaceMeshBuffer::getSurfaceFaceOverallId(int fid)
{
	return fid + faceOffset;
}
