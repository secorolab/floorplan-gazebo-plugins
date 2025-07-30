#include <gz/sim/System.hh>
#include <gz/sim/Model.hh>
#include <gz/plugin/Register.hh>
#include <gz/common/URI.hh>
#include <gz/sim/Util.hh>
#include <gz/msgs/actuators.pb.h>
#include <gz/transport/Node.hh>
#include "gz/sim/components/SourceFilePath.hh"
#include "gz/sim/components/Joint.hh"
#include <gz/sim/components/Name.hh>
#include <jsoncpp/json/json.h>

namespace gz
{
  using namespace std::chrono_literals;
  class TimedJointStateChangePlugin : public sim::System,
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
      this->topic = transport::TopicUtils::AsValidTopic("/" + model.Name(ecm) + "_controller");

      if (!sdf->HasElement("joint"))
      {
        gzerr << "Missing required parameter <joint>." << std::endl;
        return;
      }

      std::string jointName = sdf->Get<std::string>("joint");
      this->jointEntity = ecm.EntityByComponents(sim::components::Joint(), sim::components::Name(jointName));

      if (!sdf->HasElement("uri"))
      {
        gzerr << "Missing required parameter <uri>." << std::endl;
        return;
      }
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

      gzdbg << "Timed state change plugin parameters:" << std::endl;
      gzdbg << "joint: [" << jointName << "]" << std::endl;
      gzdbg << "file: " << path << std::endl;
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

          transport::Node node;
          auto pub = node.Advertise<msgs::Actuators>(this->topic);

          const double targetPosition{this->currPosition};
          msgs::Actuators msg;
          msg.add_position(targetPosition);

          pub.Publish(msg);
          std::this_thread::sleep_for(100ms);
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
    private: std::string topic;
  };

  GZ_ADD_PLUGIN(TimedJointStateChangePlugin, sim::System, TimedJointStateChangePlugin::ISystemConfigure, TimedJointStateChangePlugin::ISystemPreUpdate)
  GZ_ADD_PLUGIN_ALIAS(TimedJointStateChangePlugin, "gz::sim::systems::TimedJointStateChangePlugin")
}

