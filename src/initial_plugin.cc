#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include "gazebo/common/UpdateInfo.hh"
#include <jsoncpp/json/json.h>
#include <fstream>

#define DEBUG

namespace gazebo
{
    class InitialStatePlugin : public ModelPlugin 
    {
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
        {
            this->model = _parent;
            this->sdf = _sdf;

            std::string parent_name = _parent->GetName();
            
            // Get the Joint to move and the Joint controller
            std::string joint_name = "";
            if (_sdf->HasElement("joint")) joint_name = _sdf->Get<std::string>("joint");
            this->joint = _parent->GetJoint(joint_name);
            this->controller = _parent->GetJointController();
            // Get the desired positions of the joint
            this->position = 0;
            if (_sdf->HasElement("position")) this->position = _sdf->Get<double>("position");

            // Apply the desired position to the joint
            // controller->SetJointPosition(joint, position);
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&InitialStatePlugin::OnUpdate, this, std::placeholders::_1));

        }

        public: void OnUpdate(const common::UpdateInfo &_info) {
            this->controller->SetJointPosition(this->joint, this->position);
        }

        private: physics::ModelPtr model;
        private: sdf::ElementPtr sdf;
        private: gazebo::physics::JointPtr joint;
        private: gazebo::physics::JointControllerPtr controller;
        private: double position;
        private: event::ConnectionPtr updateConnection;
    };

    GZ_REGISTER_MODEL_PLUGIN(InitialStatePlugin)
}