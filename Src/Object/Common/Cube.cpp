#include <DxLib.h>
#include <vector>
#include "Cube.h"

Cube::Cube(const Transform& parent) : transformParent_(parent)
{
}

Cube::Cube(const Cube& base, const Transform& parent) : transformParent_(parent)
{
}

Cube::~Cube(void)
{
}

void Cube::MakeCube(VECTOR center, float size, COLOR_U8  col)
{
    float h = size / 2.0f;

    // 8���_���`�i�����̂̊e���j
    VECTOR v[8] = {
        VGet(center.x - h, center.y - h, center.z - h), // 0: �����O
        VGet(center.x + h, center.y - h, center.z - h), // 1: �E���O
        VGet(center.x + h, center.y + h, center.z - h), // 2: �E��O
        VGet(center.x - h, center.y + h, center.z - h), // 3: ����O
        VGet(center.x - h, center.y - h, center.z + h), // 4: ������
        VGet(center.x + h, center.y - h, center.z + h), // 5: �E����
        VGet(center.x + h, center.y + h, center.z + h), // 6: �E���
        VGet(center.x - h, center.y + h, center.z + h)  // 7: �����
    };

    ////��ʍ��W�𒆐S���W��
    //VECTOR v[8] = {
    //    VGet(center.x - h, center.y,     center.z - h), // ��
    //    VGet(center.x + h, center.y,     center.z - h),
    //    VGet(center.x + h, center.y + size, center.z - h),
    //    VGet(center.x - h, center.y + size, center.z - h),
    //    VGet(center.x - h, center.y,     center.z + h),
    //    VGet(center.x + h, center.y,     center.z + h),
    //    VGet(center.x + h, center.y + size, center.z + h),
    //    VGet(center.x - h, center.y + size, center.z + h)
    //};

    // �e�ʂ��ƂɎO�p�`��2����`�i�S6�� �� 12�O�p�`�j

    // �e�ʂ̎O�p�`�̒��_�C���f�b�N�X�i12�O�p�` * 3�j
    int faceIndices[6][2][3] = {
    { {2, 1, 0}, {3, 2, 0} }, // �O��
    { {4, 5, 6}, {4, 6, 7} }, // �w��
    { {3, 0, 4}, {7, 3, 4} }, // ����
    { {6, 5, 1}, {2, 6, 1} }, // �E��
    { {6, 2, 3}, {7, 6, 3} }, // ���
    { {0, 1, 5}, {0, 5, 4} }  // ���
    };

    int vi = 0;
    for (int f = 0; f < 6; ++f) {
        VECTOR normal;

        // �e�ʂ̖@���x�N�g���i���������C�e�B���O�p�j
        switch (f) {
        case 0: normal = VGet(0, 0, -1); break; // �O
        case 1: normal = VGet(0, 0, 1); break; // ��
        case 2: normal = VGet(-1, 0, 0); break; // ��
        case 3: normal = VGet(1, 0, 0); break; // �E
        case 4: normal = VGet(0, 1, 0); break; // ��
        case 5: normal = VGet(0, -1, 0); break; // ��
        }

        for (int t = 0; t < 2; ++t) {
            for (int i = 0; i < 3; ++i) {
                int idx = faceIndices[f][t][i];
                Vertex[vi].pos = v[idx];
                Vertex[vi].norm = normal;
                Vertex[vi].dif = col;
                Vertex[vi].spc = GetColorU8(0, 0, 0, 0);
                Vertex[vi].u = Vertex[vi].v = Vertex[vi].su = Vertex[vi].sv = 0.0f;
                vi++;
            }
        }

    }
}

void Cube::MakeBox(VECTOR center, float width, float height, float depth, COLOR_U8 col)
{
    VERTEX3D verts[36]; // 6�� �~ 2�O�p�` �~ 3���_ = 36
    int vi = 0;

    float hw = width / 2.0f;
    float hh = height / 2.0f;
    float hd = depth / 2.0f;

    //VECTOR p[8] =
    //{
    //    VGet(center.x - hw, center.y - hh, center.z - hd), // 0: ������O
    //    VGet(center.x + hw, center.y - hh, center.z - hd), // 1: �E����O
    //    VGet(center.x + hw, center.y + hh, center.z - hd), // 2: �E���O
    //    VGet(center.x - hw, center.y + hh, center.z - hd), // 3: �����O
    //    VGet(center.x - hw, center.y - hh, center.z + hd), // 4: ������
    //    VGet(center.x + hw, center.y - hh, center.z + hd), // 5: �E����
    //    VGet(center.x + hw, center.y + hh, center.z + hd), // 6: �E�㉜
    //    VGet(center.x - hw, center.y + hh, center.z + hd), // 7: ���㉜
    //};

    //��ʒ��S���W
    VECTOR p[8] =
    {
        VGet(center.x - hw, center.y,         center.z - hd), // 0: ������O
        VGet(center.x + hw, center.y,         center.z - hd), // 1: �E����O
        VGet(center.x + hw, center.y + height, center.z - hd), // 2: �E���O
        VGet(center.x - hw, center.y + height, center.z - hd), // 3: �����O
        VGet(center.x - hw, center.y,         center.z + hd), // 4: ������
        VGet(center.x + hw, center.y,         center.z + hd), // 5: �E����
        VGet(center.x + hw, center.y + height, center.z + hd), // 6: �E�㉜
        VGet(center.x - hw, center.y + height, center.z + hd), // 7: ���㉜
    };

    struct Face {
        int a, b, c, d;
        VECTOR normal;
    };

    Face faces[6] =
    {
        { 0, 1, 2, 3, VGet(0.0f,  0.0f, -1.0f) }, // �O
        { 5, 4, 7, 6, VGet(0.0f,  0.0f,  1.0f) }, // ���
        { 4, 0, 3, 7, VGet(-1.0f,  0.0f,  0.0f) }, // ��
        { 1, 5, 6, 2, VGet(1.0f,  0.0f,  0.0f) }, // �E
        { 3, 2, 6, 7, VGet(0.0f,  1.0f,  0.0f) }, // ��
        { 4, 5, 1, 0, VGet(0.0f, -1.0f,  0.0f) }, // ��
    };

    for (int i = 0; i < 6; ++i)
    {
        Face f = faces[i];

        // �O�p�`1: a, b, c
        verts[vi++] = { p[f.c], f.normal, col, GetColorU8(0,0,0,0), 0,0,0,0 };
        verts[vi++] = { p[f.b], f.normal, col, GetColorU8(0,0,0,0), 0,0,0,0 };
        verts[vi++] = { p[f.a], f.normal, col, GetColorU8(0,0,0,0), 0,0,0,0 };

        // �O�p�`2: a, c, d
        verts[vi++] = { p[f.d], f.normal, col, GetColorU8(0,0,0,0), 0,0,0,0 };
        verts[vi++] = { p[f.c], f.normal, col, GetColorU8(0,0,0,0), 0,0,0,0 };
        verts[vi++] = { p[f.a], f.normal, col, GetColorU8(0,0,0,0), 0,0,0,0 };
    }
    
    // �`��i12�|���S���j
    DrawPolygon3D(verts, 12, DX_NONE_GRAPH, true);
}

void Cube::Draw(void)
{
    // �����̕`��i12�|���S���j
    DrawPolygon3D(Vertex, 12, DX_NONE_GRAPH, false);
}
