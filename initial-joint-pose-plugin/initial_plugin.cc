#include <functional>
#include <gz/sim/System.hh>
#include <gz/sim/Model.hh>
#include <gz/plugin/Register.hh>
#include <gz/sim/components/Pose.hh>
#include <gz/sim/components/JointPosition.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Joint.hh>
#include <gz/math/PID.hh>
#include <ignition/math/Vector3.hh>
#include <jsoncpp/json/json.h>
#include <fstream>

#define DEBUG

namespace gz
{
    class InitialStatePlugin : public sim::System,
                               public sim::ISystemConfigure,
                               public sim::ISystemPreUpdate
    {
        public: void Configure(const sim::Entity &entity, const std::shared_ptr<const sdf::Element> &sdf,
                               sim::EntityComponentManager &ecm, sim::EventManager &eventMgr) override
        {
            this->entity = entity;
            this->sdf = sdf;

            auto model = sim::Model(entity);

            // Get the Joint to move and the Joint controller
            std::string jointName = "";
            if (sdf->HasElement("joint")) 
            {
                jointName = sdf->Get<std::string>("joint");
                this->jointEntity = model.JointByName(ecm, jointName);
            }

            // Get the desired positions of the joint
            this->position = 0;
            if (sdf->HasElement("position")) 
            {
                this->position = sdf->Get<double>("position");
            }

            // You don't need to explicitly connect to events here. Implementing ISystemPreUpdate will handle this.
        }

        public: void PreUpdate(const sim::UpdateInfo &info, sim::EntityComponentManager &ecm) override
        {
            // Apply the desired position to the joint
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

    // Register this plugin with the simulator
    GZ_ADD_PLUGIN(InitialStatePlugin, sim::System, InitialStatePlugin::ISystemConfigure, InitialStatePlugin::ISystemPreUpdate)
    GZ_ADD_PLUGIN_ALIAS(InitialStatePlugin, "gz::sim::systems::InitialStatePlugin")
}

