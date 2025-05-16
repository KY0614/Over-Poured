#pragma once
#include <string>
#include <vector>
#include <unordered_map>


class StageObjectLibrary
{
public:

    struct ObjectParams{
        std::string id_;               //���ʎq
        std::string modelFile_;        //���f���t�@�C����
        bool placeable_ = false;       //��ɕ���u���邩�i��F���j
        bool carryable_ = false;       //�����^�тł��邩�i��F�J�b�v�j
        bool interactable_ = false;    //�v���C���[���A�N�V�����\��

        std::string interactType_;     //"place", "machine", "serve", "charge", �ȂǓ���^�C�v�i�C�Ӂj
        float interactTime = 0.0f;    //�C���^���N�g�ɂ����鎞�ԁi��F�������ԁj

        std::vector<std::string> acceptedItems_; //�C���^���N�g�Ώۂɂł���A�C�e���i��F�R�b�v�����Ȃǁj

        // �Q�[���I�ȃJ�e�S���i���ށj��F"machine", "item", "counter"
        std::string category_;

        //// �����ڂŔ��f�ł��鑮���i�z�b�g/�A�C�X�p�j�Ȃ�
        //std::string visualHin_t;
    };

    static bool LoadData(const std::string& jsonFilePath);
    static const ObjectParams& Get(const std::string& id);

private:

    static std::unordered_map<std::string, ObjectParams> objectMap_;

};

