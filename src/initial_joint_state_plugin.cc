#include <functional>
#include <gz/sim/System.hh>
#include <gz/sim/Model.hh>
#include <gz/plugin/Register.hh>
#include <gz/sim/components/Pose.hh>
#include <gz/sim/components/JointPositionReset.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Joint.hh>
#include <gz/math/PID.hh>
#include <gz/math/Vector3.hh>
#include <fstream>


namespace gz
{
    class InitialJointStatePlugin : public sim::System,
                               public sim::ISystemConfigure
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
            gzdbg << "Initial state plugin parameters:" << std::endl;
            gzdbg << "start joint angle: [" << this->position << "]" << std::endl;
            gzdbg << "joint: [" << jointName << "]" << std::endl;

            auto jointPosition = ecm.Component<sim::components::JointPositionReset>(this->jointEntity);
            if (!jointPosition)
            {
                ecm.CreateComponent(this->jointEntity, sim::components::JointPositionReset({this->position}));
            }

        }

        private: sim::Entity entity;
        private: std::shared_ptr<const sdf::Element> sdf;
        private: sim::Entity jointEntity;
        private: double position;
    };

    GZ_ADD_PLUGIN(InitialJointStatePlugin, sim::System, InitialJointStatePlugin::ISystemConfigure)
    GZ_ADD_PLUGIN_ALIAS(InitialJointStatePlugin, "gz::sim::systems::InitialJointStatePlugin")
}

