#include <functional>
#include <gz/sim/System.hh>
#include <gz/sim/Model.hh>
#include <gz/plugin/Register.hh>
#include <gz/sim/components/Pose.hh>
#include <gz/sim/components/JointPosition.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Model.hh>
#include <gz/math/PID.hh>
#include <gz/math/Vector3.hh>
#include <fstream>

namespace gz
{
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

      std::string jointName = "";
      if (sdf->HasElement("joint")) 
      {
          jointName = sdf->Get<std::string>("joint");
          this->jointEntity = model.JointByName(ecm, jointName);
      }

      if (sdf->HasElement("start_joint_angle")) this->startingPose = sdf->Get<double>("start_joint_angle");
      if (sdf->HasElement("end_joint_angle")) this->eventPose = sdf->Get<double>("end_joint_angle");
      if (sdf->HasElement("trigger_dist")) this->distanceToEvent = sdf->Get<float>("trigger_dist");

      this->pid = gz::math::PID(0.5, 1, 0);

      auto jointPosition = ecm.Component<sim::components::JointPosition>(this->jointEntity);
      if (!jointPosition)
      {
          ecm.CreateComponent(this->jointEntity, sim::components::JointPosition({this->startingPose}));
      }
      else
      {
          jointPosition->Data()[0] = this->startingPose;
          ecm.SetChanged(this->jointEntity, sim::components::JointPosition::typeId);
      }
    }

    // Called by the world update start event
    public: void PreUpdate(const sim::UpdateInfo &info, sim::EntityComponentManager &ecm) override
    {
      float currentTime = std::chrono::duration_cast<std::chrono::seconds>(info.simTime).count();

      if (this->robot == sim::Entity())
      {
        std::string myName = "robile_john";

        ecm.Each<sim::components::Model, sim::components::Name>(
          [&](const sim::Entity &entity, const sim::components::Model *, const sim::components::Name *nameComp) -> bool
          {
            if (nameComp->Data() == myName)
            {
              this->robot = entity;
              return false;
            }
            return true;
          });

        auto jointPosition = ecm.Component<sim::components::JointPosition>(this->jointEntity);
        jointPosition->Data()[0] = this->startingPose;
        ecm.SetChanged(this->jointEntity, sim::components::JointPosition::typeId);
      }
      else
      {
        auto robotPose = ecm.Component<sim::components::Pose>(this->robot)->Data();
        auto doorPose = ecm.Component<sim::components::Pose>(this->entity)->Data();
        float distance = doorPose.Pos().Distance(robotPose.Pos());

        if (distance < this->distanceToEvent)
        {
          auto jointPosition = ecm.Component<sim::components::JointPosition>(this->jointEntity);
          jointPosition->Data()[0] = this->eventPose;
          ecm.SetChanged(this->jointEntity, sim::components::JointPosition::typeId);
        }
      }
    }

    private: sim::Entity entity;
    private: std::shared_ptr<const sdf::Element> sdf;

    private: double startingPose;
    private: double eventPose;
    private: float distanceToEvent;

    private: sim::Entity jointEntity;
    private: sim::Entity robot;
    
    private: gz::math::PID pid;
  };

  GZ_ADD_PLUGIN(DistanceTriggeredJointStateChangePlugin, sim::System, DistanceTriggeredJointStateChangePlugin::ISystemConfigure, DistanceTriggeredJointStateChangePlugin::ISystemPreUpdate)
  GZ_ADD_PLUGIN_ALIAS(DistanceTriggeredJointStateChangePlugin, "gz::sim::systems::DistanceTriggeredJointStateChangePlugin")
}

