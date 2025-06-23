#include <functional>
#include <gz/sim/System.hh>
#include <gz/sim/Model.hh>
#include <gz/plugin/Register.hh>
#include <gz/sim/components/Pose.hh>
#include <gz/sim/components/JointPosition.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Joint.hh>
#include <gz/math/PID.hh>
#include <gz/math/Vector3.hh>
#include <fstream>

#define DEBUG

namespace gz
{
    class InitialJointStatePlugin : public sim::System,
                               public sim::ISystemConfigure,
                               public sim::ISystemPreUpdate
    {
        public: void Configure(const sim::Entity &entity, const std::shared_ptr<const sdf::Element> &sdf,
                               sim::EntityComponentManager &ecm, sim::EventManager &eventMgr) override
        {
            this->entity = entity;
            this->sdf = sdf;

            auto model = sim::Model(entity);

            std::string jointName = "";
            if (sdf->HasElement("joint")) 
            {
                jointName = sdf->Get<std::string>("joint");
                this->jointEntity = model.JointByName(ecm, jointName);
            }

            this->position = 0;
            if (sdf->HasElement("position")) 
            {
                this->position = sdf->Get<double>("position");
            }

        }

        public: void PreUpdate(const sim::UpdateInfo &info, sim::EntityComponentManager &ecm) override
        {
            auto jointPosition = ecm.Component<sim::components::JointPosition>(this->jointEntity);
            if (!jointPosition)
            {
                ecm.CreateComponent(this->jointEntity, sim::components::JointPosition({this->position}));
            }
            else
            {
                jointPosition->Data()[0] = this->position;
                ecm.SetChanged(this->jointEntity, sim::components::JointPosition::typeId);
            }
        }

        private: sim::Entity entity;
        private: std::shared_ptr<const sdf::Element> sdf;
        private: sim::Entity jointEntity;
        private: double position;
    };

    GZ_ADD_PLUGIN(InitialJointStatePlugin, sim::System, InitialJointStatePlugin::ISystemConfigure, InitialJointStatePlugin::ISystemPreUpdate)
    GZ_ADD_PLUGIN_ALIAS(InitialJointStatePlugin, "gz::sim::systems::InitialJointStatePlugin")
}

