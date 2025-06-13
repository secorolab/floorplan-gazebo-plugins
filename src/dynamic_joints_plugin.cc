#include <gz/sim/System.hh>
#include <gz/sim/Model.hh>
#include <gz/plugin/Register.hh>
#include <gz/common/URI.hh>
#include <gz/common/Util.hh>
#include "gz/sim/Util.hh"
#include "gz/sim/components/SourceFilePath.hh"
#include <gz/sim/components/JointPositionReset.hh>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <iostream>
#include <functional>

namespace gz
{
  class DynamicJointModelPlugin : public sim::System,
                                  public sim::ISystemConfigure,
                                  public sim::ISystemPreUpdate
  {
    public: void Configure(const sim::Entity &entity, const std::shared_ptr<const sdf::Element> &sdf,
                           sim::EntityComponentManager &ecm, sim::EventManager &eventMgr) override
    {
      this->entity = entity;
      this->sdf = sdf;
      this->posesKeyframes = std::vector<Pose>();
      this->currPose = 0.0;

      auto model = sim::Model(entity);

      std::string jointName = "";
      if (sdf->HasElement("joint")) 
      {
          jointName = sdf->Get<std::string>("joint");
          this->jointEntity = model.JointByName(ecm, jointName);
      }

      if (sdf->HasElement("uri")) 
      {
          std::string actionsUri = sdf->Get<std::string>("uri");
          auto modelEntity = topLevelModel(entity, ecm);
          auto modelPath = ecm.ComponentData<sim::components::SourceFilePath>(modelEntity);
          auto path = common::findFile(sim::asFullPath(actionsUri, modelPath.value()));

          std::ifstream posesJson(path);
          Json::Reader reader;
          Json::Value completeJsonData;
          reader.parse(posesJson, completeJsonData);

          for (int i = 0; i < completeJsonData["keyframes"].size(); i++){
              float time = completeJsonData["keyframes"][i]["time"].asFloat();
              double pose = completeJsonData["keyframes"][i]["pose"].asFloat();
              Pose poseStruct = {.pose = pose, .time = time};
              this->posesKeyframes.push_back(poseStruct);
          }
          posesJson.close();

          this->poseToExecute = this->posesKeyframes[0];
      }
    }

    public: void PreUpdate(const sim::UpdateInfo &info, sim::EntityComponentManager &ecm) override
    {
      float currentTime = std::chrono::duration_cast<std::chrono::seconds>(info.simTime).count();
      if (currentTime >= this->poseToExecute.time && !this->posesKeyframes.empty())
      {
          this->currPose = this->poseToExecute.pose;
          this->posesKeyframes.erase(this->posesKeyframes.begin());
          if (!this->posesKeyframes.empty())
          {
              this->poseToExecute = this->posesKeyframes[0];
          }

          auto jointPositionReset = ecm.Component<sim::components::JointPositionReset>(this->jointEntity);
          if (!jointPositionReset)
          {
              ecm.CreateComponent(this->jointEntity, sim::components::JointPositionReset({this->currPose}));
          }
          else
          {
              jointPositionReset->Data()[0] = this->currPose;
              ecm.SetChanged(this->jointEntity, sim::components::JointPositionReset::typeId);
          }
      }
    }

    struct Pose {
      double pose;
      float time;
    };

    private: sim::Entity entity;
    private: std::shared_ptr<const sdf::Element> sdf;
    private: std::vector<Pose> posesKeyframes;
    private: Pose poseToExecute;
    private: sim::Entity jointEntity;
    private: double currPose;
  };

  GZ_ADD_PLUGIN(DynamicJointModelPlugin, sim::System, DynamicJointModelPlugin::ISystemConfigure, DynamicJointModelPlugin::ISystemPreUpdate)
  GZ_ADD_PLUGIN_ALIAS(DynamicJointModelPlugin, "gz::sim::systems::DynamicJointModelPlugin")
}

