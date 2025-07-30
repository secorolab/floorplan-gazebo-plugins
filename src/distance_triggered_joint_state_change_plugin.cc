#include <gz/sim/System.hh>
#include <gz/sim/Model.hh>
#include <gz/plugin/Register.hh>
#include <gz/sim/components/Pose.hh>
#include "gz/sim/components/Joint.hh"
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Model.hh>
#include <gz/msgs/actuators.pb.h>
#include <gz/transport/Node.hh>

namespace gz
{
  using namespace std::chrono_literals;
  class DistanceTriggeredJointStateChangePlugin : public sim::System,
                                      public sim::ISystemConfigure,
                                      public sim::ISystemPreUpdate
  {
    public: void Configure(const sim::Entity &entity, const std::shared_ptr<const sdf::Element> &sdf,
                           sim::EntityComponentManager &ecm, sim::EventManager &eventMgr) override
    {
      this->entity = entity;
      this->sdf = sdf;

      auto model = sim::Model(entity);
      this->robot = sim::Entity();
      this->active = false;
      this->topic = transport::TopicUtils::AsValidTopic("/" + model.Name(ecm) + "_controller");

      if (!sdf->HasElement("joint"))
      {
        gzerr << "Missing required parameter <joint>." << std::endl;
        return;
      }

      std::string jointName = sdf->Get<std::string>("joint");
      this->jointEntity = ecm.EntityByComponents(sim::components::Joint(), sim::components::Name(jointName));

      if (sdf->HasElement("end_joint_angle")) this->eventPose = sdf->Get<double>("end_joint_angle");
      if (sdf->HasElement("trigger_dist")) this->distanceToEvent = sdf->Get<float>("trigger_dist");

      if (!sdf->HasElement("target"))
      {
        gzerr << "Missing required parameter <target>." << std::endl;
        return;
      }
      this->targetName = sdf->Get<std::string>("target");

      gzdbg << "Dist. triggered plugin parameters:" << std::endl;
      gzdbg << "joint: [" << jointName << "]" << std::endl;
      gzdbg << "end joint angle: [" << this->eventPose << "]" << std::endl;
      gzdbg << "trigger distance: [" << this->distanceToEvent << "]" << std::endl;
      gzdbg << "target: [" << this->targetName << "]" << std::endl;

    }

    public: void PreUpdate(const sim::UpdateInfo &info, sim::EntityComponentManager &ecm) override
    {
      float currentTime = std::chrono::duration_cast<std::chrono::seconds>(info.simTime).count();

      if (this->robot == sim::Entity())
      {

        gzmsg << "Finding " << this->targetName << " model" << std::endl;
        ecm.Each<sim::components::Model, sim::components::Name>(
          [&](const sim::Entity &entity, const sim::components::Model *, const sim::components::Name *nameComp) -> bool
          {
            if (nameComp->Data() == this->targetName)
            {
              this->robot = entity;
              return false;
            }
            return true;
          });

      }
      else
      {
        auto robotPose = ecm.Component<sim::components::Pose>(this->robot)->Data();
        auto doorPose = ecm.Component<sim::components::Pose>(this->entity)->Data();
        float distance = doorPose.Pos().Distance(robotPose.Pos());

        if (distance < this->distanceToEvent && !this->active)
        {
          gzmsg << "Activating distance trigger" << std::endl;
          transport::Node node;
          auto pub = node.Advertise<msgs::Actuators>(this->topic);

          const double targetPosition{this->eventPose};
          msgs::Actuators msg;
          msg.add_position(targetPosition);

          pub.Publish(msg);
          std::this_thread::sleep_for(100ms);
          this->active = true;
        }
      }
    }
    public: void PostUpdate(const sim::UpdateInfo &info, sim::EntityComponentManager &ecm)
    {
        auto robotPose = ecm.Component<sim::components::Pose>(this->robot)->Data();
        auto doorPose = ecm.Component<sim::components::Pose>(this->entity)->Data();
        float distance = doorPose.Pos().Distance(robotPose.Pos());
        if (distance > this->distanceToEvent && this->active){
          gzmsg << "Deactivating distance trigger" << std::endl;
          this->active = false;
        }

    }

    private: sim::Entity entity;
    private: std::shared_ptr<const sdf::Element> sdf;

    private: double eventPose;
    private: float distanceToEvent;
    private: std::string targetName;
    private: bool active;
    private: std::string topic;

    private: sim::Entity jointEntity;
    private: sim::Entity robot;
  };

  GZ_ADD_PLUGIN(DistanceTriggeredJointStateChangePlugin, sim::System, DistanceTriggeredJointStateChangePlugin::ISystemConfigure, DistanceTriggeredJointStateChangePlugin::ISystemPreUpdate)
  GZ_ADD_PLUGIN_ALIAS(DistanceTriggeredJointStateChangePlugin, "gz::sim::systems::DistanceTriggeredJointStateChangePlugin")
}

