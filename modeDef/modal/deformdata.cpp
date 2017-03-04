/*********************************************************************
 * deformdata.cpp
 * Authored by Kris Hauser 2002-2003
 *
 * Source for the deformable object data.
 *
 * Copyright 2003, Regents of the University of California 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *********************************************************************/

#include "modal/deformdata.h"
#include "shared/stdincludes.h"
#include "math/bary.h"
#include <float.h>

/*
int DeformMeshData::RayCast(const Ray& r, vec3_t pos)
{
  int np = NumPolygons();
  int face [3];
  vec3_t a,b,c;

  int minface = -1;
  float_t mindist = FLT_MAX;
  float_t dist, t, u, v;
  vec3_t temp;
  for(int i=0; i<np; i++) {
    GetPolygon(i, face);
    GetVertex(face[0], a);
    GetVertex(face[1], b);
    GetVertex(face[2], c);

    if(ray_triangle_intersect(r.source, r.direction, a,b,c, &t, &u, &v)) {
      //u,v are the coordinates in the triangle's frame, t is the dist
      dist = t;
      if(dist < mindist) {
	mindist = dist;
	minface = i;
	intersect_to_intersection(t,u,v, a,b,c, temp);
	vec3_equal(pos, temp);
      }
    }
  }
  return minface;
}*/

int DeformMeshData::RayCast(const Ray& r, vec3_t pos)
{
  return ray_cast(r.source, r.direction, pos);
}

void DeformMeshData::GetPolygon(int i, int verts[3]) const
{
  verts[0] = faces[i][0].vertex;
  verts[1] = faces[i][1].vertex;
  verts[2] = faces[i][2].vertex;
}

void DeformMeshData::GetVertex(int i, vec3_t v) const
{
  vec3_equal(v, vertices[i]);
}

void DeformMeshData::GetBarycentricCoords(const vec3_t point, const int face[3], vec3_t out)
{
  int i;
  vec3_t vertex[3];
  float_t area[3];
  float_t total_area;

  for ( i = 0; i < 3; i++ ) {
    GetVertex(face[i], vertex[i]);
  }

  area[0] = triangle_area(point, vertex[1], vertex[2]);
  area[1] = triangle_area(point, vertex[2], vertex[0]);
  area[2] = triangle_area(point, vertex[0], vertex[1]);

  total_area = area[0]+area[1]+area[2];
  float_t inv_total_area = 1.0/total_area;

  for ( i = 0; i < 3; i++)
    out[i] = area[i]*inv_total_area;
}

int DeformMeshData::RayCast(const Ray& r, RayCastCollision& rc)
{
  int face_index = RayCast(r, rc.pos);
  if(face_index != -1) {
    GetPolygon(face_index, rc.poly);
    GetBarycentricCoords(rc.pos, rc.poly, rc.bary);
  }
  return face_index;
}





DeformTetData::DeformTetData()
:elements(NULL),num_elements(0)
{}

DeformTetData::~DeformTetData()
{
  SafeArrayDelete(elements);
}

void DeformTetData::Init(int size)
{
  SafeArrayDelete(elements);
  num_elements = size;
  elements = new tet_index_element[size];
}

bool DeformTetData::Load(std::ifstream& f)
{
  f >> num_elements;
  if(!f.good()) return false;
  printf("DeformTetData::Load(): %d elements\n", num_elements);
  Init(num_elements);
  for(int i=0; i<num_elements; i++)
    f >> elements[i][0] >> elements[i][1] >> elements[i][2] >> elements[i][3];
  return f.good();
}

bool DeformTetData::Save(std::ofstream& f)
{
	f << num_elements << std::endl;
  for(int i=0; i<num_elements; i++)
    f << elements[i][0] << " " <<
	 elements[i][1] << " " <<
	 elements[i][2] << " " <<
	 elements[i][3] << std::endl;
  return f.good();
}




DeformTetPointers::DeformTetPointers()
:elements(NULL), num_elements(0)
{}

DeformTetPointers::~DeformTetPointers()
{
	SafeArrayDelete(elements);
}

#define max(a,b) ( a>b? a:b)

void DeformTetPointers::Init(const DeformTetData& d, float_t* verts, int nv)
{
  SafeArrayDelete(elements);
  num_elements = d.num_elements;
  elements = new tetrahedron_ptr[num_elements];
  int imax = -1;
  for(int i=0; i<d.num_elements; i++) {
    assert(d.elements[i][0] < nv);
    assert(d.elements[i][1] < nv);
    assert(d.elements[i][2] < nv);
    assert(d.elements[i][3] < nv);
    assert(d.elements[i][0] >= 0);
    assert(d.elements[i][1] >= 0);
    assert(d.elements[i][2] >= 0);
    assert(d.elements[i][3] >= 0);
    
    elements[i][0] = &verts[d.elements[i][0]*3];
    elements[i][1] = &verts[d.elements[i][1]*3];
    elements[i][2] = &verts[d.elements[i][2]*3];
    elements[i][3] = &verts[d.elements[i][3]*3];
  }
}

DeformTetEmbedding::DeformTetEmbedding()
:bary(NULL), num_bary(0)
{}

DeformTetEmbedding::~DeformTetEmbedding()
{
  SafeArrayDelete(bary);
}

void DeformTetEmbedding::Init(int size)
{
  SafeArrayDelete(bary);
  num_bary = size;
  bary = new barycentric_index[size];
}

bool DeformTetEmbedding::Load(std::ifstream& f)
{
  f >> num_bary;
  if(!f.good()) return false;
  Init(num_bary);
  printf("DeformTetEmbedding::Load(): %d bary vertices\n");
  for(int i=0; i<num_bary; i++) {
    f >> bary[i].tet;
    f >> bary[i].coords[0] >> bary[i].coords[1] >> bary[i].coords[2] >> bary[i].coords[3];
  }
  return f.good();
}

bool DeformTetEmbedding::Save(std::ofstream& f)
{
	f << num_bary << std::endl;
  for(int i=0; i<num_bary; i++) {
    f << bary[i].tet << "\t";
    f << bary[i].coords[0] << " " <<
	 bary[i].coords[1] << " " <<
	 bary[i].coords[2] << " " <<
	 bary[i].coords[3] << std::endl;
  }
  return f.good();
}

int DeformTetEmbedding::Embed(const float_t* meshvertices, int nmv, const DeformTetPointers& tets) {
  Init(nmv);
  int num_inside = 0;
  const float_t* mv = meshvertices;
  for(int i=0; i<nmv; i++, mv+=3) {
    int min_tet = -1;
    float_t min_bary = FLT_MAX;
    bary_t temp;
    for(int j=0; j<tets.num_elements; j++) {
      float_t bary = get_barycentric_coordinates(mv, tets[j], temp);
      if(bary < min_bary) {
	min_bary = bary;
	min_tet = j;
      }
    }
    if(min_bary < 1.001)
      num_inside++;
    bary[i].tet = min_tet;
    get_barycentric_coordinates(mv, tets[min_tet], bary[i].coords);
  }
  return num_inside;
}

void DeformTetEmbedding::Apply(float_t* meshvertices, int nmv, const DeformTetData& tets, const float_t* tetvertices) const {
  const int* t;
  const barycentric_index* b = bary;
  for(int i=0; i<nmv; i++, b++, meshvertices+=3){
    t = tets.elements[b->tet];
    vec3_multiply(meshvertices, tetvertices + t[0]*3, b->coords[0]);
    vec3_madd(meshvertices, tetvertices + t[1]*3, b->coords[1]);
    vec3_madd(meshvertices, tetvertices + t[2]*3, b->coords[2]);
    vec3_madd(meshvertices, tetvertices + t[3]*3, b->coords[3]);
  }
}

void DeformTetEmbedding::Apply(float_t* meshvertices, int nmv, const DeformTetPointers& tets) const {
  const barycentric_index* b = bary;
  for(int i=0; i<nmv; i++, b++, meshvertices+=3){
	tets.Deform(*b, meshvertices);
  }
}


DeformObjectData::DeformObjectData()
  :carton_embedded(false), embedding(NULL)
{
  /*
  accel_response[0] = accel_response[1] = accel_response[2] = NULL;
  angaccel_response[0] = angaccel_response[1] = angaccel_response[2] = NULL;
  angvel_response[0] = angvel_response[1] = angvel_response[2] = NULL;
  */
}

DeformObjectData::~DeformObjectData()
{
  SafeDelete(embedding);
  /*
  for(int i=0; i<3; i++) {
    SafeDeleteProc(accel_response[i], vector_delete);
    SafeDeleteProc(angaccel_response[i], vector_delete);
    SafeDeleteProc(angvel_response[i], vector_delete);
  }
  */
}

bool DeformObjectData::Load(std::ifstream& f)
{
  if(!DeformData::Load(f)) return false;
  if(!DeformMeshData::Load(f)) return false;
  if(!DeformTetData::Load(f)) return false;

  printf("DeformObjectData::Load(): %d mesh vertices\n", NumVertices());
  printf("DeformObjectData::Load(): %d mesh faces\n", NumPolygons());

  int embed = 0;
  f >> embed;
  if(embed) {
    carton_embedded = true;
    SafeDelete(embedding);
    embedding = new DeformTetEmbedding;
    if(!embedding->Load(f)) return false;
  }
  return true;
}

bool DeformObjectData::Save(std::ofstream& f)
{
  if(!DeformData::Save(f)) return false;
  f << std::endl;
  if(!DeformMeshData::Save(f)) return false;
  f << std::endl;
  if(!DeformTetData::Save(f)) return false;
  f << std::endl;
  int embed = (carton_embedded ? 1 : 0);
  f << embed << std::endl;
  if(carton_embedded) {
    assert(embedding != NULL);
    if(!embedding->Save(f)) return false;
  }
  return true;
}

/*
void DeformObjectData::PrecalculateResponse() {
  int i,j;
  approximate_response = true;
  for(i=0; i<3; i++) {
    accel_response[i] = vector_create(num_Freq);
    angvel_response[i] = vector_create(num_Freq);
    angaccel_response[i] = vector_create(num_Freq);
  }

  for(i=0; i<num_Freq; i++) {
    accel_response[0][i] = 0.0;
    accel_response[1][i] = 0.0;
    accel_response[2][i] = 0.0;
    for(j=0; j<length; j+=3) {
      accel_response[0][i] -= mEigenVectors[j][i];
      accel_response[1][i] -= mEigenVectors[j+1][i];
      accel_response[2][i] -= mEigenVectors[j+2][i];
    }
  }

  for(i=0; i<num_Freq; i++) {
    //coriolis = 2[dp] = 0
    //centrifugal = 0 because it's the angvel squared?
    angvel_response[0][i] = 0.0;
    angvel_response[1][i] = 0.0;
    angvel_response[2][i] = 0.0;
  }
 
  //find derivative of accels wrt the angaccel (done for each component)
  //it's the negative cross product of the rest positions
  //thus the negatives cancel out
  for(i=0; i<num_Freq; i++) {
    angaccel_response[0][i] = 0.0;
    angaccel_response[1][i] = 0.0;
    angaccel_response[2][i] = 0.0;
    for(j=0; j<length; j+=3) {
      vec3_t temp;
      vec3_cross(temp, &mEigenVectors[j][i], &rest_positions[j]);
      angaccel_response[0][i] += temp[0];
      angaccel_response[1][i] += temp[1];
      angaccel_response[2][i] += temp[2];
    }
  }
}
*/






DeformObjectData* LoadDeformObjectData(const char* fn) {
	std::ifstream f(fn);
  if(f.fail()) {
    printf("Error opening object data file %s\n", fn);
    return NULL;
  }
  DeformObjectData* d = new DeformObjectData;

  if(!d->Load(f)) {
    printf("Error loading object data file %s\n", fn);
    return NULL;
  }

  return d;
}


