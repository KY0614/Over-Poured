#pragma once
#include <string>
#include <vector>
#include <unordered_map>


class StageObjectLibrary
{
public:

    /// <summary>
    /// �I�u�W�F�N�g�̑�����ێ�����\����
    /// </summary>
    struct ObjectParams 
    {
        std::string id_ = "";               //���ʎq
        bool placeable_ = false;            //��ɕ���u���邩�i��F���j
        bool carryable_ = false;            //�����^�тł��邩�i��F�J�b�v�j
        bool interactable_ = false;         //�v���C���[���A�N�V�����\��

        float interactTime_ = 0.0f;         //�C���^���N�g�ɂ����鎞�ԁi��F�������ԁj

        std::vector<std::string> acceptedItems_;   //�C���^���N�g�Ώۂɂł���A�C�e���i��F�R�b�v�����Ȃǁj

		float collisionRadius_ = 0.0f;    //�����蔻��̔��a
    };

    /// <summary>
    /// �w�肳�ꂽ�I�u�W�F�N�g���Ɋ�Â��ăf�[�^��ǂݍ���
    /// </summary>
    /// <param name="objName">�f�[�^��ǂݍ��ޑΏۂ̃I�u�W�F�N�g��</param>
    /// <returns>�I�u�W�F�N�g���ƑΉ�����p�����[�^</returns>
    static std::pair<std::string, ObjectParams> LoadData(const std::string& objName);
};

