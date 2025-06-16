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
      this->keyframeList = std::vector<Keyframe>();
      this->currPosition = 0.0;

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

          std::ifstream keyframeJson(path);
          Json::Reader reader;
          Json::Value completeJsonData;
          reader.parse(keyframeJson, completeJsonData);

          for (int i = 0; i < completeJsonData["keyframes"].size(); i++){
              float time = completeJsonData["keyframes"][i]["time"].asFloat();
              double position = completeJsonData["keyframes"][i]["position"].asFloat();
              Keyframe kf = {.position = position, .time = time};
              this->keyframeList.push_back(kf);
          }
          keyframeJson.close();

          this->keyframeToExecute = this->keyframeList[0];
      }
    }

    public: void PreUpdate(const sim::UpdateInfo &info, sim::EntityComponentManager &ecm) override
    {
      float currentTime = std::chrono::duration_cast<std::chrono::seconds>(info.simTime).count();
      if (currentTime >= this->keyframeToExecute.time && !this->keyframeList.empty())
      {
          this->currPosition = this->keyframeToExecute.position;
          this->keyframeList.erase(this->keyframeList.begin());
          if (!this->keyframeList.empty())
          {
              this->keyframeToExecute = this->keyframeList[0];
          }

          auto jointPositionReset = ecm.Component<sim::components::JointPositionReset>(this->jointEntity);
          if (!jointPositionReset)
          {
              ecm.CreateComponent(this->jointEntity, sim::components::JointPositionReset({this->currPosition}));
          }
          else
          {
              jointPositionReset->Data()[0] = this->currPosition;
              ecm.SetChanged(this->jointEntity, sim::components::JointPositionReset::typeId);
          }
      }
    }

    struct Keyframe {
      double position;
      float time;
    };

    private: sim::Entity entity;
    private: std::shared_ptr<const sdf::Element> sdf;
    private: std::vector<Keyframe> keyframeList;
    private: Keyframe keyframeToExecute;
    private: sim::Entity jointEntity;
    private: double currPosition;
  };

  GZ_ADD_PLUGIN(DynamicJointModelPlugin, sim::System, DynamicJointModelPlugin::ISystemConfigure, DynamicJointModelPlugin::ISystemPreUpdate)
  GZ_ADD_PLUGIN_ALIAS(DynamicJointModelPlugin, "gz::sim::systems::DynamicJointModelPlugin")
}

