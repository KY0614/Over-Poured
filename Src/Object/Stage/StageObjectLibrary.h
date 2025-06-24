#pragma once
#include <string>
#include <vector>
#include <unordered_map>


class StageObjectLibrary
{
public:

    struct ObjectParams {
        std::string id_ = "";               //���ʎq
        std::string name_ = "";             //���O
        bool placeable_ = false;            //��ɕ���u���邩�i��F���j
        bool carryable_ = false;            //�����^�тł��邩�i��F�J�b�v�j
        bool interactable_ = false;         //�v���C���[���A�N�V�����\��

        std::string interactType_;          //"place", "machine", "serve", "charge", �ȂǓ���^�C�v�i�C�Ӂj
        float interactTime_ = 0.0f;          //�C���^���N�g�ɂ����鎞�ԁi��F�������ԁj

        std::vector<std::string> acceptedItems_;   //�C���^���N�g�Ώۂɂł���A�C�e���i��F�R�b�v�����Ȃǁj
        std::vector<std::string> produces_;        //�ΏۃA�C�e�����󂯎������A�n���A�C�e��

        // �Q�[���I�ȃJ�e�S���i���ށj��F"machine", "item", "counter"
        std::string category_;

    };

    static std::pair<std::string, ObjectParams> LoadData(const std::string& objName);

private:

};

