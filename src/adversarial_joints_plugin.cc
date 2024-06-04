#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include "gazebo/common/UpdateInfo.hh"
#include <jsoncpp/json/json.h>
#include <fstream>

namespace gazebo
{
  class AdversarialJointModelPlugin : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {
      // Store the pointer to the model
      this->model = _parent;
      this->sdf = _sdf;

      this->world = physics::get_world();
      this->robot = NULL;

      // Get the Joint controller
      std::string joint_name = "";
      if (_sdf->HasElement("joint")) joint_name = _sdf->Get<std::string>("joint");
      this->joint = _parent->GetJoint(joint_name);
      this->controller = _parent->GetJointController();

      common::PID pid = common::PID(0.5, 1, 0);
      this->controller->SetPositionPID(this->joint->GetScopedName(), pid);

      // Read the keyframes from the json file 
      // if (_sdf->HasElement("<place>")) {
      if (_sdf->HasElement("x")) this->starting_pose = _sdf->Get<double>("x");
      if (_sdf->HasElement("y")) this->event_pose = _sdf->Get<double>("y");
      if (_sdf->HasElement("near")) this->distance_to_event = _sdf->Get<float>("near");

      // Listen to the update event. This event is broadcast every simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&AdversarialJointModelPlugin::OnUpdate, this, std::placeholders::_1));
    }

    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo &_info)
    {
      // Read current sim time      
      float current_time = physics::get_world()->SimTime().Float();

      if (this->robot == NULL){
        std::string my_name = "robile_john";

        std::vector<physics::ModelPtr> models = this->world->Models();
        for(int i=0; i<models.size(); i++) {
          
          if (models[i]->GetName()==my_name){
            this->robot = this->world->ModelByName(my_name);
          }
        }
        this->controller->SetPositionTarget(this->joint->GetScopedName(), this->starting_pose);

      }else{
        // std::cout << "Found the robot!" << this->starting_pose << "\t" << this->event_pose << std::endl;
        ignition::math::Pose3d robot_pose = this->robot->WorldPose();
        ignition::math::Pose3d door_pose = this->model->WorldPose();
        float distance = door_pose.Pos().Distance(robot_pose.Pos());

        // std::cout << "ROBOT POSE: " << robot_pose << "\t DOOR POSE: " << door_pose << "\t DISTANCE: " << door_pose.Pos().Distance(robot_pose.Pos()) << std::endl;
        if (distance < this->distance_to_event) {
          this->controller->SetPositionTarget(this->joint->GetScopedName(), this->event_pose);
        }
      }
    }

    // Pointer to the model
    private: physics::ModelPtr model;
    private: sdf::ElementPtr sdf;

    private: double starting_pose;
    private: double event_pose;
    private: float distance_to_event;
    private: double step;

    private: gazebo::physics::JointControllerPtr controller;
    private: gazebo::physics::JointPtr joint;
    private: physics::ModelPtr robot;
    private: physics::WorldPtr world;
    
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(AdversarialJointModelPlugin)
}